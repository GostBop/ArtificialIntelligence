#-------------------------------------------------
#
# Project created by QtCreator 2016-09-11T22:08:38
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = Chess
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Item.cpp \
    Step.cpp

HEADERS  += mainwindow.h \
    Item.h \
    Step.h

FORMS    += mainwindow.ui

RESOURCES += \
    rc.qrc
