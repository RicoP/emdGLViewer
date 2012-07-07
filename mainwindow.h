#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glwidget.h"

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
    
private:
    Ui::MainWindow *ui;
    GLWidget* gl;
};

#endif // MAINWINDOW_H
