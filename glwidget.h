#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include <QGLWidget>
#include <QtGui>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    virtual void initializeGL();
    virtual void resizeGL(int, int);
    virtual void paintGL();
    virtual void repaint();
    
signals:
    
public slots:
    
private:
     QTimer* timer;
     GLuint program;
     int frame;
};

#endif // GLWIDGET_H
