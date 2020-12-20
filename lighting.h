#ifndef LIGHTING_H
#define LIGHTING_H
#include <QOpenGLShaderProgram>
#include <wrappers/pointObject.h>

namespace RubicsCube {

const QVector3D INIT_LIGHT_LA = QVector3D(0.3, 0.3, 0.3);       // »сходна€ интенсивность фонового света
const QVector3D INIT_LIGHT_LD = QVector3D(0.3, 0.3, 0.3);       // »сходна€ интенсивность рассе€нного света
const QVector3D INIT_LIGHT_LS = QVector3D(0.1, 0.1, 0.1); // »сходна€ интенсивность отраженного света

typedef struct
{
    QVector3D position;             // ѕозици€ источника света в видимых координатах
    QVector3D La = INIT_LIGHT_LA;   // »нтенсивность фонового света
    QVector3D Ld = INIT_LIGHT_LD;   // »нтенсивность рассе€нного света
    QVector3D Ls = INIT_LIGHT_LS;   // »нтенсивность отраженного света
} t_lightInfo;

class LightObject {
    t_lightInfo info;

    public:
        LightObject();                                          //  онструктор по умолчанию
        LightObject(const double, const double, const double);  //  онструктор с инициализацией координат источника света
        LightObject(const QVector3D&);                          //  онструктор с инициализацией позиции источника света
        ~LightObject();                                         // ƒеструктор

        QVector3D getPosition() const;      // ¬озвращает позицию источника света
        QVector3D getLa() const;            // ¬озвращает интенсивность фонового света источника
        QVector3D getLd() const;            // ¬озвращает интенсивность рассе€нного света источника
        QVector3D getLs() const;            // ¬озвращает интенсивность отраженного источника
        void setPosition(const QVector3D&); // »змен€ет позицию источника света
        void setLa(const QVector3D&);       // »змен€ет интенсивность фонового света источника
        void setLd(const QVector3D&);       // »змен€ет интенсивность рассе€нного света источника
        void setLs(const QVector3D&);       // »змен€ет интенсивность отраженного света источника

        void bindLightWithShaderProgram(const QMatrix4x4&, QOpenGLShaderProgram&, const int) const; // —в€зывает данный источник света с шейдерной программой по i-ой позиции
};

}

#endif // LIGHTING_H
