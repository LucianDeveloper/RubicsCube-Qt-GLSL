#ifndef CAMERAOBJECT_H
#define CAMERAOBJECT_H
#include <QObject>
#include <QMatrix4x4>
#include <QMouseEvent>
#include "wrappers/pointObject.h"

namespace RubicsCube {
const float CAMERA_INIT_SPEED = 0.1;       // ����������� �������� ������
const float CAMERA_INIT_SENSITIVITY = 0.2; // ����������� ���������������� ������
const float CAMERA_INIT_FOV = 70.0;        // ����������� ���� ������ ������
const float CAMERA_FOV_MAX = 70.0;         // ������������ ���� ������ ������
const float CAMERA_FOV_MIN = 1.0;          // ����������� ���� ������ ������

typedef struct
{
    Point3D front; // ������ ����������� ������
    Point3D eye;   // ������� ������ � ������� �����������
    Point3D up;    // ������-����� ������
  } t_cameraAxis;

typedef struct
  {
    float speed = CAMERA_INIT_SPEED;                // �������� ����������� ������
    float sensitivity = CAMERA_INIT_SENSITIVITY;    // ���������������� ����
    float fov = CAMERA_INIT_FOV;                    // ���� ������ ������
    float yaw = -90;                                // �������� (��� ��������)
    float pitch = 0;                                // ������ (��� ��������)
    QPoint mousePosOld;                             // ��� ��� ���������� ���� ����
    Qt::MouseButtons activeMouseButton = Qt::NoButton; // ����� ������� ���� ���� ������ � ������ ������
  } t_cameraInfo;

class CameraObject : public QObject
  {
    Q_OBJECT

    friend class SceneObject;

    t_cameraAxis axis; // ��� ������� ��������� ������
    t_cameraInfo info; // ���������� � ��������������� ������
    QMatrix4x4 lookAt; // ������� ���� ������

    public:
        CameraObject(); // ����������� �� ���������
        CameraObject(const Point3D&, const Point3D&); // ����������� � �������������� ������� � ����������� ������� ������
        ~CameraObject(); // ����������

        QMatrix4x4 getLook() const; // ���������� ������� ���� ������
        Point3D getEye() const;     // ���������� ������� ������
        Point3D getFront() const;   // ���������� ������ ����������� ������

        void resetLook(const Point3D&, const Point3D&); // �������� ������� ���� ������
        void translateRight();  // ����������� ������ ������
        void translateLeft();   // ����������� ������ �����
        void translateUp();     // ����������� ������ �����
        void translateDown();   // ����������� ������ ����
        void translateDeep();   // ����������� ������ ������
        void translateUndeep(); // ����������� ������ �� �����������, ��������� ������
        void rotateSelf(const QPoint&); // ������������ �������� ������

    signals:
        void repaintScene();            // ������, ��� ������ ���������� � ����� ����� ������������
        void resetProjectionObject();   // ������, ��� ������ �������� ��������� �������� (����., ��� ���������������)

    public slots:
        virtual void mouseRightButtonRotate(QMouseEvent*);  // ������� ������ ������ ������� ����
        virtual void mouseLeftButtonRotate(QMouseEvent*);   // ������� ������ ������ ������� ����
        virtual void mouseClick(QMouseEvent*);              // ���� �� ������ �����
        virtual void mouseWheel(QWheelEvent*);              // ������ ���������� ������� ����
        virtual void keyPress(QKeyEvent*);                  // ������ �������
  };

}

#endif // CAMERAOBJECT_H
