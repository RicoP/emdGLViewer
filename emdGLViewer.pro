#-------------------------------------------------
#
# Project created by QtCreator 2012-07-06T15:23:02
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
QT       += designer

TARGET = emdGLViewer
TEMPLATE = app

DEFINES += GL_GLEXT_PROTOTYPES

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    vec3.c \
    str.c \
    quat.c \
    mat4.c \
    mat3.c

HEADERS  += mainwindow.h \
    glwidget.h \
    gl-matrix.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc \
    shader.qrc
