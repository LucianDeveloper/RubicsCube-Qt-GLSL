#include "wrappers/pointObject.h"
#include <cmath>
#include <QtDebug>

namespace RubicsCube {
Point3D::Point3D() : point(0, 0, 0), color() {}

// Инициализация точки числовыми координатами
Point3D::Point3D(const double x, const double y, const double z) : point(x, y, z), color() {}

// Инициализация точки числовыми координатами и цветом
Point3D::Point3D(const double x, const double y, const double z, const QColor& bufColor) : point(x, y, z), color(bufColor) {}

// Инициализация точки вектором
Point3D::Point3D(const QVector3D& buf) : point(buf), color() {}

// Инициализация точки вектором и цветом
Point3D::Point3D(const QVector3D& bufPoint, const QColor& bufColor) : point(bufPoint), color(bufColor) {}

// Конструктор копирования
Point3D::Point3D(const Point3D& rigth) : point(rigth.point), color(rigth.color) {}

// Деструктор
Point3D::~Point3D() {}

// Присваивание точки
Point3D& Point3D::operator=(const Point3D& right) {
    if (this == &right) // Проверка на самокопирование
        return *this;

    point = right.point;
    color = right.color;

    return *this;
}

// Присваивание вектора
Point3D& Point3D::operator=(const QVector3D& right) { point = right; return *this; }

// Унарный плюс: возвращает точку без изменений
Point3D Point3D::operator+() const { return *this;}

// Сложение: складываются соответствующие координаты
Point3D Point3D::operator+(const Point3D& right) const {
    return Point3D(point.x() + right.point.x(), point.y() + right.point.y(), point.z() + right.point.z(), color);
}

// Корректирующее сложение
Point3D& Point3D::operator+=(const Point3D& right) {
    point.setX(point.x() + right.point.x());
    point.setY(point.y() + right.point.y());
    point.setZ(point.z() + right.point.z());
    return *this;
}

// Унарный минус: возвращает точку с противоположными координатами
Point3D Point3D::operator-() const {
    return Point3D(-point.x(), -point.y(), -point.z(), color);
}

// Вычитание: вычитаются соответствующие координаты
Point3D Point3D::operator-(const Point3D& right) const {
    return Point3D(point.x() - right.point.x(), point.y() - right.point.y(), point.z() - right.point.z(), color);
}

// Корректирующее вычитание
Point3D& Point3D::operator-=(const Point3D& right) {
    point.setX(point.x() - right.point.x());
    point.setY(point.y() - right.point.y());
    point.setZ(point.z() - right.point.z());
    return *this;
}

// Умножение на число справа
Point3D operator*(const Point3D& left, const double right) {
    return Point3D(left.x() * right, left.y() * right, left.z() * right, left.getColor());
}

// Корректирующее умножение на число справа
Point3D& Point3D::operator*=(const double right) {
    point.setX(point.x() * right);
    point.setY(point.y() * right);
    point.setZ(point.z() * right);
    return *this;
}

// Умножение на число слева
Point3D operator*(const double left, const Point3D& right) { return right * left; }

// Деление
Point3D Point3D::operator/(const double right) const {
    return Point3D(point.x() / right, point.y() / right, point.z() / right, color);
}

// Корректирующее деление
Point3D& Point3D::operator/=(const double right) {
    point.setX(point.x() / right);
    point.setY(point.y() / right);
    point.setZ(point.z() / right);
    return *this;
}

// Проверка на равенство
bool Point3D::operator==(const Point3D& right) const { return point == right.point; }

// Проверка на неравенство
bool Point3D::operator!=(const Point3D& right) const { return point != right.point;}

// Возвращает точку в форме QVector3D
QVector3D Point3D::toQVector3D() const{return QVector3D(point);}

// Возвращает координаты точки в форме QVector<float>
QVector<float> Point3D::getVectorCoords() const {
    QVector<float> res;
    res << point.x() << point.y() << point.z();
    return res;
}

// Возвращает байты цвета точки в форме QVector<float>, приведенные к диапазону [0, 1]
QVector<float> Point3D::getVectorColor() const {
    QVector<float> res;
    res << color.redF() << color.greenF() << color.blueF();
    return res;
}

// Возвращает точку без координаты z
QPointF Point3D::toQPointF() const { return QPointF(point.x(), point.y()); }

// Возвращет цвет точки
QColor Point3D::getColor() const { return color; }

// Возвращает координату x точки
double Point3D::x() const { return point.x(); }

// Возвращает координату x точки
double Point3D::y() const { return point.y(); }

// Возвращает координату x точки
double Point3D::z() const { return point.z(); }

// Изменяет всю точку (с помощью точки)
void Point3D::setPoint3D(const Point3D& buf) {
    point.setX(buf.x());
    point.setY(buf.y());
    point.setZ(buf.z());
}

// Изменяет всю точку (с помощью вектора)
void Point3D::setCoords(const QVector3D buf) {
    point.setX(buf.x());
    point.setY(buf.y());
    point.setZ(buf.z());
}

// Изменяет цвет точки
void Point3D::setColor(const QColor& bufColor) { color = bufColor; }

// Изменяет координату x точки
void Point3D::setX(const double x) { point.setX(x); }

// Изменяет координату x точки
void Point3D::setY(const double y) { point.setY(y); }

// Изменяет координату x точки
void Point3D::setZ(const double z) { point.setZ(z); }

// Вычисляет расстояние от точки до точки
double Point3D::distance(const Point3D& buf) const
  {
    double dx = (point.x() - buf.x()) * (point.x() - buf.x());
    double dy = (point.y() - buf.y()) * (point.y() - buf.y());
    double dz = (point.z() - buf.z()) * (point.z() - buf.z());
    return sqrt(dx + dy + dz);
  }

}
