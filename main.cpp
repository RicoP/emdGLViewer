#include <QtGui/QApplication>
#include <assert.h>
#include <QFile>
#include "gpubuffer.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //Check the alligned of out structs because this is extremely important for our GPU.
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


//    QFile file(":/shader/shader.vert");
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//        return 0;

//    QTextStream in(&file);

//    while (!in.atEnd()) {
//        QString line = in.readLine();
//        DataPtr p = line.data_ptr();

//        qDebug() << "Hello" << line << "world";
//    }


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
