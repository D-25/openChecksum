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

RC_FILE = openChecksum.rc

macx
{
    ICON = icons/macintosh-icon.icns
}

SOURCES += main.cpp\
        mainwindow.cpp \
    dialogstyle.cpp \
    comparechecksums.cpp \
    settingpage.cpp \
    datacalc.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    dialogstyle.h \
    comparechecksums.h \
    settingpage.h \
    datacalc.h \
    about.h

FORMS    += mainwindow.ui \
    settingpage.ui \
    about.ui

RESOURCES += \
    icons.qrc \
    translation.qrc

TRANSLATIONS += \
    languages/lang_it.ts
    languages/lang_en.ts

DISTFILES += \
    openChecksum.rc
