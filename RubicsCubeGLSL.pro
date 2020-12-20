QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lglu32 -lopengl32

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    lighting.cpp \
    glWidget.cpp \
    object/cube.cpp \
    object/room.cpp \
    object/smallcube.cpp \
    wrappers/cameraObject.cpp \
    wrappers/edgeObject.cpp \
    wrappers/pointObject.cpp \
    wrappers/sceneObject.cpp \
    wrappers/shaderObject.cpp \
    object/lamp.cpp \
    object/sky.cpp \
    object/floor.cpp

HEADERS += \
    mainwindow.h \
    lighting.h \
    glWidget.h \
    object/cube.h \
    object/room.h \
    object/smallcube.h \
    wrappers/cameraObject.h \
    wrappers/edgeObject.h \
    wrappers/pointObject.h \
    wrappers/sceneObject.h \
    wrappers/shaderObject.h \
    object/lamp.h \
    object/sky.h \
    object/floor.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    shaders/fragmentLightingShader.fsh \
    shaders/vertexLightingShader.vsh \
    assets/2cvr.bmp \
    assets/3cvr.bmp \
    assets/floor.bmp \
    assets/glass.bmp \
    assets/lamp.bmp \
    assets/pages_2.bmp \
    assets/sky.bmp \
    assets/sky2.bmp \
    assets/wall.bmp \
    assets/wood.bmp \
    shaders/fragmentTextureShader.fsh \
    shaders/vertexTextureShader.vsh

