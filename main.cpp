#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*
     * Load a new language.
     * To make it happen, the language inside the Resource file MUST BE
     * named with an alias (for example: language_en_EN, language_fr_FR).
     * It automatically change during startup of the application based off the system settings.
     *
     * To make a new language, first edit the .pro file, then make a new language using the tools of QT Creator.
     *
     */

    QTranslator language;
    language.load(":/languages/language_" + QLocale::system().name() + ".qm");
    a.installTranslator(&language);

    qDebug() << "Language loaded: qrc://languages/language_" + QLocale::system().name() + ".qm";

    MainWindow w;
    w.show();

    return a.exec();
}
