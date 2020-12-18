#ifndef POINTOBJECT_H
#define POINTOBJECT_H
#include <QVector3D>
#include <QVector>
#include <QPointF>
#include <QColor>

namespace RubicsCube {

class Point3D
  {
    QVector3D point;    // ����� � 3-������ ������������
    QColor color;       // ���� ����� � ������� RGB

    public:
        Point3D(); // ����������� �� ���������: ��� ���������� ����� 0
        Point3D(const double, const double, const double); // ������������� ����� ��������� ������������
        Point3D(const double, const double, const double, const QColor&); // ������������� ����� ��������� ������������ � ������
        Point3D(const QVector3D&);                  // ������������� ����� ��������
        Point3D(const QVector3D&, const QColor&);   // ������������� ����� �������� � ������
        Point3D(const Point3D&);                    // ����������� �����������
        ~Point3D();                                 // ����������

        Point3D& operator=(const Point3D&);             // ������������ �����
        Point3D& operator=(const QVector3D&);           // ������������ �������
        Point3D operator+() const;                      // ������� ����: ���������� ����� ��� ���������
        Point3D operator+(const Point3D&) const;        // ��������: ������������ ��������������� ����������
        Point3D& operator+=(const Point3D&);            // �������������� ��������
        Point3D operator-() const;                      // ������� �����: ���������� ����� � ���������������� ������������
        Point3D operator-(const Point3D&) const;        // ���������: ���������� ��������������� ����������
        Point3D& operator-=(const Point3D&);            // �������������� ���������
        friend Point3D operator*(const Point3D&, const double); // ��������� �� ����� ������
        Point3D& operator*=(const double);              // �������������� ��������� �� ����� ������
        friend Point3D operator*(const double, const Point3D&); // ��������� �� ����� �����
        Point3D operator/(const double) const;          // �������
        Point3D& operator/=(const double);              // �������������� �������
        bool operator==(const Point3D&) const;          // �������� �� ���������
        bool operator!=(const Point3D&) const;          // �������� �� �����������

        QVector3D toQVector3D() const;          // ���������� ����� � ����� QVector3D
        QVector<float> getVectorCoords() const; // ���������� ���������� ����� � ����� QVector<float>
        QVector<float> getVectorColor() const;  // ���������� ����� ����� ����� � ����� QVector<float>, ����������� � ��������� [0, 1]
        QPointF toQPointF() const;              // ���������� ����� ��� ���������� z
        QColor getColor() const;                // ��������� ���� �����
        double x() const;                       // ���������� ���������� x �����
        double y() const;                       // ���������� ���������� x �����
        double z() const;                       // ���������� ���������� x �����
        void setPoint3D(const Point3D&);        // �������� ��� ����� (� ������� �����)
        void setCoords(const QVector3D);        // �������� ���������� ����� (� ������� �������)
        void setColor(const QColor&);           // �������� ���� �����
        void setX(const double);                // �������� ���������� x �����
        void setY(const double);                // �������� ���������� x �����
        void setZ(const double);                // �������� ���������� x �����

        double distance(const Point3D&) const; // ��������� ���������� �� ����� �� �����
  };

}

#endif // POINTOBJECT_H
