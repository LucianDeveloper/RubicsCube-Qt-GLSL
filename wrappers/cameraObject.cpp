#include "wrappers/cameraObject.h"
#include <cmath>

namespace RubicsCube {

CameraObject::CameraObject() : QObject() {
    axis.front = Point3D(0, 0, -1);
    axis.up = Point3D(0, 1, 0);
    resetLook(Point3D(5, 2, 5), Point3D(-5, -2, -5));
}

CameraObject::CameraObject(const Point3D& eyeInit, const Point3D& frontInit) {
    axis.front = frontInit;
    axis.up = Point3D(0, 1, 0);
    resetLook(eyeInit, frontInit);
}

CameraObject::~CameraObject() {}

QMatrix4x4 CameraObject::getLook() const { return lookAt; }

// ���������� ������� ������
Point3D CameraObject::getEye() const { return axis.eye; }

// ���������� ������ ����������� ������
Point3D CameraObject::getFront() const { return axis.front; }


// �������� ������� ���� ������
void CameraObject::resetLook(const Point3D& eyeNew, const Point3D& frontNew) {
    // �������� ���� �� ������ https://ravesli.com/urok-9-kamera-v-opengl/
    axis.eye = eyeNew;
    axis.front = frontNew;

    lookAt.setToIdentity();
    lookAt.lookAt(axis.eye.toQVector3D(), (axis.eye + axis.front).toQVector3D(), axis.up.toQVector3D());
}

// ����������� ������ ������
void CameraObject::translateRight() {
    axis.eye += Point3D(QVector3D::crossProduct(axis.front.toQVector3D(), axis.up.toQVector3D()).normalized()) * info.speed;
    resetLook(axis.eye, axis.front);
}

// ����������� ������ �����
void CameraObject::translateLeft() {
    axis.eye -= Point3D(QVector3D::crossProduct(axis.front.toQVector3D(), axis.up.toQVector3D()).normalized()) * info.speed;
    resetLook(axis.eye, axis.front);
}

// ����������� ������ �����
void CameraObject::translateUp() {
    axis.eye += axis.up * info.speed;
    resetLook(axis.eye, axis.front);
}

// ����������� ������ ����

void CameraObject::translateDown() {
    axis.eye -= axis.up * info.speed;
    resetLook(axis.eye, axis.front);
}

// ����������� ������ ������
void CameraObject::translateDeep() {
    axis.eye += axis.front * info.speed;
    resetLook(axis.eye, axis.front);
}

// ����������� ������ �� �����������, ��������� ������
void CameraObject::translateUndeep() {
    axis.eye -= axis.front * info.speed;
    resetLook(axis.eye, axis.front);
}

// ������������ �������� ������
void CameraObject::rotateSelf(const QPoint& pos) {
    // ��������� �������� �� �����
    QPoint delta = pos - info.mousePosOld;
    info.mousePosOld = pos;
    info.yaw += delta.x() * info.sensitivity;
    info.pitch -= delta.y() * info.sensitivity;

    // ��������� ����������� �� ���� ��������
    if(info.pitch > 89.0)
      info.pitch =  89.0;
    else if(info.pitch < -89.0)
      info.pitch = -89.0;
    if (info.yaw > 360)
        info.yaw -= 360;
    else if (info.yaw < -360)
        info.yaw += 360;

    // ��������� ����� ������ �����������
    Point3D direction;

    direction.setX(cos(info.yaw * M_PI / 180) * cos(info.pitch * M_PI / 180));
    direction.setY(sin(info.pitch * M_PI / 180));
    direction.setZ(sin(info.yaw * M_PI / 180) * cos(info.pitch * M_PI / 180));

    resetLook(axis.eye, direction);
}

// ������� ������ ������ ������� ����
void CameraObject::mouseRightButtonRotate(QMouseEvent* event) {
    rotateSelf(event->pos());
    emit repaintScene();
}

// ������� ������ ����� ������� ����
void CameraObject::mouseLeftButtonRotate(QMouseEvent* event) {
    return;
    emit repaintScene();
}

// ���� �� ������ �����
void CameraObject::mouseClick(QMouseEvent* event) {
    info.mousePosOld = event->pos();
    info.activeMouseButton = event->button();
}

// ������ ���������� ������� ����
void CameraObject::mouseWheel(QWheelEvent* event) {
    info.fov -= event->angleDelta().y() / 120;
    if(info.fov <= CAMERA_FOV_MIN)
      info.fov = CAMERA_FOV_MIN;
    else if(info.fov >= CAMERA_FOV_MAX)
      info.fov = CAMERA_FOV_MAX;

    emit resetProjectionObject(); // ������������ ����
}

// ������ �������
void CameraObject::keyPress(QKeyEvent* event) {
    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W)
       translateDeep();
    else if (event->key() == Qt::Key_Down|| event->key() == Qt::Key_S)
       translateUndeep();
    else if (event->key() == Qt::Key_Right|| event->key() == Qt::Key_D)
       translateRight();
    else if (event->key() == Qt::Key_Left|| event->key() == Qt::Key_A)
       translateLeft();
    else if (event->key() == Qt::Key_Space)
       translateUp();
    else if (event->key() == Qt::Key_Z)
       translateDown();
    emit repaintScene();
}

}
