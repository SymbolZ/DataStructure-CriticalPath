#-------------------------------------------------
#
# Project created by QtCreator 2017-08-24T16:50:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DataStructure
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    renderarea.cpp \
    dialog_in_ar.cpp \
    topocontrol.cpp

HEADERS  += mainwindow.h \
    renderarea.h \
    def.h \
    linkedlist.h \
    stack.h \
    dialog_in_ar.h \
    topocontrol.h

FORMS    += mainwindow.ui \
    renderarea.ui \
    dialog_in_ar.ui \
    topocontrol.ui \
    form.ui

RC_FILE=images/logo.rc

DISTFILES += \
    images/logo.ico \
    images/logo.rc
