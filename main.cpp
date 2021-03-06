#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QTranslator>
#include <QSettings>


int main(int argc, char *argv[])
{
    // Application is scaled based on screen's DPI.
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");

    QApplication a(argc, argv);

    QApplication::setApplicationName("openChecksum");
    QApplication::setApplicationVersion("0.7.2");
    QApplication::setOrganizationName("D-25");
    QApplication::setOrganizationDomain("http://d-25.net");
    QApplication::setWindowIcon(QIcon(":/icons/ico_windows"));

    /*
     * Load a new language.
     * To make it happen, the language inside the Resource file MUST BE
     * named with an alias (for example: language_en_EN, language_fr_FR).
     * It automatically change during startup of the application based off the system settings.
     *
     * To make a new language, first edit the .pro file, then make a new language using the tools of QT Creator.
     *
     */

    QSettings settings("D-25" ,"openChecksum");

    int languageSelected = settings.value("language", 0).toInt();
    QTranslator language;

    switch (languageSelected)
    {
        case 1: // Italian.
        {
            language.load(":/languages/language_it_IT.qm");
            a.installTranslator(&language);
            break;
        }

        case 2: // English.
        {
            language.load(":/languages/language_en_EN.qm");
            a.installTranslator(&language);
            break;
        }

        default: // Detected by system settings or unknown value.
        {
            language.load(":/languages/language_" + QLocale::system().name() + ".qm");
            a.installTranslator(&language);

            qDebug() << "Language loaded: qrc://languages/language_" + QLocale::system().name() + ".qm";
            break;
        }
    }

    MainWindow w;
    w.show();

    return a.exec();
}
