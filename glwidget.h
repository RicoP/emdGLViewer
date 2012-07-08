#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include <QGLWidget>
#include <QtGui>

#include "gpubuffer.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = 0);
    virtual void initializeGL();
    virtual void resizeGL(int, int);
    virtual void paintGL();
    virtual void repaint();    

    float& angleX();
    float angleX() const;

    float& angleY();
    float angleY() const;
    
private:
     QTimer* timer;
     GLuint program;
     int frame;
     float m_angleX;
     float m_angleY;

    trianglevtn* triangles;
};

#endif // GLWIDGET_H
