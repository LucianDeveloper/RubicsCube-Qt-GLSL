#include "lighting.h"

namespace RubicsCube {

LightObject::LightObject() : info(){info.position = QVector3D(0, 0, 3);}

LightObject::LightObject(const double x, const double y, const double z) : info(){
    info.position = QVector3D(x, y, z);
}

LightObject::LightObject(const QVector3D& bufPos) : info(){info.position = bufPos;}

LightObject::~LightObject() {}

QVector3D LightObject::getPosition() const {return info.position;}

QVector3D LightObject::getLa() const {return info.La;}


QVector3D LightObject::getLd() const {return info.Ld;}

QVector3D LightObject::getLs() const {return info.Ls;}


void LightObject::setPosition(const QVector3D& bufPos){info.position = bufPos;}

void LightObject::setLa(const QVector3D& bufLa){info.La = bufLa;}

void LightObject::setLd(const QVector3D& bufLd){ info.Ld= bufLd;}

void LightObject::setLs(const QVector3D& bufLs){info.Ls = bufLs;}

void LightObject::bindLightWithShaderProgram(const QMatrix4x4& view, QOpenGLShaderProgram& shpr, const int i) const{
    QString num;
    num.setNum(i);
    QString buf = "lights[" + num + "]";
    shpr.setUniformValue((buf + ".position").toStdString().c_str(), info.position);
    shpr.setUniformValue((buf + ".La").toStdString().c_str(), info.La);
    shpr.setUniformValue((buf + ".Ld").toStdString().c_str(), info.Ld);
    shpr.setUniformValue((buf + ".Ls").toStdString().c_str(), info.Ls);
}

}
