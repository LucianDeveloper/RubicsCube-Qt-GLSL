#include "object/cube.h"

namespace RubicsCube {

Cube::Cube(const int idBuf, const QVector3D& bufBias) : ShaderObject(){
    id = idBuf;
    mode = GL_QUADS;
    material.Ka = material.Kd = material.Ks = QVector3D(1, 1, 1);
    material.shininess = 256;
    rotAngle = 3;
    idx = -1;
    active = true;
    rotFlag = false;
    autoRot = false;
    isMaySelect = true;
    isSuperClass = true;
    isAnimated = true;
    double size = 0.15f;
    for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) for (int k = 0; k < 3; k++) {
        ShaderObject *tmp = (ShaderObject*) new
                SmallCube(1 + i*9 + j*3 + k, QVector3D(
                              (i-1) * size * 2.2 + bufBias.x(),
                              (j-1)* size * 2.2 + bufBias.y(),
                              (k-1)* size * 2.2 + bufBias.z()), size);
        submodels << tmp;
    }

    size *= 2.2;
    object.clear();
    EdgeObject bufEdge;
    bufEdge << Point3D(-size, -size, -size) << Point3D(-size, -size, size)
            << Point3D(size, -size, size) << Point3D(size, -size, -size);
    bufEdge.setColor(QColor(0, 0, 0));
    object << bufEdge;
    bufEdge.clear();
    bufEdge << Point3D(size, -size, size) << Point3D(size, -size, -size)
            << Point3D(size, size, -size) << Point3D(size, size, size);
    bufEdge.setColor(QColor(0, 0, 0));
    object << bufEdge;
    bufEdge.clear();
    bufEdge << Point3D(size, size, -size) << Point3D(size, size, size)
            << Point3D(-size, size, size) << Point3D(-size, size, -size);
    bufEdge.setColor(QColor(0, 0, 0));
    object << bufEdge;
    bufEdge.clear();
    bufEdge << Point3D(-size, size, size) << Point3D(-size, size, -size)
            << Point3D(-size, -size, -size) << Point3D(-size, -size, size);
    bufEdge.setColor(QColor(0, 0, 0));
    object << bufEdge;
    bufEdge.clear();
    bufEdge << Point3D(-size, -size, size) << Point3D(-size, size, size)
            << Point3D(size, size, size) << Point3D(size, -size, size);
    bufEdge.setColor(QColor(0, 0, 0));
    object << bufEdge;
    bufEdge.clear();
    bufEdge << Point3D(-size, -size, -size) << Point3D(-size, size, -size)
            << Point3D(size, size, -size) << Point3D(size, -size, -size);
    bufEdge.setColor(QColor(0, 0, 0));
    object << bufEdge;
    bufEdge.clear();

    sides = QVector<QVector<int>>();
    sides.append(QVector<int>({18, 19, 20, 21, 22, 23, 24 , 25, 26}));
    sides.append(QVector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8}));
    sides.append(QVector<int>({2, 5, 8, 11, 14, 17, 20, 23, 26}));
    sides.append(QVector<int>({0, 3, 6, 9, 12, 15, 18 , 21, 24}));
    sides.append(QVector<int>({0, 1, 2, 9, 10, 11, 18, 19, 20}));
    sides.append(QVector<int>({6, 7, 8, 15, 16, 17, 24 , 25, 26}));

    setInitBias(bufBias);

}

Cube::~Cube() {}

int Cube::bind(const CameraObject& camera, const QVector<LightObject>& lights) {
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

void Cube::release() {
    shpr.disableAttributeArray("posVertex");
    shpr.disableAttributeArray("colorVertex");
    shpr.disableAttributeArray("normalVertex");
    shpr.release();
}

void Cube::initShader() {
    // Текст вершинного шейдера
    shpr.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shaders/vertexLightingShader.vsh");

    // Текст фрагментного шейдера
    shpr.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/shaders/fragmentLightingShader.fsh");

    if (shpr.link() == false) {
        qDebug() << shpr.log();
        exit(EXIT_FAILURE);
    }
}

void Cube::resetModelView() {
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

void Cube::resetProjection(const int w, const int h, const float fov) {
    // Инициализация единичной матрицы
    view.projectMatrix.setToIdentity();

    // Умножение на матрицу перспективного проектирования
    view.projectMatrix.perspective(fov, (float) w / h, 0.1, 20);
}

// Поворот объекта мышью (без реакции)
void Cube::mouseRotate(const int, QMouseEvent*) {return;}

// Объект прокрутили колесом мыши (без реакции)
void Cube::mouseWheel(const int, QWheelEvent*) {return;}

// Когда объект был выделен, нажали клавишу
void Cube::keyPress(const int idCheck, QKeyEvent* event, const QVector<ShaderObject*>& objs) {
    if (id != idCheck) // Если сигнал был послан другому объекту, то ничего не делаем
        return;
    else if (event->key() == Qt::Key_Space){
        if (autoRot) autoRot = false;
        else {
            autoRot = true;
            idx = qrand() % 6;
            rotFlag = true;
        }
    } else if (!rotFlag){
               if (event->key() == Qt::Key_L){ idx = 1; rotFlag = true;
        } else if (event->key() == Qt::Key_R){ idx = 0; rotFlag = true;
        } else if (event->key() == Qt::Key_B){ idx = 3; rotFlag = true;
        } else if (event->key() == Qt::Key_F){ idx = 2; rotFlag = true;
        } else if (event->key() == Qt::Key_D){ idx = 4; rotFlag = true;
        } else if (event->key() == Qt::Key_U){ idx = 5; rotFlag = true; }
    }
    resetModelView();
    emit repaintScene();
}


void Cube::rot90(int idx, int sign) {
    int i, j, k;
    // sign задаётся в зависимости он направления
    // sign = -1, sign = 1
    // если sign = -1, значит крутим 3 раза
    QVector<ShaderObject*> tmp = QVector<ShaderObject*>();
    ShaderObject* t;
    if (sign == -1) sign = 3;
    while (sign--) {
        if (idx & 1 == 1){
            tmp.push_back(submodels[sides[idx][2]]);
            tmp.push_back(submodels[sides[idx][5]]);
            tmp.push_back(submodels[sides[idx][8]]);
            tmp.push_back(submodels[sides[idx][1]]);
            tmp.push_back(submodels[sides[idx][4]]);
            tmp.push_back(submodels[sides[idx][7]]);
            tmp.push_back(submodels[sides[idx][0]]);
            tmp.push_back(submodels[sides[idx][3]]);
            tmp.push_back(submodels[sides[idx][6]]);
        } else {
            tmp.push_back(submodels[sides[idx][6]]);
            tmp.push_back(submodels[sides[idx][3]]);
            tmp.push_back(submodels[sides[idx][0]]);
            tmp.push_back(submodels[sides[idx][7]]);
            tmp.push_back(submodels[sides[idx][4]]);
            tmp.push_back(submodels[sides[idx][1]]);
            tmp.push_back(submodels[sides[idx][8]]);
            tmp.push_back(submodels[sides[idx][5]]);
            tmp.push_back(submodels[sides[idx][2]]);
        }

        for (auto i: sides[idx]) {
            t = tmp.front();
            submodels[i] = t;
            tmp.pop_front();
        }
        tmp.clear();
    }
}

// крутит грань под номером idx на угол angle (в градусах)
void Cube::Rotate(int angle, int reverse) {
    // мы пытаемся покрутить грань с номером idx
    // значит нужно проверить что другая грань уже не крутится
    if (idx == 1 || idx == 3 || idx == 5) angle *= -1;
    if (reverse == true) angle *= -1;

    rotate[idx] += angle;

    if (idx == 0 || idx == 1) for (int i: sides[idx]) ((SmallCube*)submodels[i])->rotateAngleX(angle);
    if (idx == 2 || idx == 3) for (int i: sides[idx]) ((SmallCube*)submodels[i])->rotateAngleZ(angle);
    if (idx == 4 || idx == 5) for (int i: sides[idx]) ((SmallCube*)submodels[i])->rotateAngleY(angle);

    if (rotate[idx] % 90 == 0) {
        // если угол стал кратным 90, то поварачиваем на массиве
        if ((idx == 4 || idx == 5)) rot90(idx, -1);
        else rot90(idx, 1);
        rotate[idx] = 0;
        rotFlag = false;
        idx = -1;
    }
}


void Cube::timerEvent(QTimerEvent *event){
    if (autoRot && !rotFlag){
        rotFlag = true;
        idx = qrand() % 6;
    }
    if (rotFlag == true && idx >= 0){
        Rotate(rotAngle, 0);
        resetModelView();
        emit repaintScene();
    }
}
}
