#ifndef LIGHTING_H
#define LIGHTING_H
#include <QOpenGLShaderProgram>
#include <wrappers/pointObject.h>

namespace RubicsCube {

const QVector3D INIT_LIGHT_LA = QVector3D(0.3, 0.3, 0.3);       // �������� ������������� �������� �����
const QVector3D INIT_LIGHT_LD = QVector3D(0.3, 0.3, 0.3);       // �������� ������������� ����������� �����
const QVector3D INIT_LIGHT_LS = QVector3D(0.1, 0.1, 0.1); // �������� ������������� ����������� �����

typedef struct
{
    QVector3D position;             // ������� ��������� ����� � ������� �����������
    QVector3D La = INIT_LIGHT_LA;   // ������������� �������� �����
    QVector3D Ld = INIT_LIGHT_LD;   // ������������� ����������� �����
    QVector3D Ls = INIT_LIGHT_LS;   // ������������� ����������� �����
} t_lightInfo;

class LightObject {
    t_lightInfo info;

    public:
        LightObject();                                          // ����������� �� ���������
        LightObject(const double, const double, const double);  // ����������� � �������������� ��������� ��������� �����
        LightObject(const QVector3D&);                          // ����������� � �������������� ������� ��������� �����
        ~LightObject();                                         // ����������

        QVector3D getPosition() const;      // ���������� ������� ��������� �����
        QVector3D getLa() const;            // ���������� ������������� �������� ����� ���������
        QVector3D getLd() const;            // ���������� ������������� ����������� ����� ���������
        QVector3D getLs() const;            // ���������� ������������� ����������� ���������
        void setPosition(const QVector3D&); // �������� ������� ��������� �����
        void setLa(const QVector3D&);       // �������� ������������� �������� ����� ���������
        void setLd(const QVector3D&);       // �������� ������������� ����������� ����� ���������
        void setLs(const QVector3D&);       // �������� ������������� ����������� ����� ���������

        void bindLightWithShaderProgram(const QMatrix4x4&, QOpenGLShaderProgram&, const int) const; // ��������� ������ �������� ����� � ��������� ���������� �� i-�� �������
};

}

#endif // LIGHTING_H
