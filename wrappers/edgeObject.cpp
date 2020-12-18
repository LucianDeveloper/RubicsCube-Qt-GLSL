#include "wrappers/edgeObject.h"
#include <QDebug>

namespace RubicsCube {

EdgeObject::EdgeObject() : edge() {}

// Конструктор с инициализацией точек грани
EdgeObject::EdgeObject(const QVector<Point3D>& buf) : edge(buf) {}

// Конструктор копирования
EdgeObject::EdgeObject(const EdgeObject& buf) : edge(buf.edge) {}

// Деструктор
EdgeObject::~EdgeObject() {}

// Присваивание
EdgeObject& EdgeObject::operator=(const EdgeObject& right) {
    if (this == &right) return *this;
    edge = right.edge;
    return *this;
}

// Индексация
Point3D& EdgeObject::operator[](const int i) {
    if (i >= size()) { qDebug() << "Error: out of range in Edge3D"; exit(EXIT_FAILURE); }
    return edge[i];
}

// Индексация (константная)
const Point3D& EdgeObject::operator[](const int i) const {
    if (i >= size()) { qDebug() << "Error: out of range in Edge3D"; exit(EXIT_FAILURE); }
    return edge[i];
}

// Смещение грани
EdgeObject EdgeObject::operator+(const Point3D& buf) const {
    EdgeObject res; for (auto i : edge) res << i + buf;
    return res;
}

// Проверка на равенство
bool EdgeObject::operator==(const EdgeObject& right) const { return edge == right.edge; }

// Проверка на неравенство
bool EdgeObject::operator!=(const EdgeObject& right) const { return edge != right.edge; }

// Добавление точки в грань

EdgeObject& EdgeObject::operator<<(const Point3D& buf) { edge << buf; return *this; }

// Возвращает координаты точек грани в форме QVector<float>
QVector<float> EdgeObject::getVectorPointsCoords() const {
    QVector<float> res;
    for (auto i : edge) res += i.getVectorCoords();
    return res;
}

// Возвращает байты цвета точек грани в форме QVector<float>, приведенные к диапазону [0, 1]
QVector<float> EdgeObject::getVectorPointsColor() const {
    QVector<float> res;
    for (auto i : edge) res += i.getVectorColor();
    return res;
}

// Возвращает нормали точек грани в форме QVector<float>
QVector<float> EdgeObject::getVectorPointsNormal() const {
    QVector<float> res;
    QVector3D normal = getNormalVector().normalized();
    for (int i = 0; i < size(); i++) res << normal.x() << normal.y() << normal.z();
    return res;
}

// Изменяет грань (с помощью другой грани)
void EdgeObject::setEdge(const EdgeObject& buf) { edge = buf.edge; }

// Изменяет цвет грани
void EdgeObject::setColor(const QColor& bufColor) { for (int i = 0; i < size(); i++) edge[i].setColor(bufColor); }

// Добавляет точку в грань
void EdgeObject::addPoint3D(const Point3D& buf) { edge << buf; }

// Возвращает точку центра тяжести грани
Point3D EdgeObject::getCenterEdge() const {
    Point3D res(0, 0, 0);
    for (auto i : edge) res += i;
    return res / edge.size();
}

// Возвращает вектор нормали грани
QVector3D EdgeObject::getNormalVector() const {
    // x = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1)
    float x = (edge[1].y() - edge[0].y()) * (edge[2].z() - edge[0].z()) - (edge[1].z() - edge[0].z()) * (edge[2].y() - edge[0].y());
    // y = (z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1)
    float y = (edge[1].z() - edge[0].z()) * (edge[2].x() - edge[0].x()) - (edge[1].x() - edge[0].x()) * (edge[2].z() - edge[0].z());
    // z = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1)
    float z = (edge[1].x() - edge[0].x()) * (edge[2].y() - edge[0].y()) - (edge[1].y() - edge[0].y()) * (edge[2].x() - edge[0].x());
    return QVector3D(x, y, z);
}

// Возвращает (true, точка пересечения), если грань пересекается с вектором, заданным концами, иначе (false, (0, 0, 0))
QPair<bool, Point3D> EdgeObject::checkIntersectVector(const QPair<Point3D, Point3D>& vec) const {
    // Алгоритм взят по ссылке https://gamedev.ru/articles/?id=20101

    QVector3D normal = getNormalVector();
    // Вычисляем расстояния между концами отрезка и плоскостью грани
    float r1 = QVector3D::dotProduct(normal, (vec.first - edge[0]).toQVector3D());
    float r2 = QVector3D::dotProduct(normal, (vec.second - edge[0]).toQVector3D());
    // Если оба конца отрезка лежат по одну сторону от плоскости, то отрезок не пересекает грань
    if (((r1 > 0) - (r1 < 0)) == ((r2 > 0) - (r1 < 0)))
        return QPair<bool, Point3D>(false, Point3D(0, 0, 0));

    // Вычисляем точку пересечения отрезка с грань
    Point3D res = vec.first + (vec.second - vec.first) * (-r1 / (r2 - r1));
    // Проверяем, находится ли точка пересечения внутри грани
    for (int i = 0; i < edge.size(); i++) {
        if (QVector3D::dotProduct(QVector3D::crossProduct((edge[(i+1) % edge.size()] - edge[i]).toQVector3D(), (res - edge[i]).toQVector3D()), normal) <= 0)
            return QPair<bool, Point3D>(false, Point3D(0, 0, 0));
    }
    return QPair<bool, Point3D>(true, res);
}

// Возвращает кол-во точек грани
int EdgeObject::size() const { return edge.size(); }

// Очищает грань от точек
void EdgeObject::clear() { edge.clear(); }

}
