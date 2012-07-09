#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL>
#include <QGLWidget>
#include <QtGui>

#include "gpubuffer.h"
#include "emdobjectset.h"
#include "emdtimbitmap.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int, int);
    void paintGL();
    void repaint();

    void refreshData();

    float& angleX();
    float angleX() const;

    float& angleY();
    float angleY() const;

    EmdObjectSet*& objects();
    EmdObjectSet* objects() const;

    int& part();
    int part() const;

    EmdTimBitmap*& tim();
    EmdTimBitmap* tim() const;
    
private:
    QTimer* timer;
    GLuint program;
    int frame;
    float m_angleX;
    float m_angleY;
    EmdObjectSet* m_objects;
    EmdTimBitmap* m_tim;
    int m_part;
    GLuint texture;
};

#endif // GLWIDGET_H
