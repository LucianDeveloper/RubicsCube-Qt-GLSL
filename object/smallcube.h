#ifndef SMALLCUBE_H
#define SMALLCUBE_H

#include <wrappers/shaderObject.h>
#include <wrappers/sceneObject.h>

namespace RubicsCube {

class SmallCube : public ShaderObject {
Q_OBJECT
public:
    SmallCube(const int, const QVector3D&, double size);    // Конструктор с инициализацией ID и смещения ящика
    ~SmallCube();                                           // Деструктор

    friend class Cube;
    QVector<QColor> colors;

    virtual void initShader();              // Инициализация шейдерной программы
    virtual int  bind(const CameraObject&, const QVector<LightObject>&); // Связывание шейдерной программы с констекстом. Возвращает кол-во используемых примитивов
    virtual void release();                 // Освобождение контекста от шейдерной программы
    virtual void resetProjection(const int, const int, const float); // Обновление матрицы проектирования для новых размеров экрана
    virtual void resetModelView();          // Обновление видовой матрицы

    // поворот на плоскости X0Y
    void rotateAngleZ(double ang);
    // поворот на плоскости X0Z
    void rotateAngleY(double ang);
    // поворот на плоскости Y0Z
    void rotateAngleX(double ang);
};

}

#endif // SMALLCUBE_H
