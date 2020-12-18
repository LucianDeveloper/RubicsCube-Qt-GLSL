#include "mainwindow.h"

#include <QApplication>
#include "wrappers/sceneObject.h"
#include "glWidget.h"
using namespace RubicsCube;

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
    QApplication a(argc, argv);

    MainScene game;
    game.start();

    return a.exec();
}
