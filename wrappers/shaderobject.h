#ifndef GRAPHICOBJECTS_H
#define GRAPHICOBJECTS_H
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include <gl/glu.h>
#include "wrappers/edgeObject.h"
#include "wrappers/cameraObject.h"
#include "lighting.h"

namespace RubicsCube {

const int ID_ERROR = -1;            // Неопознанный ID объекта
const float BIAS_TRANSLATE = 0.05;  // Шаг смещения при нажатии клавиш
const float KSCALE_MIN = 0.1;       // Минимальный масштаб объекта
const float KSCALE_MAX = 10;        // Максимальный масштаб объекта

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
    QVector3D Ka;       // Коэффициент отражения фонового света
    QVector3D Kd;       // Коэффициент отражения рассеянного света
    QVector3D Ks;       // Коэффициент отражения отраженного света
    float shininess;    // Показатель степени зеркального отражения
  } t_materialInfo;

class ShaderObject : public QObject
  {
    Q_OBJECT

        friend class SceneObject;

    protected:

        int id;                     // Идентификатор объекта
        GLenum mode;                // Какими примитивами построен объект
        t_materialInfo material;    // Материал объекта
        bool active;                // Отображаем ли объект на экране или нет (true - отображаем, false - нет)
        t_view view;                // Матрицы проектирования, вида и поворота объекта
        QVector<EdgeObject> object; // 3-мерный объект как набор 3-мерных граней
        QOpenGLShaderProgram shpr;  // Шейдерная программа объекта
        t_bias bias;                // Смещение объекта по осям Ox, Oy, Oz
        t_bias initBias;            // Исходное смещение позиции объекта
        float kScale;               // Масштаб
        QPoint mousePosOld;         // Предыдущая позиция мыши
        bool isSuperClass;          // Класс содержит другие шейдерные объекты
        bool isAnimated;
        bool isMaySelect;
        QVector<ShaderObject*> submodels; // Если isSuperClass
    public:

        ShaderObject();             // Конструктор по умолчанию: создает пустой объект
        ShaderObject(const int);    // Конструктор с инициализацией ID объекта
        ShaderObject(const GLenum, const QVector<EdgeObject>&); // Конструктор с инициализацией граней объекта
        ShaderObject(const ShaderObject&); // Конструктор копирования
        ~ShaderObject();            // Деструктор

        ShaderObject& operator=(const ShaderObject&);   // Присваивание
        EdgeObject& operator[](const int);              // Индексация
        const EdgeObject& operator[](const int) const;  // Индексация (константная)
        bool operator==(const ShaderObject&) const;     // Проверка на равенство
        bool operator!=(const ShaderObject&) const;     // Проверка на неравенство
        ShaderObject& operator<<(const EdgeObject&);    // Добавление грани в объект
        ShaderObject& operator<<(const QVector<EdgeObject>&); // Добавление граней в объект

        QVector<float> getVectorEdgeCoords() const;     // Возвращает координаты точек граней объекта в форме QVector<float>
        QVector<float> getVectorEdgeColor() const;      // Возвращает байты цвета точек граней объекта в форме QVector<float>, приведенные к диапазону [0, 1]
        QVector<float> getVectorEdgeNormal() const;     // Возвращает нормали точек граней объекта в форме QVector<float>
        GLenum getMode() const;                         // Возвращает примитив, которыми построен объект
        int getID() const;                              // Возвращает идентификатор объекта
        t_materialInfo getMaterial() const;             // Возвращает материал объекта
        t_bias getBias() const;                         // Возвращает смещение объекта
        t_bias getInitBias() const;                     // Возвращает исходное смещение объекта
        void setInitBias(const QVector3D&);             // Задает исходное смещение ящика
        bool check() const;                             // Возвращает true, если отображение объекта включено, иначе возвращает false
        void setObject(const ShaderObject&);            // Изменяет объект (с помощью другого объекта)
        void setMode(GLenum);                           // Устанавливает новый примитив, которыми построен объект
        void setID(const int);                          // Устанавливает новый идентификатор объекту
        void on();                                      // Включает отображение объекта
        void off();                                     // Выключает отображение объекта

        virtual void initShader() = 0;      // Инициализация шейдерной программы
        virtual int bind(const CameraObject&, const QVector<LightObject>&) = 0; // Связывание шейдерной программы с констекстом. Возвращает кол-во используемых примитивов
        virtual void release() = 0;         // Освобождение контекста от шейдерной программы
        virtual void resetProjection(const int, const int, const float) = 0; // Обновление матрицы проектирования для новых размеров экрана
        virtual void resetModelView() = 0;  // Обновление видовой матрицы
        void addEdge3D(const EdgeObject&);  // Добавляет грань в объект
        Point3D getCentreObject() const;    // Возвращает центр объекта
        float depthPoint(const Point3D&, const CameraObject&) const; // Возвращает глубину точки в преобразованных координатах
        int size() const;                   // Возвращает кол-во граней в объекте

    signals:

        void repaintScene(); // Сигнал, что объект изменился и сцену нужно перерисовать

    public slots:

        virtual void mouseRotate(const int, QMouseEvent*);  // Поворот объекта мышью
        virtual void mouseClick(const int, QMouseEvent*);   // Клик по объекту мышью
        virtual void mouseWheel(const int, QWheelEvent*);   // Объект прокрутили колесом мыши
        virtual void keyPress(const int, QKeyEvent*, const QVector<ShaderObject*>&); // Когда объект был выделен, нажали клавишу
  };

}

#endif // GRAPHICOBJECTS_H
