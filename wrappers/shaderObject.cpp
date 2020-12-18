#include "wrappers/shaderObject.h"
#include <QDebug>

namespace RubicsCube {
ShaderObject::ShaderObject() : QObject(),
                               id(ID_ERROR),
                               mode(),
                               material(),
                               active(false),
                               view(),
                               object(),
                               shpr(),
                               bias(),
                               initBias(),
                               kScale(1),
                               isSuperClass(false),
                               isAnimated(false),
                               isMaySelect(true),
                               mousePosOld() {}

// Конструктор с инициализацией ID объекта
ShaderObject::ShaderObject(const int idBuf) : QObject(),
                                              id(idBuf),
                                              mode(),
                                              material(),
                                              active(false),
                                              view(),
                                              object(),
                                              shpr(),
                                              bias(),
                                              initBias(),
                                              kScale(1),
                                              isAnimated(false),
                                              isSuperClass(false),
                                              isMaySelect(true),
                                              mousePosOld() {}

// Конструктор с инициализацией граней объекта
ShaderObject::ShaderObject(const GLenum modeBuf, const QVector<EdgeObject>& buf) : QObject(),
                                                                                     id(ID_ERROR),
                                                                                     mode(modeBuf),
                                                                                     material(),
                                                                                     active(false),
                                                                                     view(),
                                                                                     object(buf),
                                                                                     shpr(),
                                                                                     bias(),
                                                                                     initBias(),
                                                                                     kScale(1),
                                                                                     isAnimated(false),
                                                                                     isSuperClass(false),
                                                                                     isMaySelect(true),
                                                                                     mousePosOld() {}

// Конструктор копирования
ShaderObject::ShaderObject(const ShaderObject& buf) : QObject(),
                                                      id(buf.id),
                                                      mode(buf.mode),
                                                      material(buf.material),
                                                      active(buf.active),
                                                      view(buf.view),
                                                      object(buf.object),
                                                      shpr(),
                                                      bias(buf.bias),
                                                      initBias(buf.initBias),
                                                      kScale(buf.kScale),
                                                      isSuperClass(false),
                                                      isAnimated(false),
                                                      isMaySelect(true),
                                                      mousePosOld(buf.mousePosOld) {}

// Деструктор
ShaderObject::~ShaderObject() {}

// Присваивание
ShaderObject& ShaderObject::operator=(const ShaderObject& right){
    if (this == &right) return *this;
    id = right.id;
    mode = right.mode;
    material = right.material;
    active = right.active;
    view = right.view;
    object = right.object;
    bias = right.bias;
    initBias = right.initBias;
    kScale = right.kScale;
    mousePosOld = right.mousePosOld;
    isAnimated = right.isAnimated;
    isMaySelect = right.isMaySelect;
    return *this;
}

// Индексация
EdgeObject& ShaderObject::operator[](const int i) {
    if (i >= size()) { qDebug() << "Error: out of range in ShaderObject3D"; exit(EXIT_FAILURE);}
    return object[i];
}

// Индексация (константная)
const EdgeObject& ShaderObject::operator[](const int i) const {
    if (i >= size()) {qDebug() << "Error: out of range in ShaderObject3D"; exit(EXIT_FAILURE);}
    return object[i];
}

// Проверка на равенство
bool ShaderObject::operator==(const ShaderObject& right) const { return object == right.object;}

// Проверка на неравенство
bool ShaderObject::operator!=(const ShaderObject& right) const { return object != right.object; }

// Добавление грани в объект
ShaderObject& ShaderObject::operator<<(const EdgeObject& right) { object.push_back(right); return *this;}

// Добавление граней в объект
ShaderObject& ShaderObject::operator<<(const QVector<EdgeObject>& right) {
    for (int i = 0; i < right.size(); i++) *this << right[i];
    return *this;
}

// Возвращает координаты точек граней объекта в форме QVector<float>
QVector<float> ShaderObject::getVectorEdgeCoords() const {
    QVector<float> res;
    for (auto i : object) res += i.getVectorPointsCoords();
    return res;
}

// Возвращает байты цвета точек граней объекта в форме QVector<float>, приведенные к диапазону [0, 1]
QVector<float> ShaderObject::getVectorEdgeColor() const {
    QVector<float> res;
    for (auto i : object) res += i.getVectorPointsColor();
    return res;
}

// Возвращает нормали точек граней объекта в форме QVector<float>
QVector<float> ShaderObject::getVectorEdgeNormal() const {
    QVector<float> res;
    for (auto i : object) res += i.getVectorPointsNormal();
    return res;
}

// Возвращает примитив, которыми построен объект
GLenum ShaderObject::getMode() const {return mode;}

// Возвращает идентификатор объекта
int ShaderObject::getID() const{return id;}

// Возвращает материал объекта
t_materialInfo ShaderObject::getMaterial() const {return material;}

// Возвращает смещение объекта
t_bias ShaderObject::getBias() const {return bias;}

// Возвращает исходное смещение объекта
t_bias ShaderObject::getInitBias() const{return initBias;}

// Задает исходное смещение ящика
void ShaderObject::setInitBias(const QVector3D& vec) {
    initBias.dx = vec.x();
    initBias.dy = vec.y();
    initBias.dz = vec.z();
}

// Возвращает true, если отображение объекта включено, иначе возвращает false
bool ShaderObject::check() const { return active;}

// Изменяет объект (с помощью другого объекта)
void ShaderObject::setObject(const ShaderObject& right){
    id = right.id;
    mode = right.mode;
    active = right.active;
    view = right.view;
    object = right.object;
    bias = right.bias;
    kScale = right.kScale;
    mousePosOld = right.mousePosOld;
}

// Устанавливает новый примитив, которыми построен объект
void ShaderObject::setMode(GLenum newMode) {mode = newMode;}

// Устанавливает новый идентификатор объекту
void ShaderObject::setID(const int newId){id = newId;}

// Включает отображение объекта
void ShaderObject::on() {active = true;}

// Выключает отображение объекта
void ShaderObject::off() {active = false;}

// Добавляет грань в объект
void ShaderObject::addEdge3D(const EdgeObject& right) {object << right;}

// Возвращает центр объекта
Point3D ShaderObject::getCentreObject() const{
    Point3D res(0, 0, 0);
    for (auto i : object) res += i.getCenterEdge();
    return res / object.size();
}

// Возвращает глубину точки в преобразованных координатах
float ShaderObject::depthPoint(const Point3D& point, const CameraObject& camera) const {
    return -(camera.getLook() * view.modelViewMatrix * QVector4D(point.x(), point.y(), point.z(), 1)).z();
}

// Возвращает кол-во граней в объекте
int ShaderObject::size() const {return object.size();}

// Поворот объекта мышью
void ShaderObject::mouseRotate(const int idCheck, QMouseEvent* event){
    if (id != idCheck) // Если сигнал был послан другому объекту, то ничего не делаем
        return;

    // Вычислим, на сколько переместился курсор мыши между двумя событиями mouseMoveEvent
    QPoint dp = event->pos() - mousePosOld;
    // Изменим матрицу поворота в соответствии с тем, как пользователь переместил курсор мыши
    view.rotateMatrix.rotate(-dp.x(), 0, 1, 0); // Умножение R на матрицу поворота вокруг оси y
    view.rotateMatrix.rotate(-dp.y(), 1, 0, 0); // Умножение R на матрицу поворота вокруг оси x
    // Сохраним текущую позицию мыши
    mousePosOld = event->pos();
    // Обновим матрицу аффинных преобразований
    resetModelView();
    emit repaintScene();
}

// Клик по объекту мышью
void ShaderObject::mouseClick(const int idCheck, QMouseEvent* event) {
    if (id != idCheck) // Если сигнал был послан другому объекту, то ничего не делаем
        return;
    mousePosOld = event->pos();
}

// Объект прокрутили колесом мыши
void ShaderObject::mouseWheel(const int idCheck, QWheelEvent* event) {
    if (id != idCheck) // Если сигнал был послан другому объекту, то ничего не делаем
        return;

    // При прокрутке колеса мыши изменяем масштаб объекта
    kScale += event->delta() / 2000.0;

    if (kScale < KSCALE_MIN) kScale = KSCALE_MIN;
    else if (kScale > KSCALE_MAX) kScale = KSCALE_MAX;
    resetModelView(); // Обновим матрицу аффинных преобразований
    emit repaintScene(); // Перерисовать окно
}

// Когда объект был выделен, нажали клавишу
void ShaderObject::keyPress(const int idCheck, QKeyEvent* event, const QVector<ShaderObject*>&) {
    if (id != idCheck) // Если сигнал был послан другому объекту, то ничего не делаем
        return;

    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W)
        bias.dz += BIAS_TRANSLATE;
    else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S)
        bias.dz -= BIAS_TRANSLATE;
    else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D)
        bias.dx += BIAS_TRANSLATE;
    else if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A)
        bias.dx -= BIAS_TRANSLATE;
    else if (event->key() == Qt::Key_Space)
        bias.dy += BIAS_TRANSLATE;
    else if (event->key() == Qt::Key_Z)
        bias.dy -= BIAS_TRANSLATE;
    resetModelView();
    emit repaintScene();
}

}
