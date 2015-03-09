#-------------------------------------------------
#
# Project created by QtCreator 2015-02-01T14:25:40
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 453team1agui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    stdafx.cpp

HEADERS  += mainwindow.h \
    stdafx.h \
    targetver.h \
    Q_DebugStream.h \
    Dijkstra.h \
    save_loadIni.h \
    switchFunctions.h

FORMS    += mainwindow.ui

RESOURCES += \
    startbutton.qrc
