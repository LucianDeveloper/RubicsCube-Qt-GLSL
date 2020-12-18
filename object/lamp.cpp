#include "lamp.h"

namespace RubicsCube {

Lamp::Lamp(const int idBuf, const QVector3D& bufBias, QVector3D _color, double size) : ShaderObject(){
    id = idBuf;
    mode = GL_QUADS;
    material.Ka = material.Kd = material.Ks = QVector3D(1, 1, 1);
    material.shininess = 256;
    active = true;
    isMaySelect = false;
    isSuperClass = false;
    isAnimated = false;
    QColor color = QColor(_color.x() * 255, _color.y() * 255, _color.z() * 255);
    this->size = size;

    light = LightObject(bufBias);
    light.setLa(_color * 0.04);
    light.setLd(_color * 0.4);
    light.setLs(_color * 0.004);

    setInitBias(bufBias);
    object.clear();
    EdgeObject bufEdge;
    bufEdge << Point3D(-size, -size, -size) << Point3D(-size, -size, size)
            << Point3D(size, -size, size) << Point3D(size, -size, -size);
    bufEdge.setColor(color);
    object << bufEdge;

    bufEdge.clear();
    bufEdge << Point3D(size, -size, size) << Point3D(size, -size, -size)
            << Point3D(size, size, -size) << Point3D(size, size, size);
    bufEdge.setColor(color);
    object << bufEdge;

    bufEdge.clear();
    bufEdge << Point3D(size, size, -size) << Point3D(size, size, size)
            << Point3D(-size, size, size) << Point3D(-size, size, -size);
    bufEdge.setColor(color);
    object << bufEdge;

    bufEdge.clear();
    bufEdge << Point3D(-size, size, size) << Point3D(-size, size, -size)
            << Point3D(-size, -size, -size) << Point3D(-size, -size, size);
    bufEdge.setColor(color);
    object << bufEdge;

    bufEdge.clear();
    bufEdge << Point3D(-size, -size, size) << Point3D(-size, size, size)
            << Point3D(size, size, size) << Point3D(size, -size, size);
    bufEdge.setColor(color);
    object << bufEdge;

    bufEdge.clear();
    bufEdge << Point3D(-size, -size, -size) << Point3D(-size, size, -size)
            << Point3D(size, size, -size) << Point3D(size, -size, -size);
    bufEdge.setColor(color);
    object << bufEdge;

}

Lamp::~Lamp() {}

int Lamp::bind(const CameraObject& camera, const QVector<LightObject>& lights) {
    QVector<float> bufCoords = getVectorEdgeCoords();
    QVector<float> bufColors = getVectorEdgeColor();
    QVector<float> bufNormal = getVectorEdgeNormal();

    shpr.bind();

    // Зададим матрицу, на которую будут умножены однородные координаты вершин в вершинном шейдере
    shpr.setUniformValue("proectionMatrix", view.projectMatrix * camera.getLook() * view.modelViewMatrix);

    // Видовая матрица для вершинного шейдера
    shpr.setUniformValue("modelViewMatrix", view.modelViewMatrix);

    // Нормальная матрица
    shpr.setUniformValue("normalMatrix", (view.modelViewMatrix).normalMatrix());

    // Материал объекта
    shpr.setUniformValue("material.Ka", material.Ka);
    shpr.setUniformValue("material.Kd", material.Kd);
    shpr.setUniformValue("material.Ks", material.Ks);
    shpr.setUniformValue("material.shininess", material.shininess);

    // Позиция камеры
    shpr.setUniformValue("frontCamera", camera.getEye().toQVector3D().normalized());

    // Кол-во источников света
    shpr.setUniformValue("countLights", lights.size());

    // Подгружает в шейдер источники света
    for (auto i = 0; i < lights.size(); i++)
        lights[i].bindLightWithShaderProgram(camera.getLook() * view.modelViewMatrix, shpr, i);

    // Передаём массив вершин (координаты каждой вершины задаются тремя числами)
    shpr.setAttributeArray("posVertex", bufCoords.data(), 3);
    shpr.enableAttributeArray("posVertex");

    // Передаем массив цветов вершин
    shpr.setAttributeArray("colorVertex", bufColors.data(), 3);
    shpr.enableAttributeArray("colorVertex");

    // Передаем массив нормалей вершин
    shpr.setAttributeArray("normalVertex", bufNormal.data(), 3);
    shpr.enableAttributeArray("normalVertex");

    return bufCoords.size() / 3;
}

void Lamp::release() {
    shpr.disableAttributeArray("posVertex");
    shpr.disableAttributeArray("colorVertex");
    shpr.disableAttributeArray("normalVertex");
    shpr.release();
}

void Lamp::initShader() {
    // Текст вершинного шейдера
    shpr.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shaders/vertexLightingShader.vsh");

    // Текст фрагментного шейдера
    shpr.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/shaders/fragmentLightingShader.fsh");

    if (shpr.link() == false) {
        qDebug() << shpr.log();
        exit(EXIT_FAILURE);
    }
}

void Lamp::resetModelView() {
    // Инициализация видовой матрицы как единичной
    view.modelViewMatrix.setToIdentity();

    // Далее аффинные преобразования записаны в обратном порядке

    view.modelViewMatrix.translate(initBias.dx, initBias.dy, initBias.dz);
    // Третья операция - перенос объекта вдоль оси z (например, вглубь экрана)
    // Умножим видовую матрицу на матрицу переноса
    view.modelViewMatrix.translate(bias.dx, bias.dy, -bias.dz);

    // Вторая операция - поворот объекта
    // Умножим видовую матрицу на матрицу поворота
    view.modelViewMatrix *= view.rotateMatrix.transposed();

    // Первая операция - масштабирование объекта (уменьшим объект, чтобы он не занимал весь экран)
    view.modelViewMatrix.scale(kScale, kScale, kScale);
    //modelViewMatrix = initView * modelViewMatrix;
}

void Lamp::resetProjection(const int w, const int h, const float fov) {
    // Инициализация единичной матрицы
    view.projectMatrix.setToIdentity();

    // Умножение на матрицу перспективного проектирования
    view.projectMatrix.perspective(fov, (float) w / h, 0.1, 20);
}

LightObject Lamp::getLight(){ return light; }

}
