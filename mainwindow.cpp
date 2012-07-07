#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"

#include <QGLWidget>
#include <QPushButton>

#define PI 3.14159265f

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);        

    QObject::connect(ui->horizontalSlider, SIGNAL(valueChanged(int)),
                     this, SLOT(sliderHorizontalChanged(int)));

    QObject::connect(ui->verticalSlider, SIGNAL(valueChanged(int)),
                     this, SLOT(sliderVerticalChanged(int)));

    gl = new GLWidget(ui->center);
    gl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);  
    gl->show();  
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent * event) {
    QMainWindow::resizeEvent(event);

    QSize size = ui->center->size();
    gl->setFixedSize(size.width(), size.height());
}

void MainWindow::sliderHorizontalChanged(int degree) {
    gl->angleY() = 2.0f * PI * degree / 360.0f;
}

void MainWindow::sliderVerticalChanged(int degree) {
    gl->angleX() = 2.0f * PI * degree / 360.0f;
}

#undef PI
