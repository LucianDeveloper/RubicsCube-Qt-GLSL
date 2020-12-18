#ifndef CAMERAOBJECT_H
#define CAMERAOBJECT_H
#include <QObject>
#include <QMatrix4x4>
#include <QMouseEvent>
#include "wrappers/pointObject.h"

namespace RubicsCube {
const float CAMERA_INIT_SPEED = 0.1;       // Изначальная скорость камеры
const float CAMERA_INIT_SENSITIVITY = 0.2; // Изначальная чувствительность камеры
const float CAMERA_INIT_FOV = 70.0;        // Изначальное поле обзора камеры
const float CAMERA_FOV_MAX = 70.0;         // Максимальное поле обзора камеры
const float CAMERA_FOV_MIN = 1.0;          // Минимальное поле обзора камеры

typedef struct
{
    Point3D front; // Вектор перемещения камеры
    Point3D eye;   // Позиция камеры в мировых координатах
    Point3D up;    // Вектор-вверх камеры
  } t_cameraAxis;

typedef struct
  {
    float speed = CAMERA_INIT_SPEED;                // Скорость перемещения камеры
    float sensitivity = CAMERA_INIT_SENSITIVITY;    // Чувствительность мыши
    float fov = CAMERA_INIT_FOV;                    // Поле обзора камеры
    float yaw = -90;                                // Рыскание (для поворота)
    float pitch = 0;                                // Тангаж (для поворота)
    QPoint mousePosOld;                             // Где был предыдущий клик мыши
    Qt::MouseButtons activeMouseButton = Qt::NoButton; // Какая клавиша мыши была нажата в данный момент
  } t_cameraInfo;

class CameraObject : public QObject
  {
    Q_OBJECT

    friend class SceneObject;

    t_cameraAxis axis; // Оси системы координат камеры
    t_cameraInfo info; // Информация о характеристиках камеры
    QMatrix4x4 lookAt; // Матрица вида камеры

    public:
        CameraObject(); // Конструктор по умолчанию
        CameraObject(const Point3D&, const Point3D&); // Конструктор с инициализацией позиции и направления взгляда камеры
        ~CameraObject(); // Деструктор

        QMatrix4x4 getLook() const; // Возвращает матрицу вида камеры
        Point3D getEye() const;     // Возвращает позицию камеры
        Point3D getFront() const;   // Возвращает вектор направления камеры

        void resetLook(const Point3D&, const Point3D&); // Изменяет матрицу вида камеры
        void translateRight();  // Передвигает камеру вправо
        void translateLeft();   // Передвигает камеру влево
        void translateUp();     // Передвигает камеру вверх
        void translateDown();   // Передвигает камеру вниз
        void translateDeep();   // Передвигает камеру вглубь
        void translateUndeep(); // Передвигает камеру по направлению, обратному вглубь
        void rotateSelf(const QPoint&); // Поворачивает объектив камеры

    signals:
        void repaintScene();            // Сигнал, что камера изменилась и сцену нужно перерисовать
        void resetProjectionObject();   // Сигнал, что камера изменила плоскость проекции (напр., при масштабировании)

    public slots:
        virtual void mouseRightButtonRotate(QMouseEvent*);  // Поворот камеры правой кнопкой мыши
        virtual void mouseLeftButtonRotate(QMouseEvent*);   // Поворот камеры правой кнопкой мыши
        virtual void mouseClick(QMouseEvent*);              // Клик по камере мышью
        virtual void mouseWheel(QWheelEvent*);              // Камеру прокрутили колесом мыши
        virtual void keyPress(QKeyEvent*);                  // Нажали клавишу
  };

}

#endif // CAMERAOBJECT_H
