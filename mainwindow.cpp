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
