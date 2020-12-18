#include "wrappers/pointObject.h"
#include <cmath>
#include <QtDebug>

namespace RubicsCube {
Point3D::Point3D() : point(0, 0, 0), color() {}

// ������������� ����� ��������� ������������
Point3D::Point3D(const double x, const double y, const double z) : point(x, y, z), color() {}

// ������������� ����� ��������� ������������ � ������
Point3D::Point3D(const double x, const double y, const double z, const QColor& bufColor) : point(x, y, z), color(bufColor) {}

// ������������� ����� ��������
Point3D::Point3D(const QVector3D& buf) : point(buf), color() {}

// ������������� ����� �������� � ������
Point3D::Point3D(const QVector3D& bufPoint, const QColor& bufColor) : point(bufPoint), color(bufColor) {}

// ����������� �����������
Point3D::Point3D(const Point3D& rigth) : point(rigth.point), color(rigth.color) {}

// ����������
Point3D::~Point3D() {}

// ������������ �����
Point3D& Point3D::operator=(const Point3D& right) {
    if (this == &right) // �������� �� ���������������
        return *this;

    point = right.point;
    color = right.color;

    return *this;
}

// ������������ �������
Point3D& Point3D::operator=(const QVector3D& right) { point = right; return *this; }

// ������� ����: ���������� ����� ��� ���������
Point3D Point3D::operator+() const { return *this;}

// ��������: ������������ ��������������� ����������
Point3D Point3D::operator+(const Point3D& right) const {
    return Point3D(point.x() + right.point.x(), point.y() + right.point.y(), point.z() + right.point.z(), color);
}

// �������������� ��������
Point3D& Point3D::operator+=(const Point3D& right) {
    point.setX(point.x() + right.point.x());
    point.setY(point.y() + right.point.y());
    point.setZ(point.z() + right.point.z());
    return *this;
}

// ������� �����: ���������� ����� � ���������������� ������������
Point3D Point3D::operator-() const {
    return Point3D(-point.x(), -point.y(), -point.z(), color);
}

// ���������: ���������� ��������������� ����������
Point3D Point3D::operator-(const Point3D& right) const {
    return Point3D(point.x() - right.point.x(), point.y() - right.point.y(), point.z() - right.point.z(), color);
}

// �������������� ���������
Point3D& Point3D::operator-=(const Point3D& right) {
    point.setX(point.x() - right.point.x());
    point.setY(point.y() - right.point.y());
    point.setZ(point.z() - right.point.z());
    return *this;
}

// ��������� �� ����� ������
Point3D operator*(const Point3D& left, const double right) {
    return Point3D(left.x() * right, left.y() * right, left.z() * right, left.getColor());
}

// �������������� ��������� �� ����� ������
Point3D& Point3D::operator*=(const double right) {
    point.setX(point.x() * right);
    point.setY(point.y() * right);
    point.setZ(point.z() * right);
    return *this;
}

// ��������� �� ����� �����
Point3D operator*(const double left, const Point3D& right) { return right * left; }

// �������
Point3D Point3D::operator/(const double right) const {
    return Point3D(point.x() / right, point.y() / right, point.z() / right, color);
}

// �������������� �������
Point3D& Point3D::operator/=(const double right) {
    point.setX(point.x() / right);
    point.setY(point.y() / right);
    point.setZ(point.z() / right);
    return *this;
}

// �������� �� ���������
bool Point3D::operator==(const Point3D& right) const { return point == right.point; }

// �������� �� �����������
bool Point3D::operator!=(const Point3D& right) const { return point != right.point;}

// ���������� ����� � ����� QVector3D
QVector3D Point3D::toQVector3D() const{return QVector3D(point);}

// ���������� ���������� ����� � ����� QVector<float>
QVector<float> Point3D::getVectorCoords() const {
    QVector<float> res;
    res << point.x() << point.y() << point.z();
    return res;
}

// ���������� ����� ����� ����� � ����� QVector<float>, ����������� � ��������� [0, 1]
QVector<float> Point3D::getVectorColor() const {
    QVector<float> res;
    res << color.redF() << color.greenF() << color.blueF();
    return res;
}

// ���������� ����� ��� ���������� z
QPointF Point3D::toQPointF() const { return QPointF(point.x(), point.y()); }

// ��������� ���� �����
QColor Point3D::getColor() const { return color; }

// ���������� ���������� x �����
double Point3D::x() const { return point.x(); }

// ���������� ���������� x �����
double Point3D::y() const { return point.y(); }

// ���������� ���������� x �����
double Point3D::z() const { return point.z(); }

// �������� ��� ����� (� ������� �����)
void Point3D::setPoint3D(const Point3D& buf) {
    point.setX(buf.x());
    point.setY(buf.y());
    point.setZ(buf.z());
}

// �������� ��� ����� (� ������� �������)
void Point3D::setCoords(const QVector3D buf) {
    point.setX(buf.x());
    point.setY(buf.y());
    point.setZ(buf.z());
}

// �������� ���� �����
void Point3D::setColor(const QColor& bufColor) { color = bufColor; }

// �������� ���������� x �����
void Point3D::setX(const double x) { point.setX(x); }

// �������� ���������� x �����
void Point3D::setY(const double y) { point.setY(y); }

// �������� ���������� x �����
void Point3D::setZ(const double z) { point.setZ(z); }

// ��������� ���������� �� ����� �� �����
double Point3D::distance(const Point3D& buf) const
  {
    double dx = (point.x() - buf.x()) * (point.x() - buf.x());
    double dy = (point.y() - buf.y()) * (point.y() - buf.y());
    double dz = (point.z() - buf.z()) * (point.z() - buf.z());
    return sqrt(dx + dy + dz);
  }

}
