#-------------------------------------------------
#
# Project created by QtCreator 2015-12-04T21:32:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Md5Checker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialogstyle.cpp \
    comparechecksums.cpp \
    settingspage.cpp

HEADERS  += mainwindow.h \
    dialogstyle.h \
    comparechecksums.h \
    settingspage.h

FORMS    += mainwindow.ui \
    settingspage.ui
