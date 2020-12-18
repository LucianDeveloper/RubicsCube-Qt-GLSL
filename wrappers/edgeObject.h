#ifndef EDGEOBJECT_H
#define EDGEOBJECT_H
#include "wrappers/pointObject.h"

namespace RubicsCube {

class EdgeObject
  {
    QVector<Point3D> edge;
    public:
        EdgeObject();                           // ����������� �� ���������: ������� ������ �����
        EdgeObject(const QVector<Point3D>&);    // ����������� � �������������� ����� �����
        EdgeObject(const EdgeObject&);          // ����������� �����������
        ~EdgeObject();                          // ����������

        EdgeObject& operator=(const EdgeObject&);   // ������������
        Point3D& operator[](const int);             // ����������
        const Point3D& operator[](const int) const; // ���������� (�����������)
        EdgeObject operator+(const Point3D&) const; // �������� �����
        bool operator==(const EdgeObject&) const;   // �������� �� ���������
        bool operator!=(const EdgeObject&) const;   // �������� �� �����������
        EdgeObject& operator<<(const Point3D&);     // ���������� ����� � �����

        QVector<float> getVectorPointsCoords() const;   // ���������� ���������� ����� ����� � ����� QVector<float>
        QVector<float> getVectorPointsColor() const;    // ���������� ����� ����� ����� ����� � ����� QVector<float>, ����������� � ��������� [0, 1]
        QVector<float> getVectorPointsNormal() const;   // ���������� ������� ����� ����� � ����� QVector<float>
        void setEdge(const EdgeObject&);                // �������� ����� (� ������� ������ �����)
        void setColor(const QColor&);                   // �������� ���� �����

        void addPoint3D(const Point3D&);    // ��������� ����� � �����
        Point3D getCenterEdge() const;      // ���������� ����� ������ ������� �����
        QVector3D getNormalVector() const;  // ���������� ������ ������� �����
        QPair<bool, Point3D> checkIntersectVector(const QPair<Point3D, Point3D>&) const; // ���������� (true, ����� �����������), ���� ����� ������������ � ��������, �������� �������, ����� (false, (0, 0, 0))
        int size() const;   // ���������� ���-�� ����� �����
        void clear();       // ������� ����� �� �����
  };

}

#endif // EDGEOBJECT_H
