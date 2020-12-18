#ifndef POINTOBJECT_H
#define POINTOBJECT_H
#include <QVector3D>
#include <QVector>
#include <QPointF>
#include <QColor>

namespace RubicsCube {

class Point3D
  {
    QVector3D point;    // Точка в 3-мерном пространстве
    QColor color;       // Цвет точки в формате RGB

    public:
        Point3D(); // Конструктор по умолчанию: все координаты равны 0
        Point3D(const double, const double, const double); // Инициализация точки числовыми координатами
        Point3D(const double, const double, const double, const QColor&); // Инициализация точки числовыми координатами и цветом
        Point3D(const QVector3D&);                  // Инициализация точки вектором
        Point3D(const QVector3D&, const QColor&);   // Инициализация точки вектором и цветом
        Point3D(const Point3D&);                    // Конструктор копирования
        ~Point3D();                                 // Деструктор

        Point3D& operator=(const Point3D&);             // Присваивание точки
        Point3D& operator=(const QVector3D&);           // Присваивание вектора
        Point3D operator+() const;                      // Унарный плюс: возвращает точку без изменений
        Point3D operator+(const Point3D&) const;        // Сложение: складываются соответствующие координаты
        Point3D& operator+=(const Point3D&);            // Корректирующее сложение
        Point3D operator-() const;                      // Унарный минус: возвращает точку с противоположными координатами
        Point3D operator-(const Point3D&) const;        // Вычитание: вычитаются соответствующие координаты
        Point3D& operator-=(const Point3D&);            // Корректирующее вычитание
        friend Point3D operator*(const Point3D&, const double); // Умножение на число справа
        Point3D& operator*=(const double);              // Корректирующее умножение на число справа
        friend Point3D operator*(const double, const Point3D&); // Умножение на число слева
        Point3D operator/(const double) const;          // Деление
        Point3D& operator/=(const double);              // Корректирующее деление
        bool operator==(const Point3D&) const;          // Проверка на равенство
        bool operator!=(const Point3D&) const;          // Проверка на неравенство

        QVector3D toQVector3D() const;          // Возвращает точку в форме QVector3D
        QVector<float> getVectorCoords() const; // Возвращает координаты точки в форме QVector<float>
        QVector<float> getVectorColor() const;  // Возвращает байты цвета точки в форме QVector<float>, приведенные к диапазону [0, 1]
        QPointF toQPointF() const;              // Возвращает точку без координаты z
        QColor getColor() const;                // Возвращет цвет точки
        double x() const;                       // Возвращает координату x точки
        double y() const;                       // Возвращает координату x точки
        double z() const;                       // Возвращает координату x точки
        void setPoint3D(const Point3D&);        // Изменяет всю точку (с помощью точки)
        void setCoords(const QVector3D);        // Изменяет координаты точки (с помощью вектора)
        void setColor(const QColor&);           // Изменяет цвет точки
        void setX(const double);                // Изменяет координату x точки
        void setY(const double);                // Изменяет координату x точки
        void setZ(const double);                // Изменяет координату x точки

        double distance(const Point3D&) const; // Вычисляет расстояние от точки до точки
  };

}

#endif // POINTOBJECT_H
