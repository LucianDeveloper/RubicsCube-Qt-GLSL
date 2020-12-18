#ifndef EDGEOBJECT_H
#define EDGEOBJECT_H
#include "wrappers/pointObject.h"

namespace RubicsCube {

class EdgeObject
  {
    QVector<Point3D> edge;
    public:
        EdgeObject();                           // Конструктор по умолчанию: создает пустую грань
        EdgeObject(const QVector<Point3D>&);    // Конструктор с инициализацией точек грани
        EdgeObject(const EdgeObject&);          // Конструктор копирования
        ~EdgeObject();                          // Деструктор

        EdgeObject& operator=(const EdgeObject&);   // Присваивание
        Point3D& operator[](const int);             // Индексация
        const Point3D& operator[](const int) const; // Индексация (константная)
        EdgeObject operator+(const Point3D&) const; // Смещение грани
        bool operator==(const EdgeObject&) const;   // Проверка на равенство
        bool operator!=(const EdgeObject&) const;   // Проверка на неравенство
        EdgeObject& operator<<(const Point3D&);     // Добавление точки в грань

        QVector<float> getVectorPointsCoords() const;   // Возвращает координаты точек грани в форме QVector<float>
        QVector<float> getVectorPointsColor() const;    // Возвращает байты цвета точек грани в форме QVector<float>, приведенные к диапазону [0, 1]
        QVector<float> getVectorPointsNormal() const;   // Возвращает нормали точек грани в форме QVector<float>
        void setEdge(const EdgeObject&);                // Изменяет грань (с помощью другой грани)
        void setColor(const QColor&);                   // Изменяет цвет грани

        void addPoint3D(const Point3D&);    // Добавляет точку в грань
        Point3D getCenterEdge() const;      // Возвращает точку центра тяжести грани
        QVector3D getNormalVector() const;  // Возвращает вектор нормали грани
        QPair<bool, Point3D> checkIntersectVector(const QPair<Point3D, Point3D>&) const; // Возвращает (true, точка пересечения), если грань пересекается с вектором, заданным концами, иначе (false, (0, 0, 0))
        int size() const;   // Возвращает кол-во точек грани
        void clear();       // Очищает грань от точек
  };

}

#endif // EDGEOBJECT_H
