#include "gpubuffer.h"
#include <QtGui/QApplication>
#include "mainwindow.h"
#include <assert.h>

int main(int argc, char *argv[])
{
    assert(offsetof(vector4, x) == 0);
    assert(offsetof(vector4, w) == 4 * 3);
    assert(offsetof(tuplevt, vertex) == 0);
    assert(offsetof(tuplevt, textcoord) == sizeof(vector4));
    assert(sizeof(trianglevt) == 4 * 3 * (4 + 2));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
