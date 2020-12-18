#ifndef CUBE_H
#define CUBE_H
#include "object/smallcube.h"


namespace RubicsCube {


// Big Rubic's Cube
class Cube : public ShaderObject {
Q_OBJECT
public:
    Cube(const int, const QVector3D&); // Конструктор с инициализацией ID и смещения ящика
    ~Cube();                           // Деструктор

    bool rotFlag;           // Вращается прямо сейчас
    bool autoRot;
    int rotate[6];          // храним угол поворота каждой грани
    double rotAngle;
    int idx;
    QVector<QVector<int>> sides;  // 6 граней по 9 кубов

    virtual void initShader();  // Инициализация шейдерной программы
    virtual int  bind(const CameraObject&, const QVector<LightObject>&); // Связывание шейдерной программы с констекстом. Возвращает кол-во используемых примитивов
    virtual void release();     // Освобождение контекста от шейдерной программы
    virtual void resetProjection(const int, const int, const float); // Обновление матрицы проектирования для новых размеров экрана
    virtual void resetModelView(); // Обновление видовой матрицы

    void rot90(int idx, int sign);
    void Rotate(int angle, int reverse);   // крутит грань под номером idx на угол angle (в градусах)
    void timerEvent(QTimerEvent *event);

public slots:
    virtual void mouseRotate(const int, QMouseEvent*);  // Поворот объекта мышью
    virtual void mouseWheel(const int, QWheelEvent*);   // Объект прокрутили колесом мыши
    virtual void keyPress(const int, QKeyEvent*, const QVector<ShaderObject*>&); // Когда объект был выделен, нажали клавишу
};

}
#endif // CUBE_H
