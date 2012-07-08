#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glwidget.h"
#include "gpubuffer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    virtual void resizeEvent(QResizeEvent *);

public Q_SLOTS:
    void sliderHorizontalChanged(int);
    void sliderVerticalChanged(int);
    void openEmdFile();

    void setFirst();
    void setLast();
    void setPrevious();
    void setNext();

    void showAbout();
    
private:
    Ui::MainWindow *ui;
    GLWidget* gl;

    void* emdblob;
    int filelength;

    objectset objects;
};

#endif // MAINWINDOW_H
