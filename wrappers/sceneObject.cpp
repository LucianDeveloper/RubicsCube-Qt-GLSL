#include "wrappers/sceneObject.h"
#include <limits>

namespace RubicsCube {

SceneObject::SceneObject() : QOpenGLWidget(),
    camera(new CameraObject), objs(), lights(),
    idActiveObject(ID_ERROR){
    setFormat(QSurfaceFormat());
    this->startTimer(1);
}

// ����������� � �������������� �������� �����
SceneObject::SceneObject(CameraObject* cBuf, const QVector<ShaderObject*>& oBuf) : QOpenGLWidget(), camera(cBuf), objs(oBuf), lights(), idActiveObject(ID_ERROR){
    setFormat(QSurfaceFormat());
}

SceneObject::~SceneObject() {delete camera;}

// ���������� ������� � �����
SceneObject& SceneObject::operator<<(ShaderObject* right) {
    if (right->isSuperClass) *this << right->submodels;
    objs << right;
    return *this;
}

// ���������� �������� � �����
SceneObject& SceneObject::operator<<(const QVector<ShaderObject*>& right) {
    for (auto i : right) objs << i;
    return *this;
}

// ���������� ��������� ����� � �����
SceneObject& SceneObject::operator<<(const LightObject& right) {
    lights << right;
    return *this;
}

// ���������� ���������� ����� � �����
SceneObject& SceneObject::operator<<(const QVector<LightObject>& right) {
    for (auto i : right)
        lights << i;
    return *this;
}

// ������������� ����� ������ �� �����
void SceneObject::setCamera(CameraObject* cameraNew) { camera = cameraNew; }

// ��������� ������� �� ����
void SceneObject::drawObject(ShaderObject* object) {
    int size = object->bind(*camera, lights);
    glDrawArrays(object->getMode(), 0, size);
    object->release();
}

// ���������� ����� pos � �������� ������� ���������
QPointF SceneObject::pointToOpenGLScreen(const QPoint& pos) const {
    float mx = -1.0f + 2.0f * (double) pos.x() / width();
    float my = 1.0f - 2.0f * (double) pos.y() / height();
    return QPointF(mx, my);
}

// ���������� ����� ������������ ����, ����������� �� ������� pos �� ������ � ������ ����������� ������� obj
QPair<Point3D, Point3D> SceneObject::selectionRay(const ShaderObject& obj, const QPoint& pos) const {
    // �������� ���������� ��������� ���� � �������� ������� ��������� OpenGL
    QPointF M = pointToOpenGLScreen(pos);
    QMatrix4x4 IQ = (obj.view.projectMatrix * camera->lookAt * obj.view.modelViewMatrix).inverted();

    // �������� ��������� �������������� ����
    // ��� ����� ����� ����� ��� �����, ������ ����� ������� ��������������� �������� ��������� � ���������� � � ����� P(x, y)
    // ������ ����� A ������ ��������� �� ������� ��������� ��������� (z = -1), ������ ����� B - �� ������� ��������� ��������� (z = 1)
    QVector3D A = (IQ * QVector4D(M.x(), M.y(), -1, 1)).toVector3DAffine();
    QVector3D B = (IQ * QVector4D(M.x(), M.y(), 1, 1)).toVector3DAffine();
    return QPair<Point3D, Point3D>(A, B);;
}

// ���������� ������������� �������, ������� ������� �� ������, ��� -1, ���� ������ �� �������
int SceneObject::selectObject(const QPoint& pos) const {
    int resID = -1; // ������������� ���������� �������
    double minDepth = __DBL_MAX__; // ����������� ������� �������: ����������, ����� �� ������������ ����� �����
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

// ���������� ����� �����
Point3D SceneObject::getCentreScene() const {
    Point3D res(0, 0, 0);
    for (auto i : objs)
        res += Point3D((camera->lookAt * i->view.modelViewMatrix * QVector4D(i->getCentreObject().toQVector3D(), 1)).toVector3D());
    return res;
}

// ������������� ���������
void SceneObject::initializeGL() {
    glEnable(GL_DEPTH_TEST); // ��������� ���������� �� �������
    //glEnable(GL_CULL_FACE); // ����� ��������� ������ ������� ������

    // ������� ����� � ������
    connect(this, &SceneObject::mouseRightButtonMoveCamera, camera, &CameraObject::mouseRightButtonRotate);
    connect(this, &SceneObject::mouseLeftButtonMoveCamera, camera, &CameraObject::mouseLeftButtonRotate);
    connect(this, &SceneObject::mouseClickCamera, camera, &CameraObject::mouseClick);
    connect(this, &SceneObject::mouseWheelCamera, camera, &CameraObject::mouseWheel);
    connect(this, &SceneObject::keyPressCamera, camera, &CameraObject::keyPress);
    connect(camera, &CameraObject::repaintScene, this, &SceneObject::repaintScene3D);
    connect(camera, &CameraObject::resetProjectionObject, this, &SceneObject::resetProjectionScene3D);

    for (auto i : objs) {
        i->initShader(); // �������������� ������� ��������
        i->resetModelView(); // �������������� ������� �������
        connect(this, &SceneObject::mouseMoveObject, i, &ShaderObject::mouseRotate); // ����� ����� ��������� � ����� � ��������� �������
        connect(i, &ShaderObject::repaintScene, this, &SceneObject::repaintScene3D); // ����� ����� ���������� ������� � ������������ �����
        connect(this, &SceneObject::mouseClickObject, i, &ShaderObject::mouseClick); // ����� ����� ������ � ����� � ������ �� �������
        connect(this, &SceneObject::mouseWheelObject, i, &ShaderObject::mouseWheel); // ����� ����� ���������� ����� � ���������� �������
        connect(this, &SceneObject::keyPressObject, i, &ShaderObject::keyPress); // ����� ����� �������� ������ � ����� � �������� ��������
    }
}

// ��������� �������� ����
void SceneObject::resizeGL(int nW, int nH) {
    glViewport(0, 0, nW, nH); // ������� ������� ������

    for (auto i : objs) {
        i->resetProjection(nW, nH, camera->info.fov); // ����� ������� ������������ ��������������
    }
}

// ��������� ����
void SceneObject::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������� ������ ������� � ������ �����
    for (auto i : objs) drawObject(i);
}

// ������� ����� �� ��������
void SceneObject::clear() { objs.clear(); }


// ������, ��� �������������
void SceneObject::timerEvent(QTimerEvent *event){for (auto object: objs){ if (object->isAnimated) object->timerEvent(event);}}


// �������� ������� ������� ����
void SceneObject::mouseMoveEvent(QMouseEvent* event) {
    if (idActiveObject == -1) {
        if (camera->info.activeMouseButton == Qt::RightButton) emit mouseRightButtonMoveCamera(event);
        else if (camera->info.activeMouseButton == Qt::LeftButton) emit mouseLeftButtonMoveCamera(event);
    } else emit mouseMoveObject(idActiveObject, event);
}

// ������� �� ������ ����
void SceneObject::mousePressEvent(QMouseEvent* event) {
    idActiveObject = selectObject(event->pos());
    if (idActiveObject == -1) emit mouseClickCamera(event);
    else emit mouseClickObject(idActiveObject, event);
}

// ���������� �������� �����
void SceneObject::wheelEvent(QWheelEvent* event) {
    idActiveObject = selectObject(event->pos());
    if (idActiveObject == -1) emit mouseWheelCamera(event);
    else emit mouseWheelObject(idActiveObject, event);
}

// ������ ������� �� ����������
void SceneObject::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) this->close();
    if (idActiveObject == -1) emit keyPressCamera(event);
    else emit keyPressObject(idActiveObject, event, objs);
}

// ����������� �����
void SceneObject::repaintScene3D() {update();}

// ��������������� �������� �������� �����
void SceneObject::resetProjectionScene3D() {
    for (auto i : objs) i->resetProjection(width(), height(), camera->info.fov);
    update();
}

}
