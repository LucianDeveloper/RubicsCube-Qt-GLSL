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

// ����������� � �������������� ID �������
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

// ����������� � �������������� ������ �������
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

// ����������� �����������
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

// ����������
ShaderObject::~ShaderObject() {}

// ������������
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

// ����������
EdgeObject& ShaderObject::operator[](const int i) {
    if (i >= size()) { qDebug() << "Error: out of range in ShaderObject3D"; exit(EXIT_FAILURE);}
    return object[i];
}

// ���������� (�����������)
const EdgeObject& ShaderObject::operator[](const int i) const {
    if (i >= size()) {qDebug() << "Error: out of range in ShaderObject3D"; exit(EXIT_FAILURE);}
    return object[i];
}

// �������� �� ���������
bool ShaderObject::operator==(const ShaderObject& right) const { return object == right.object;}

// �������� �� �����������
bool ShaderObject::operator!=(const ShaderObject& right) const { return object != right.object; }

// ���������� ����� � ������
ShaderObject& ShaderObject::operator<<(const EdgeObject& right) { object.push_back(right); return *this;}

// ���������� ������ � ������
ShaderObject& ShaderObject::operator<<(const QVector<EdgeObject>& right) {
    for (int i = 0; i < right.size(); i++) *this << right[i];
    return *this;
}

// ���������� ���������� ����� ������ ������� � ����� QVector<float>
QVector<float> ShaderObject::getVectorEdgeCoords() const {
    QVector<float> res;
    for (auto i : object) res += i.getVectorPointsCoords();
    return res;
}

// ���������� ����� ����� ����� ������ ������� � ����� QVector<float>, ����������� � ��������� [0, 1]
QVector<float> ShaderObject::getVectorEdgeColor() const {
    QVector<float> res;
    for (auto i : object) res += i.getVectorPointsColor();
    return res;
}

// ���������� ������� ����� ������ ������� � ����� QVector<float>
QVector<float> ShaderObject::getVectorEdgeNormal() const {
    QVector<float> res;
    for (auto i : object) res += i.getVectorPointsNormal();
    return res;
}

// ���������� ��������, �������� �������� ������
GLenum ShaderObject::getMode() const {return mode;}

// ���������� ������������� �������
int ShaderObject::getID() const{return id;}

// ���������� �������� �������
t_materialInfo ShaderObject::getMaterial() const {return material;}

// ���������� �������� �������
t_bias ShaderObject::getBias() const {return bias;}

// ���������� �������� �������� �������
t_bias ShaderObject::getInitBias() const{return initBias;}

// ������ �������� �������� �����
void ShaderObject::setInitBias(const QVector3D& vec) {
    initBias.dx = vec.x();
    initBias.dy = vec.y();
    initBias.dz = vec.z();
}

// ���������� true, ���� ����������� ������� ��������, ����� ���������� false
bool ShaderObject::check() const { return active;}

// �������� ������ (� ������� ������� �������)
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

// ������������� ����� ��������, �������� �������� ������
void ShaderObject::setMode(GLenum newMode) {mode = newMode;}

// ������������� ����� ������������� �������
void ShaderObject::setID(const int newId){id = newId;}

// �������� ����������� �������
void ShaderObject::on() {active = true;}

// ��������� ����������� �������
void ShaderObject::off() {active = false;}

// ��������� ����� � ������
void ShaderObject::addEdge3D(const EdgeObject& right) {object << right;}

// ���������� ����� �������
Point3D ShaderObject::getCentreObject() const{
    Point3D res(0, 0, 0);
    for (auto i : object) res += i.getCenterEdge();
    return res / object.size();
}

// ���������� ������� ����� � ��������������� �����������
float ShaderObject::depthPoint(const Point3D& point, const CameraObject& camera) const {
    return -(camera.getLook() * view.modelViewMatrix * QVector4D(point.x(), point.y(), point.z(), 1)).z();
}

// ���������� ���-�� ������ � �������
int ShaderObject::size() const {return object.size();}

// ������� ������� �����
void ShaderObject::mouseRotate(const int idCheck, QMouseEvent* event){
    if (id != idCheck) // ���� ������ ��� ������ ������� �������, �� ������ �� ������
        return;

    // ��������, �� ������� ������������ ������ ���� ����� ����� ��������� mouseMoveEvent
    QPoint dp = event->pos() - mousePosOld;
    // ������� ������� �������� � ������������ � ���, ��� ������������ ���������� ������ ����
    view.rotateMatrix.rotate(-dp.x(), 0, 1, 0); // ��������� R �� ������� �������� ������ ��� y
    view.rotateMatrix.rotate(-dp.y(), 1, 0, 0); // ��������� R �� ������� �������� ������ ��� x
    // �������� ������� ������� ����
    mousePosOld = event->pos();
    // ������� ������� �������� ��������������
    resetModelView();
    emit repaintScene();
}

// ���� �� ������� �����
void ShaderObject::mouseClick(const int idCheck, QMouseEvent* event) {
    if (id != idCheck) // ���� ������ ��� ������ ������� �������, �� ������ �� ������
        return;
    mousePosOld = event->pos();
}

// ������ ���������� ������� ����
void ShaderObject::mouseWheel(const int idCheck, QWheelEvent* event) {
    if (id != idCheck) // ���� ������ ��� ������ ������� �������, �� ������ �� ������
        return;

    // ��� ��������� ������ ���� �������� ������� �������
    kScale += event->delta() / 2000.0;

    if (kScale < KSCALE_MIN) kScale = KSCALE_MIN;
    else if (kScale > KSCALE_MAX) kScale = KSCALE_MAX;
    resetModelView(); // ������� ������� �������� ��������������
    emit repaintScene(); // ������������ ����
}

// ����� ������ ��� �������, ������ �������
void ShaderObject::keyPress(const int idCheck, QKeyEvent* event, const QVector<ShaderObject*>&) {
    if (id != idCheck) // ���� ������ ��� ������ ������� �������, �� ������ �� ������
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
