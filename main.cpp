#include <QtGui/QApplication>
#include <assert.h>
#include <QFile>
#include "gpubuffer.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //Check the alligned of our structs
    assert(sizeof(GLfloat) == 4);

    assert(offsetof(vector4, x) == 0);
    assert(offsetof(vector4, y) == 4);
    assert(offsetof(vector4, z) == 8);
    assert(offsetof(vector4, w) == 12);

    assert(offsetof(textureuv, u) == 0);
    assert(offsetof(textureuv, v) == 4);

    assert(offsetof(vertexvtn, position) == 0);
    assert(offsetof(vertexvtn, uv)       == sizeof(vector4));
    assert(offsetof(vertexvtn, normal)   == sizeof(vector4) + sizeof(textureuv));

    assert(sizeof(vector4)     == sizeof(GLfloat) * 4);
    assert(sizeof(textureuv)   == sizeof(GLfloat) * 2);
    assert(sizeof(vertexvtn)   == sizeof(vector4) + sizeof(textureuv) + sizeof(vector4));
    assert(sizeof(trianglevtn) == sizeof(vertexvtn) * 3);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
