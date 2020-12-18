#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "object/cube.h"
#include "object/room.h"
#include "object/lamp.h"

namespace RubicsCube {

class MainScene {
    SceneObject scene;

public:
    MainScene();
    ~MainScene();
    void start();
};

#endif  //GLWIDGET_H

}
