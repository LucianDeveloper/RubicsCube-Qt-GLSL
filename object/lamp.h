#ifndef LAMP_H
#define LAMP_H


#include <wrappers/shaderObject.h>
#include <wrappers/sceneObject.h>
#include <lighting.h>

namespace RubicsCube {

class Lamp : public ShaderObject {
Q_OBJECT
public:
    Lamp(const int, const QVector3D&, QVector3D, double);     // Конструктор с инициализацией ID и смещения ящика
    ~Lamp();

    LightObject light;
    double size;
    QVector<QColor> colors;

    virtual void initShader();              // Инициализация шейдерной программы
    virtual int  bind(const CameraObject&, const QVector<LightObject>&); // Связывание шейдерной программы с констекстом. Возвращает кол-во используемых примитивов
    virtual void release();                 // Освобождение контекста от шейдерной программы
    virtual void resetProjection(const int, const int, const float); // Обновление матрицы проектирования для новых размеров экрана
    virtual void resetModelView();          // Обновление видовой матрицы

    LightObject getLight();
};

}

#endif // LAMP_H
