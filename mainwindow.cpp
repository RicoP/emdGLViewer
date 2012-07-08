#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"
#include "emdhelper.h"

#include <QGLWidget>
#include <QPushButton>

#define PI 3.14159265f

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    emdblob(0),
    filelength(0),
    objects()
{
    ui->setupUi(this);        

    QObject::connect(ui->horizontalSlider, SIGNAL(valueChanged(int)),
                     this, SLOT(sliderHorizontalChanged(int)));

    QObject::connect(ui->verticalSlider, SIGNAL(valueChanged(int)),
                     this, SLOT(sliderVerticalChanged(int)));

    QObject::connect(ui->actionOpen, SIGNAL(triggered()),
                     this, SLOT(openEmdFile()));

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

void MainWindow::openEmdFile() {
    QString path =
            QFileDialog::getOpenFileName(this, tr("Open EMD File"),
                                         QString(), tr("EMD Files (*.emd)"));

    if(path.isNull() || path.isEmpty()) {
        return;
    }

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "ERROR", "Couldn't open File.");
        return;
    }

    QByteArray array = file.readAll();

    EmdHelper::Dictionary dict = EmdHelper::getDictionary(array);
}

void MainWindow::sliderHorizontalChanged(int degree) {
    gl->angleY() = 2.0f * PI * degree / 360.0f;
}

void MainWindow::sliderVerticalChanged(int degree) {
    gl->angleX() = -2.0f * PI * degree / 360.0f;
}

#undef PI
