#-------------------------------------------------
#
# Project created by QtCreator 2012-07-06T15:23:02
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
QT       += designer

TARGET = emdviewer
TEMPLATE = app

DEFINES += GL_GLEXT_PROTOTYPES

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    glwidgetplugin.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    glwidgetplugin.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
