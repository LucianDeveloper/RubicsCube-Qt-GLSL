#include "wrappers/edgeObject.h"
#include <QDebug>

namespace RubicsCube {

EdgeObject::EdgeObject() : edge() {}

// ����������� � �������������� ����� �����
EdgeObject::EdgeObject(const QVector<Point3D>& buf) : edge(buf) {}

// ����������� �����������
EdgeObject::EdgeObject(const EdgeObject& buf) : edge(buf.edge) {}

// ����������
EdgeObject::~EdgeObject() {}

// ������������
EdgeObject& EdgeObject::operator=(const EdgeObject& right) {
    if (this == &right) return *this;
    edge = right.edge;
    return *this;
}

// ����������
Point3D& EdgeObject::operator[](const int i) {
    if (i >= size()) { qDebug() << "Error: out of range in Edge3D"; exit(EXIT_FAILURE); }
    return edge[i];
}

// ���������� (�����������)
const Point3D& EdgeObject::operator[](const int i) const {
    if (i >= size()) { qDebug() << "Error: out of range in Edge3D"; exit(EXIT_FAILURE); }
    return edge[i];
}

// �������� �����
EdgeObject EdgeObject::operator+(const Point3D& buf) const {
    EdgeObject res; for (auto i : edge) res << i + buf;
    return res;
}

// �������� �� ���������
bool EdgeObject::operator==(const EdgeObject& right) const { return edge == right.edge; }

// �������� �� �����������
bool EdgeObject::operator!=(const EdgeObject& right) const { return edge != right.edge; }

// ���������� ����� � �����

EdgeObject& EdgeObject::operator<<(const Point3D& buf) { edge << buf; return *this; }

// ���������� ���������� ����� ����� � ����� QVector<float>
QVector<float> EdgeObject::getVectorPointsCoords() const {
    QVector<float> res;
    for (auto i : edge) res += i.getVectorCoords();
    return res;
}

// ���������� ����� ����� ����� ����� � ����� QVector<float>, ����������� � ��������� [0, 1]
QVector<float> EdgeObject::getVectorPointsColor() const {
    QVector<float> res;
    for (auto i : edge) res += i.getVectorColor();
    return res;
}

// ���������� ������� ����� ����� � ����� QVector<float>
QVector<float> EdgeObject::getVectorPointsNormal() const {
    QVector<float> res;
    QVector3D normal = getNormalVector().normalized();
    for (int i = 0; i < size(); i++) res << normal.x() << normal.y() << normal.z();
    return res;
}

// �������� ����� (� ������� ������ �����)
void EdgeObject::setEdge(const EdgeObject& buf) { edge = buf.edge; }

// �������� ���� �����
void EdgeObject::setColor(const QColor& bufColor) { for (int i = 0; i < size(); i++) edge[i].setColor(bufColor); }

// ��������� ����� � �����
void EdgeObject::addPoint3D(const Point3D& buf) { edge << buf; }

// ���������� ����� ������ ������� �����
Point3D EdgeObject::getCenterEdge() const {
    Point3D res(0, 0, 0);
    for (auto i : edge) res += i;
    return res / edge.size();
}

// ���������� ������ ������� �����
QVector3D EdgeObject::getNormalVector() const {
    // x = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1)
    float x = (edge[1].y() - edge[0].y()) * (edge[2].z() - edge[0].z()) - (edge[1].z() - edge[0].z()) * (edge[2].y() - edge[0].y());
    // y = (z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1)
    float y = (edge[1].z() - edge[0].z()) * (edge[2].x() - edge[0].x()) - (edge[1].x() - edge[0].x()) * (edge[2].z() - edge[0].z());
    // z = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1)
    float z = (edge[1].x() - edge[0].x()) * (edge[2].y() - edge[0].y()) - (edge[1].y() - edge[0].y()) * (edge[2].x() - edge[0].x());
    return QVector3D(x, y, z);
}

// ���������� (true, ����� �����������), ���� ����� ������������ � ��������, �������� �������, ����� (false, (0, 0, 0))
QPair<bool, Point3D> EdgeObject::checkIntersectVector(const QPair<Point3D, Point3D>& vec) const {
    // �������� ���� �� ������ https://gamedev.ru/articles/?id=20101

    QVector3D normal = getNormalVector();
    // ��������� ���������� ����� ������� ������� � ���������� �����
    float r1 = QVector3D::dotProduct(normal, (vec.first - edge[0]).toQVector3D());
    float r2 = QVector3D::dotProduct(normal, (vec.second - edge[0]).toQVector3D());
    // ���� ��� ����� ������� ����� �� ���� ������� �� ���������, �� ������� �� ���������� �����
    if (((r1 > 0) - (r1 < 0)) == ((r2 > 0) - (r1 < 0)))
        return QPair<bool, Point3D>(false, Point3D(0, 0, 0));

    // ��������� ����� ����������� ������� � �����
    Point3D res = vec.first + (vec.second - vec.first) * (-r1 / (r2 - r1));
    // ���������, ��������� �� ����� ����������� ������ �����
    for (int i = 0; i < edge.size(); i++) {
        if (QVector3D::dotProduct(QVector3D::crossProduct((edge[(i+1) % edge.size()] - edge[i]).toQVector3D(), (res - edge[i]).toQVector3D()), normal) <= 0)
            return QPair<bool, Point3D>(false, Point3D(0, 0, 0));
    }
    return QPair<bool, Point3D>(true, res);
}

// ���������� ���-�� ����� �����
int EdgeObject::size() const { return edge.size(); }

// ������� ����� �� �����
void EdgeObject::clear() { edge.clear(); }

}
