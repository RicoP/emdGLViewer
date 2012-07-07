#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"

#include <QGLWidget>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);        

    //QPushButton* btn = new QPushButton(ui->centralWidget);
    //btn->show();

    gl = new GLWidget(ui->center);
    gl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //gl->setFixedSize(640, 480);
    gl->show();

    QObject::connect(ui->actionOpen, SIGNAL(triggered()),
                     this, SLOT(onResize()));        

//    QPushButton* btn = new QPushButton(ui->center);
//    btn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//    btn->setFixedWidth(640);
//    btn->setFixedHeight(480);
//    btn->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent * event) {
    QMainWindow::resizeEvent(event);

    qDebug() << "resizeEvent";
    //gl->resizeGL(ui->center->size().width(), ui->center->size().height());
    gl->setFixedSize(ui->center->size().width(), ui->center->size().height());
}
