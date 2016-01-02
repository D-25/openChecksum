#-------------------------------------------------
#
# Project created by QtCreator 2015-12-04T21:32:07
#
#-------------------------------------------------

QT       += core gui
win32:  QT += winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Md5Checker
TEMPLATE = app

RC_FILE = Md5Checker.rc

macx
{
    ICON = icons/macintosh-icon.icns
}

SOURCES += main.cpp\
        mainwindow.cpp \
    dialogstyle.cpp \
    comparechecksums.cpp \
    settingpage.cpp \
    datacalc.cpp

HEADERS  += mainwindow.h \
    dialogstyle.h \
    comparechecksums.h \
    settingpage.h \
    datacalc.h

FORMS    += mainwindow.ui \
    settingpage.ui

RESOURCES += \
    icons.qrc \
    translation.qrc

TRANSLATIONS += \
    languages/lang_it.ts
    languages/lang_en.ts

DISTFILES += \
    Md5Checker.rc
