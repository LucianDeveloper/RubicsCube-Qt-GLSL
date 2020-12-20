#ifndef SKY_H
#define SKY_H

#include <wrappers/shaderObject.h>
#include <wrappers/sceneObject.h>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QOpenGLTexture>

namespace RubicsCube {

class Sky : public ShaderObject {
Q_OBJECT
public:
    Sky(const int, const QVector3D&); // Конструктор с инициализацией ID и смещения ящика
    ~Sky();                           // Деструктор

    // Текстуры
    GLuint textureLoc[5];
    QOpenGLBuffer vbo;

    virtual void initShader();              // Инициализация шейдерной программы
    virtual int  bind(const CameraObject&, const QVector<LightObject>&); // Связывание шейдерной программы с констекстом. Возвращает кол-во используемых примитивов
    virtual void release();                 // Освобождение контекста от шейдерной программы
    virtual void resetProjection(const int, const int, const float); // Обновление матрицы проектирования для новых размеров экрана
    virtual void resetModelView();          // Обновление видовой матрицы
};

}

#endif // SKY_H
