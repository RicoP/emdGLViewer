#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"
#include "emdhelper.h"
#include "emdtimbitmap.h"
#include "about.h"

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

    QObject::connect(ui->actionFirstPart, SIGNAL(triggered()),
                     this, SLOT(setFirst()));

    QObject::connect(ui->actionLastPart, SIGNAL(triggered()),
                     this, SLOT(setLast()));

    QObject::connect(ui->actionPreviousPart, SIGNAL(triggered()),
                     this, SLOT(setPrevious()));

    QObject::connect(ui->actionNextPart, SIGNAL(triggered()),
                     this, SLOT(setNext()));

    QObject::connect(ui->actionAbout, SIGNAL(triggered()),
                     this, SLOT(showAbout()));

    QObject::connect(ui->actionExportObj, SIGNAL(triggered()),
                     this, SLOT(exportObj()));

    QObject::connect(ui->actionExportBmp, SIGNAL(triggered()),
                     this, SLOT(exportBmp()));

    QObject::connect(ui->actionZoom_In, SIGNAL(triggered()),
                     this, SLOT(zoomIn()));

    QObject::connect(ui->actionZoom_Out, SIGNAL(triggered()),
                     this, SLOT(zoomOut()));

    QObject::connect(ui->actionWireframe, SIGNAL(triggered(bool)),
                     this, SLOT(wireframe(bool)));



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

    bool successSet = false;
    EmdObjectSet* set = new EmdObjectSet(array.mid(dict.mesh), &successSet);

    bool successTim = false;
    EmdTimBitmap* tim = new EmdTimBitmap(array.mid(dict.tim), &successTim);

    if(successSet && successTim) {
        delete gl->objects();
        delete gl->tim();

        gl->objects() = set;
        gl->tim() = tim;
        gl->part() = 0;

        gl->refreshData();

        ui->actionExportBmp->setEnabled(true);
        ui->actionExportObj->setEnabled(true);
        ui->actionFirstPart->setEnabled(true);
        ui->actionLastPart->setEnabled(true);
        ui->actionNextPart->setEnabled(true);
        ui->actionPreviousPart->setEnabled(true);
        ui->actionWireframe->setEnabled(true);
        ui->actionZoom_In->setEnabled(true);
        ui->actionZoom_Out->setEnabled(true);
    }
    else {
        if(!successSet && !successTim) {
            QMessageBox::warning(this, "ERROR", "Error while parsing mesh and image data.");
        }
        else if(!successSet) {
            QMessageBox::warning(this, "ERROR", "Error while parsing mesh Data.");
        }
        else if(!successTim) {
            QMessageBox::warning(this, "ERROR", "Error while parsing image Data.");
        }

        delete set;
        delete tim;
    }
}

void MainWindow::setFirst() {
    if(this->gl->objects()) {
        this->gl->part() = 0;
    }
}

void MainWindow::setLast() {
    if(this->gl->objects()) {
        this->gl->part() = gl->objects()->numParts() - 1;        
    }
}

void MainWindow::setPrevious() {
    if(gl->objects() && gl->part() > 0) {
        gl->part()--;
    }
}

void MainWindow::setNext() {
    if(gl->objects() && gl->part() < (gl->objects()->numParts() - 1)) {
        gl->part()++;
    }
}

void MainWindow::sliderHorizontalChanged(int degree) {
    gl->angleY() = 2.0f * PI * degree / 360.0f;    
}

void MainWindow::sliderVerticalChanged(int degree) {
    gl->angleX() = -2.0f * PI * degree / 360.0f;        
}

void MainWindow::showAbout() {
    About about(this);
    about.exec();
}

void MainWindow::zoomIn() {
    gl->zoom(-0.1f);
}

void MainWindow::zoomOut() {
    gl->zoom(0.1f);
}

void MainWindow::wireframe(bool b) {
    gl->showWireframe() = b;
}

void MainWindow::exportObj() {
    QString path =
            QFileDialog::getSaveFileName(this, tr("Save OBJ File"),
                                         QString(), tr("OBJ Files (*.obj)"));

    if(path.isNull() || path.isEmpty()) {
        return;
    }

    gl->objects()->writeObjFile(path);
}

void MainWindow::exportBmp() {
    QString path =
            QFileDialog::getSaveFileName(this, tr("Save BMP File"),
                                         QString(), tr("BMP Files (*.bmp)"));

    if(path.isNull() || path.isEmpty()) {
        return;
    }

    gl->tim()->writeBmpFile(path);
}

#undef PI
