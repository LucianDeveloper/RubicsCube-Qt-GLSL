#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QOpenGLWidget>
#include "wrappers/shaderObject.h"
#include "wrappers/cameraObject.h"

namespace RubicsCube {

class SceneObject : public QOpenGLWidget {
    Q_OBJECT

    CameraObject* camera;           // Камера сцены
    QVector<ShaderObject*> objs;    // Объекты сцены
    QVector<LightObject> lights;    // Источники света на сцене
    int idActiveObject;             // Выбранный объект сцены

    public:
        SceneObject(); // Конструктор по умолчанию
        SceneObject(CameraObject*, const QVector<ShaderObject*>&); // Конструктор с инициализацией камеры и объектов сцены
        ~SceneObject(); // Деструктор

        SceneObject& operator<<(ShaderObject*);                 // Добавление объекта в сцену
        SceneObject& operator<<(const QVector<ShaderObject*>&); // Добавление объектов в сцену
        SceneObject& operator<<(const LightObject&);            // Добавление источника света в сцену
        SceneObject& operator<<(const QVector<LightObject>&);   // Добавление источников света в сцену

        void setCamera(CameraObject*); // Устанавливает новую камеру на сцене

        void drawObject(ShaderObject*);         // Отрисовка объекта на окне
        QPointF pointToOpenGLScreen(const QPoint&) const; // Возвращает точку в экранной системе координат
        QPair<Point3D, Point3D> selectionRay(const ShaderObject&, const QPoint&) const; // Возвращает концы селективного луча, полученного из позиции на экране и матриц отображения объекта
        int selectObject(const QPoint&) const;  // Возвращает идентификатор объекта, который выбрали на экране, или -1, если ничего не выбрано
        Point3D getCentreScene() const;         // Возвращает центр сцены

    protected:
        void initializeGL() override;       // Инициализация контекста
        void resizeGL(int, int) override;   // Изменение размеров окна
        void paintGL() override;            // Отрисовка окна
        void clear();                       // Очистка сцены от объектов

    private:
        void mouseMoveEvent(QMouseEvent*) override;     // Движение зажатой кнопкой мыши
        void mousePressEvent(QMouseEvent*) override;    // Нажатие на кнопку мыши
        void wheelEvent(QWheelEvent*) override;         // Прокручено колесико мышки
        void keyPressEvent(QKeyEvent *event) override;  // Нажата клавиша на клавиатуре
        void timerEvent(QTimerEvent *event) override;   // Таймер, для переотрисовки

    signals:
        void mouseMoveObject(int, QMouseEvent*);        // Сигнал, что объект двигали мышью
        void mouseClickObject(int, QMouseEvent*);       // Сигнал, что на объект кликнули мышью
        void mouseWheelObject(int, QWheelEvent*);       // Сигнал о том, что объект прокрутили колесом мыши
        void keyPressObject(int, QKeyEvent*, const QVector<ShaderObject*>&); // Сигнал о том, что была нажата клавиша клавиатуры при выбранном объекте

        void mouseRightButtonMoveCamera(QMouseEvent*);  // Сигнал, что камеру двигали правой кнопкой мыши
        void mouseLeftButtonMoveCamera(QMouseEvent*);   // Сигнал, что камеру двигали левой кнопкой мыши
        void mouseClickCamera(QMouseEvent*);            // Сигнал, что на экран кликнули мышью, но не на объект
        void mouseWheelCamera(QWheelEvent*);            // Сигнал о том, что камеру прокрутили колесом мыши
        void keyPressCamera(QKeyEvent*);                // Сигнал о том, что была нажата клавиша клавиатуры при невыбранном объекте

    public slots:
        void repaintScene3D();          // Перерисовка сцены
        void resetProjectionScene3D();  // Переопределение проекции объектов сцены
  };

}

#endif // SCENEOBJECT_H
