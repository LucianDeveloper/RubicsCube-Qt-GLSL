#include "object/smallcube.h"


namespace RubicsCube {

SmallCube::SmallCube(const int idBuf, const QVector3D& bufBias, double size) : ShaderObject() {
    id = idBuf;
    mode = GL_QUADS;
    material.Ka = material.Kd = material.Ks = QVector3D(1, 1, 1);
    material.shininess = 256;
    active = true;
    isMaySelect = false;
    colors = QVector<QColor>();

    setInitBias(bufBias);

    object.clear();
    EdgeObject bufEdge;
    bufEdge << Point3D(-size, -size, -size) << Point3D(-size, -size, size)
            << Point3D(size, -size, size) << Point3D(size, -size, -size);
    bufEdge.setColor(QColor(200, 200, 200));
    colors.append(QColor(200, 200, 200));
    object << bufEdge;

    bufEdge.clear();
    bufEdge << Point3D(size, -size, size) << Point3D(size, -size, -size)
            << Point3D(size, size, -size) << Point3D(size, size, size);
    bufEdge.setColor(QColor(0, 0, 255));
    colors.append(QColor(0, 0, 255));
    object << bufEdge;

    bufEdge.clear();
    bufEdge << Point3D(size, size, -size) << Point3D(size, size, size)
            << Point3D(-size, size, size) << Point3D(-size, size, -size);
    bufEdge.setColor(QColor(255, 255, 0));
    colors.append(QColor(255, 255, 0));
    object << bufEdge;

    bufEdge.clear();
    bufEdge << Point3D(-size, size, size) << Point3D(-size, size, -size)
            << Point3D(-size, -size, -size) << Point3D(-size, -size, size);
    bufEdge.setColor(QColor(0, 255, 0));
    colors.append(QColor(0, 255, 0));
    object << bufEdge;

    bufEdge.clear();
    bufEdge << Point3D(-size, -size, size) << Point3D(-size, size, size)
            << Point3D(size, size, size) << Point3D(size, -size, size);
    bufEdge.setColor(QColor(255, 153, 13));
    colors.append(QColor(255, 153, 13));
    object << bufEdge;

    bufEdge.clear();
    bufEdge << Point3D(-size, -size, -size) << Point3D(-size, size, -size)
            << Point3D(size, size, -size) << Point3D(size, -size, -size);
    bufEdge.setColor(QColor(255, 0, 0));
    colors.append(QColor(255, 0, 0));
    object << bufEdge;
}

SmallCube::~SmallCube() {}

int SmallCube::bind(const CameraObject& camera, const QVector<LightObject>& lights) {
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

void SmallCube::release() {
    shpr.disableAttributeArray("posVertex");
    shpr.disableAttributeArray("colorVertex");
    shpr.disableAttributeArray("normalVertex");

    shpr.release();
}

void SmallCube::initShader() {
    // Текст вершинного шейдера
    shpr.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shaders/vertexLightingShader.vsh");

    // Текст фрагментного шейдера
    shpr.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/shaders/fragmentLightingShader.fsh");

    if (shpr.link() == false) {
        qDebug() << shpr.log();
        exit(EXIT_FAILURE);
    }
}

void SmallCube::resetModelView() {
    // Инициализация видовой матрицы как единичной
    view.modelViewMatrix.setToIdentity();

    // Далее аффинные преобразования записаны в обратном порядке

    // Вторая операция - поворот объекта
    // Умножим видовую матрицу на матрицу поворота
    view.modelViewMatrix *= view.rotateMatrix.transposed();

    view.modelViewMatrix.translate(initBias.dx, initBias.dy, initBias.dz);
    // Вторая операция - перенос объекта вдоль оси z (например, вглубь экрана)
    // Умножим видовую матрицу на матрицу переноса
    view.modelViewMatrix.translate(bias.dx, bias.dy, -bias.dz);

    // Первая операция - масштабирование объекта (уменьшим объект, чтобы он не занимал весь экран)
    view.modelViewMatrix.scale(kScale, kScale, kScale);
    //modelViewMatrix = initView * modelViewMatrix;
}

void SmallCube::resetProjection(const int w, const int h, const float fov) {
    // Инициализация единичной матрицы
    view.projectMatrix.setToIdentity();

    // Умножение на матрицу перспективного проектирования
    view.projectMatrix.perspective(fov, (float) w / h, 0.1, 20);
}

// поворот на плоскости X0Y
void SmallCube::rotateAngleZ(double ang) {
    view.rotateMatrix.rotate(ang, 0, 0, 1);
    resetModelView();
    emit repaintScene();
}

// поворот на плоскости X0Z
void SmallCube::rotateAngleY(double ang) {
    view.rotateMatrix.rotate(ang, 0, 1, 0);
    resetModelView();
    emit repaintScene();
}

// поворот на плоскости Y0Z
void SmallCube::rotateAngleX(double ang) {
    view.rotateMatrix.rotate(ang, 1, 0, 0);
    resetModelView();
    emit repaintScene();
}

}
