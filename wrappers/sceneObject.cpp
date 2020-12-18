#include "wrappers/sceneObject.h"
#include <limits>

namespace RubicsCube {

SceneObject::SceneObject() : QOpenGLWidget(),
    camera(new CameraObject), objs(), lights(),
    idActiveObject(ID_ERROR){
    setFormat(QSurfaceFormat());
    this->startTimer(1);
}

// Конструктор с инициализацией объектов сцены
SceneObject::SceneObject(CameraObject* cBuf, const QVector<ShaderObject*>& oBuf) : QOpenGLWidget(), camera(cBuf), objs(oBuf), lights(), idActiveObject(ID_ERROR){
    setFormat(QSurfaceFormat());
}

SceneObject::~SceneObject() {delete camera;}

// Добавление объекта в сцену
SceneObject& SceneObject::operator<<(ShaderObject* right) {
    if (right->isSuperClass) *this << right->submodels;
    objs << right;
    return *this;
}

// Добавление объектов в сцену
SceneObject& SceneObject::operator<<(const QVector<ShaderObject*>& right) {
    for (auto i : right) objs << i;
    return *this;
}

// Добавление источника света в сцену
SceneObject& SceneObject::operator<<(const LightObject& right) {
    lights << right;
    return *this;
}

// Добавление источников света в сцену
SceneObject& SceneObject::operator<<(const QVector<LightObject>& right) {
    for (auto i : right)
        lights << i;
    return *this;
}

// Устанавливает новую камеру на сцене
void SceneObject::setCamera(CameraObject* cameraNew) { camera = cameraNew; }

// Отрисовка объекта на окне
void SceneObject::drawObject(ShaderObject* object) {
    int size = object->bind(*camera, lights);
    glDrawArrays(object->getMode(), 0, size);
    object->release();
}

// Возвращает точку pos в экранной системе координат
QPointF SceneObject::pointToOpenGLScreen(const QPoint& pos) const {
    float mx = -1.0f + 2.0f * (double) pos.x() / width();
    float my = 1.0f - 2.0f * (double) pos.y() / height();
    return QPointF(mx, my);
}

// Возвращает концы селективного луча, полученного из позиции pos на экране и матриц отображения объекта obj
QPair<Point3D, Point3D> SceneObject::selectionRay(const ShaderObject& obj, const QPoint& pos) const {
    // Вычислим координаты указателя мыши в экранной системе координат OpenGL
    QPointF M = pointToOpenGLScreen(pos);
    QMatrix4x4 IQ = (obj.view.projectMatrix * camera->lookAt * obj.view.modelViewMatrix).inverted();

    // Вычислим параметры селектирующего луча
    // Для этого нужно взять две точки, прямая через которые перпендикулярна экранной плоскости и пересекает её в точке P(x, y)
    // Первая точка A должна находится на ближней плоскости отсечения (z = -1), вторая точка B - на дальней плоскости отсечения (z = 1)
    QVector3D A = (IQ * QVector4D(M.x(), M.y(), -1, 1)).toVector3DAffine();
    QVector3D B = (IQ * QVector4D(M.x(), M.y(), 1, 1)).toVector3DAffine();
    return QPair<Point3D, Point3D>(A, B);;
}

// Возвращает идентификатор объекта, который выбрали на экране, или -1, если ничего не выбрано
int SceneObject::selectObject(const QPoint& pos) const {
    int resID = -1; // Идентификатор выбранного объекта
    double minDepth = __DBL_MAX__; // Минимальная глубина объекта: определяем, какой из пересеченных лучом ближе
    for (auto object : objs) {
        if (!(object->isMaySelect)) continue;
        QPair<Point3D, Point3D> ray = selectionRay(*object, pos);
        for (int i = 0; i < object->size(); i++) {
            QPair<bool, Point3D> intersect = (*object)[i].checkIntersectVector(ray);
            float objDepth = object->depthPoint(intersect.second, *camera);
            if (intersect.first && (objDepth - minDepth < 0)){
                minDepth = objDepth;
                resID = object->getID();
            }
        }
    }
    return resID;
}

// Возвращает центр сцены
Point3D SceneObject::getCentreScene() const {
    Point3D res(0, 0, 0);
    for (auto i : objs)
        res += Point3D((camera->lookAt * i->view.modelViewMatrix * QVector4D(i->getCentreObject().toQVector3D(), 1)).toVector3D());
    return res;
}

// Инициализация контекста
void SceneObject::initializeGL() {
    glEnable(GL_DEPTH_TEST); // Включение сортировки по глубине
    //glEnable(GL_CULL_FACE); // Режим рисования только лицевых граней

    // Коннект сцены и камеры
    connect(this, &SceneObject::mouseRightButtonMoveCamera, camera, &CameraObject::mouseRightButtonRotate);
    connect(this, &SceneObject::mouseLeftButtonMoveCamera, camera, &CameraObject::mouseLeftButtonRotate);
    connect(this, &SceneObject::mouseClickCamera, camera, &CameraObject::mouseClick);
    connect(this, &SceneObject::mouseWheelCamera, camera, &CameraObject::mouseWheel);
    connect(this, &SceneObject::keyPressCamera, camera, &CameraObject::keyPress);
    connect(camera, &CameraObject::repaintScene, this, &SceneObject::repaintScene3D);
    connect(camera, &CameraObject::resetProjectionObject, this, &SceneObject::resetProjectionScene3D);

    for (auto i : objs) {
        i->initShader(); // Инициализируем шейдеры объектов
        i->resetModelView(); // Инициализируем видовую матрицу
        connect(this, &SceneObject::mouseMoveObject, i, &ShaderObject::mouseRotate); // Связь между вращением в сцене и вращением объекта
        connect(i, &ShaderObject::repaintScene, this, &SceneObject::repaintScene3D); // Связь между изменением объекта и перерисовкой сцены
        connect(this, &SceneObject::mouseClickObject, i, &ShaderObject::mouseClick); // Связь между кликом в сцене и кликом по объекту
        connect(this, &SceneObject::mouseWheelObject, i, &ShaderObject::mouseWheel); // Связь между прокруткой сцены и прокруткой объекта
        connect(this, &SceneObject::keyPressObject, i, &ShaderObject::keyPress); // Связь между нажатием клавиш в сцене и реакцией объектов
    }
}

// Изменение размеров окна
void SceneObject::resizeGL(int nW, int nH) {
    glViewport(0, 0, nW, nH); // Задание области вывода

    for (auto i : objs) {
        i->resetProjection(nW, nH, camera->info.fov); // Задаём матрицу центрального проектирования
    }
}

// Отрисовка окна
void SceneObject::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистка буфера глубины и буфера цвета
    for (auto i : objs) drawObject(i);
}

// Очистка сцены от объектов
void SceneObject::clear() { objs.clear(); }


// Таймер, для переотрисовки
void SceneObject::timerEvent(QTimerEvent *event){for (auto object: objs){ if (object->isAnimated) object->timerEvent(event);}}


// Движение зажатой кнопкой мыши
void SceneObject::mouseMoveEvent(QMouseEvent* event) {
    if (idActiveObject == -1) {
        if (camera->info.activeMouseButton == Qt::RightButton) emit mouseRightButtonMoveCamera(event);
        else if (camera->info.activeMouseButton == Qt::LeftButton) emit mouseLeftButtonMoveCamera(event);
    } else emit mouseMoveObject(idActiveObject, event);
}

// Нажатие на кнопку мыши
void SceneObject::mousePressEvent(QMouseEvent* event) {
    idActiveObject = selectObject(event->pos());
    if (idActiveObject == -1) emit mouseClickCamera(event);
    else emit mouseClickObject(idActiveObject, event);
}

// Прокручено колесико мышки
void SceneObject::wheelEvent(QWheelEvent* event) {
    idActiveObject = selectObject(event->pos());
    if (idActiveObject == -1) emit mouseWheelCamera(event);
    else emit mouseWheelObject(idActiveObject, event);
}

// Нажата клавиша на клавиатуре
void SceneObject::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) this->close();
    if (idActiveObject == -1) emit keyPressCamera(event);
    else emit keyPressObject(idActiveObject, event, objs);
}

// Перерисовка сцены
void SceneObject::repaintScene3D() {update();}

// Переопределение проекции объектов сцены
void SceneObject::resetProjectionScene3D() {
    for (auto i : objs) i->resetProjection(width(), height(), camera->info.fov);
    update();
}

}
