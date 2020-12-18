#include "glWidget.h"

namespace RubicsCube {


MainScene::MainScene() : scene() {
    scene.setGeometry(30,30,1400,1200);
    scene.setWindowTitle("Rubic's Cube with GLSL [LucianDeveloper & Andromagin]");

    ShaderObject *tmp = (ShaderObject*) new Cube(0, QVector3D(0, 0, 0));
    scene << tmp;
    tmp = (ShaderObject*) new Room(1, QVector3D(0, 1, 0));
    scene << tmp;

    Lamp* newLamp = new Lamp(2, QVector3D(5, 3.5, 5), QVector3D(0.0, 0.0, 0.7), 0.3);
    newLamp->light.setLa(QVector3D(0.03, 0.03, 0.05));
    newLamp->light.setLd(QVector3D(0.1, 0.1, 0.5));
    scene << newLamp->getLight();
    scene << (ShaderObject*) newLamp;

    newLamp = new Lamp(2, QVector3D(-5, 3.5, -5), QVector3D(0.7, 0.0, 0.0), 0.3);
    newLamp->light.setLa(QVector3D(0.05, 0.03, 0.03));
    newLamp->light.setLd(QVector3D(0.5, 0.1, 0.1));
    scene << newLamp->getLight();
    scene << (ShaderObject*) newLamp;

    newLamp = new Lamp(2, QVector3D(-5, 3.5, 5), QVector3D(0.0, 0.7, 0.0), 0.3);
    newLamp->light.setLa(QVector3D(0.03, 0.05, 0.03));
    newLamp->light.setLd(QVector3D(0.1, 0.5, 0.1));
    scene << newLamp->getLight();
    scene << (ShaderObject*) newLamp;
}

MainScene::~MainScene() {}

void MainScene::start() { scene.show(); }

}
