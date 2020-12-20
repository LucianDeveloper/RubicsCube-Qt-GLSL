#include "floor.h"

#include "object/room.h"
#include <QGLWidget>

namespace RubicsCube {


Floor::Floor(const int idBuf, const QVector3D& bufBias) : ShaderObject(){
    id = idBuf;
    mode = GL_QUADS;
    material.Ka = material.Kd = material.Ks = QVector3D(1, 1, 1);
    material.shininess = 256;
    isMaySelect = false;
    isSuperClass = true;
    isAnimated = false;

    double sizeW = 8;
    double sizeH = sizeW * 0.5;
    object.clear();
    EdgeObject bufEdge;
    bufEdge << Point3D(sizeW, -sizeH, -sizeW) << Point3D(sizeW, -sizeH, sizeW)
            << Point3D(-sizeW, -sizeH, sizeW) << Point3D(-sizeW, -sizeH, -sizeW);
    bufEdge.setColor(QColor(255, 255, 255));
    object << bufEdge;
    setInitBias(bufBias);

    //загружаем текстуру

}

Floor::~Floor() {}

int Floor::bind(const CameraObject& camera, const QVector<LightObject>& lights) {
    QVector<float> bufCoords = getVectorEdgeCoords();
    QVector<float> bufColors = getVectorEdgeColor();
    QVector<float> bufNormal = getVectorEdgeNormal();

    shpr.bind();
    glBindTexture(GL_TEXTURE_2D, textureLoc[5]);
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

    // Текстуры
    // Текстурные координаты
    static const float texturePos[4][2] =
    {{ 0.0f,  0.0f}, { 0.0f,  20.0f},  { 20.0f, 20.0f}, { 20.0f,  0.0f}};
    shpr.setAttributeArray("texturePos", (float*)texturePos, 2);
    shpr.enableAttributeArray("texturePos");
    // Конец

    return bufCoords.size() / 3;
  }

void Floor::release() {
    shpr.disableAttributeArray("posVertex");
    shpr.disableAttributeArray("colorVertex");
    shpr.disableAttributeArray("normalVertex");
    shpr.disableAttributeArray("texturePos");
    shpr.release();
}

void Floor::initShader() {
    // Текст вершинного шейдера
    shpr.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shaders/vertexTextureShader.vsh");

    // Текст фрагментного шейдера
    shpr.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/shaders/fragmentTextureShader.fsh");

    if (shpr.link() == false) {
        qDebug() << shpr.log();
        exit(EXIT_FAILURE);
    }

    // Текстуры
    QImage texture;
    glGenTextures(5, textureLoc);
    texture.load(":/assets/assets/floor.bmp");
    texture = QGLWidget::convertToGLFormat(texture);
    glBindTexture(GL_TEXTURE_2D, textureLoc[5]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // параметры фильтрации
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, GLsizei(texture.width()), GLsizei(texture.height()), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());
}

void Floor::resetModelView() {
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
}

void Floor::resetProjection(const int w, const int h, const float fov) {
    // Инициализация единичной матрицы
    view.projectMatrix.setToIdentity();

    // Умножение на матрицу перспективного проектирования
    view.projectMatrix.perspective(fov, (float) w / h, 0.1, 20);
}

}
