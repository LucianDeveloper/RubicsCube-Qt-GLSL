#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QOpenGLWidget>
#include "wrappers/shaderObject.h"
#include "wrappers/cameraObject.h"

namespace RubicsCube {

class SceneObject : public QOpenGLWidget {
    Q_OBJECT

    CameraObject* camera;           // ������ �����
    QVector<ShaderObject*> objs;    // ������� �����
    QVector<LightObject> lights;    // ��������� ����� �� �����
    int idActiveObject;             // ��������� ������ �����

    public:
        SceneObject(); // ����������� �� ���������
        SceneObject(CameraObject*, const QVector<ShaderObject*>&); // ����������� � �������������� ������ � �������� �����
        ~SceneObject(); // ����������

        SceneObject& operator<<(ShaderObject*);                 // ���������� ������� � �����
        SceneObject& operator<<(const QVector<ShaderObject*>&); // ���������� �������� � �����
        SceneObject& operator<<(const LightObject&);            // ���������� ��������� ����� � �����
        SceneObject& operator<<(const QVector<LightObject>&);   // ���������� ���������� ����� � �����

        void setCamera(CameraObject*); // ������������� ����� ������ �� �����

        void drawObject(ShaderObject*);         // ��������� ������� �� ����
        QPointF pointToOpenGLScreen(const QPoint&) const; // ���������� ����� � �������� ������� ���������
        QPair<Point3D, Point3D> selectionRay(const ShaderObject&, const QPoint&) const; // ���������� ����� ������������ ����, ����������� �� ������� �� ������ � ������ ����������� �������
        int selectObject(const QPoint&) const;  // ���������� ������������� �������, ������� ������� �� ������, ��� -1, ���� ������ �� �������
        Point3D getCentreScene() const;         // ���������� ����� �����

    protected:
        void initializeGL() override;       // ������������� ���������
        void resizeGL(int, int) override;   // ��������� �������� ����
        void paintGL() override;            // ��������� ����
        void clear();                       // ������� ����� �� ��������

    private:
        void mouseMoveEvent(QMouseEvent*) override;     // �������� ������� ������� ����
        void mousePressEvent(QMouseEvent*) override;    // ������� �� ������ ����
        void wheelEvent(QWheelEvent*) override;         // ���������� �������� �����
        void keyPressEvent(QKeyEvent *event) override;  // ������ ������� �� ����������
        void timerEvent(QTimerEvent *event) override;   // ������, ��� �������������

    signals:
        void mouseMoveObject(int, QMouseEvent*);        // ������, ��� ������ ������� �����
        void mouseClickObject(int, QMouseEvent*);       // ������, ��� �� ������ �������� �����
        void mouseWheelObject(int, QWheelEvent*);       // ������ � ���, ��� ������ ���������� ������� ����
        void keyPressObject(int, QKeyEvent*, const QVector<ShaderObject*>&); // ������ � ���, ��� ���� ������ ������� ���������� ��� ��������� �������

        void mouseRightButtonMoveCamera(QMouseEvent*);  // ������, ��� ������ ������� ������ ������� ����
        void mouseLeftButtonMoveCamera(QMouseEvent*);   // ������, ��� ������ ������� ����� ������� ����
        void mouseClickCamera(QMouseEvent*);            // ������, ��� �� ����� �������� �����, �� �� �� ������
        void mouseWheelCamera(QWheelEvent*);            // ������ � ���, ��� ������ ���������� ������� ����
        void keyPressCamera(QKeyEvent*);                // ������ � ���, ��� ���� ������ ������� ���������� ��� ����������� �������

    public slots:
        void repaintScene3D();          // ����������� �����
        void resetProjectionScene3D();  // ��������������� �������� �������� �����
  };

}

#endif // SCENEOBJECT_H
