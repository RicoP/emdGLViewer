#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGLWidget>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);        

    QPushButton* btn = new QPushButton("Hallo", ui->glBox);
    btn->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
