#ifndef GRAPHICOBJECTS_H
#define GRAPHICOBJECTS_H
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include <gl/glu.h>
#include "wrappers/edgeObject.h"
#include "wrappers/cameraObject.h"
#include "lighting.h"

namespace RubicsCube {

const int ID_ERROR = -1;            // ������������ ID �������
const float BIAS_TRANSLATE = 0.05;  // ��� �������� ��� ������� ������
const float KSCALE_MIN = 0.1;       // ����������� ������� �������
const float KSCALE_MAX = 10;        // ������������ ������� �������

typedef struct
  {
    float dx = 0;
    float dy = 0;
    float dz = 0;
  } t_bias;

typedef struct
  {
    QMatrix4x4 projectMatrix;
    QMatrix4x4 modelViewMatrix;
    QMatrix4x4 rotateMatrix;
  } t_view;

typedef struct
  {
    QVector3D Ka;       // ����������� ��������� �������� �����
    QVector3D Kd;       // ����������� ��������� ����������� �����
    QVector3D Ks;       // ����������� ��������� ����������� �����
    float shininess;    // ���������� ������� ����������� ���������
  } t_materialInfo;

class ShaderObject : public QObject
  {
    Q_OBJECT

        friend class SceneObject;

    protected:

        int id;                     // ������������� �������
        GLenum mode;                // ������ ����������� �������� ������
        t_materialInfo material;    // �������� �������
        bool active;                // ���������� �� ������ �� ������ ��� ��� (true - ����������, false - ���)
        t_view view;                // ������� ��������������, ���� � �������� �������
        QVector<EdgeObject> object; // 3-������ ������ ��� ����� 3-������ ������
        QOpenGLShaderProgram shpr;  // ��������� ��������� �������
        t_bias bias;                // �������� ������� �� ���� Ox, Oy, Oz
        t_bias initBias;            // �������� �������� ������� �������
        float kScale;               // �������
        QPoint mousePosOld;         // ���������� ������� ����
        bool isSuperClass;          // ����� �������� ������ ��������� �������
        bool isAnimated;
        bool isMaySelect;
        QVector<ShaderObject*> submodels; // ���� isSuperClass
    public:

        ShaderObject();             // ����������� �� ���������: ������� ������ ������
        ShaderObject(const int);    // ����������� � �������������� ID �������
        ShaderObject(const GLenum, const QVector<EdgeObject>&); // ����������� � �������������� ������ �������
        ShaderObject(const ShaderObject&); // ����������� �����������
        ~ShaderObject();            // ����������

        ShaderObject& operator=(const ShaderObject&);   // ������������
        EdgeObject& operator[](const int);              // ����������
        const EdgeObject& operator[](const int) const;  // ���������� (�����������)
        bool operator==(const ShaderObject&) const;     // �������� �� ���������
        bool operator!=(const ShaderObject&) const;     // �������� �� �����������
        ShaderObject& operator<<(const EdgeObject&);    // ���������� ����� � ������
        ShaderObject& operator<<(const QVector<EdgeObject>&); // ���������� ������ � ������

        QVector<float> getVectorEdgeCoords() const;     // ���������� ���������� ����� ������ ������� � ����� QVector<float>
        QVector<float> getVectorEdgeColor() const;      // ���������� ����� ����� ����� ������ ������� � ����� QVector<float>, ����������� � ��������� [0, 1]
        QVector<float> getVectorEdgeNormal() const;     // ���������� ������� ����� ������ ������� � ����� QVector<float>
        GLenum getMode() const;                         // ���������� ��������, �������� �������� ������
        int getID() const;                              // ���������� ������������� �������
        t_materialInfo getMaterial() const;             // ���������� �������� �������
        t_bias getBias() const;                         // ���������� �������� �������
        t_bias getInitBias() const;                     // ���������� �������� �������� �������
        void setInitBias(const QVector3D&);             // ������ �������� �������� �����
        bool check() const;                             // ���������� true, ���� ����������� ������� ��������, ����� ���������� false
        void setObject(const ShaderObject&);            // �������� ������ (� ������� ������� �������)
        void setMode(GLenum);                           // ������������� ����� ��������, �������� �������� ������
        void setID(const int);                          // ������������� ����� ������������� �������
        void on();                                      // �������� ����������� �������
        void off();                                     // ��������� ����������� �������

        virtual void initShader() = 0;      // ������������� ��������� ���������
        virtual int bind(const CameraObject&, const QVector<LightObject>&) = 0; // ���������� ��������� ��������� � �����������. ���������� ���-�� ������������ ����������
        virtual void release() = 0;         // ������������ ��������� �� ��������� ���������
        virtual void resetProjection(const int, const int, const float) = 0; // ���������� ������� �������������� ��� ����� �������� ������
        virtual void resetModelView() = 0;  // ���������� ������� �������
        void addEdge3D(const EdgeObject&);  // ��������� ����� � ������
        Point3D getCentreObject() const;    // ���������� ����� �������
        float depthPoint(const Point3D&, const CameraObject&) const; // ���������� ������� ����� � ��������������� �����������
        int size() const;                   // ���������� ���-�� ������ � �������

    signals:

        void repaintScene(); // ������, ��� ������ ��������� � ����� ����� ������������

    public slots:

        virtual void mouseRotate(const int, QMouseEvent*);  // ������� ������� �����
        virtual void mouseClick(const int, QMouseEvent*);   // ���� �� ������� �����
        virtual void mouseWheel(const int, QWheelEvent*);   // ������ ���������� ������� ����
        virtual void keyPress(const int, QKeyEvent*, const QVector<ShaderObject*>&); // ����� ������ ��� �������, ������ �������
  };

}

#endif // GRAPHICOBJECTS_H
