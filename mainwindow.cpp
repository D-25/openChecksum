#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QCryptographicHash>
#include <QFile>
#ifdef Q_OS_WIN32
    #include <QtWinExtras/QWinTaskbarButton>
    #include <QtWinExtras/QWinTaskbarProgress>
#endif
#include <QDragEnterEvent>
#include <QMimeData>
#include <QUrl>
#include <QDebug>
#include <QTranslator>
#include <QSettings>
#include <QTime>

#include "settingpage.h"
#include "dialogstyle.h"
#include "comparechecksums.h"
#include "datacalc.h"

/*
 * A primordial version of the program.
 * More functions coming soon, just stay tuned.
 * Please.
 *
 * Danilo Civati (http://d-25.net)
 * cdanilo25@gmail.com
 *
 */

bool aborted; // I know, this is ugly.

bool checkAndCompare;
QString fileNameCompare;

int statistic_checkCounter;
int statistic_checkEnded;
int statistic_checkAborted;
qint64 statistic_totalTime;
qint64 statistic_totalDataChecked;
qint64 statistic_totalData;
QString statistic_lastFile;

int Speed = 262144;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->checkingBar->setVisible(false);
    ui->abortButton->setVisible(false);

    this->setWindowTitle(tr("%1 %2").arg(QApplication::applicationName()).arg(QApplication::applicationVersion()));

    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// disableAll and enableAll was used instead of this.setEnable(false) to
// mantain AbortButton enabled.

void MainWindow::disableAll()
{
    ui->startCheck->setEnabled(false);
    ui->fileSelectLocation->setEnabled(false);
    ui->fileSelectBrowse->setEnabled(false);
    ui->comparationCheck->setEnabled(false);
    ui->comparationString->setEnabled(false);
    ui->radioMd5->setEnabled(false);
    ui->radioSHA1->setEnabled(false);
    ui->radioSHA256->setEnabled(false);
    ui->menuBar->setEnabled(false);
}

void MainWindow::enableAll()
{
    ui->startCheck->setEnabled(true);
    ui->fileSelectLocation->setEnabled(true);
    ui->fileSelectBrowse->setEnabled(true);
    ui->comparationCheck->setEnabled(true);
    ui->comparationString->setEnabled(true);
    ui->radioMd5->setEnabled(true);
    ui->radioSHA1->setEnabled(true);
    ui->radioSHA256->setEnabled(true);
    ui->menuBar->setEnabled(true);
}

void MainWindow::loadStatistics()
{
    QSettings settings("D-25" ,"MD5Checker");
    statistic_checkCounter = settings.value("statistics/checkCounter", 0).toInt();
    statistic_checkEnded = settings.value("statistics/checkEnded", 0).toInt();
    statistic_checkAborted = settings.value("statistics/checkAborted", 0).toInt();
    statistic_totalTime = settings.value("statistics/totalTime", -1).toULongLong();
    statistic_totalDataChecked = settings.value("statistics/totalDataChecked", 0).toULongLong();
    statistic_totalData = settings.value("statistics/totalData", 0).toULongLong();
    statistic_lastFile = settings.value("statistics/lastFile", tr("Nessun file ancora analizzato.")).toString();
    qDebug() << "Statistics loaded!";
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void MainWindow::dropEvent(QDropEvent *event)
{

    QList<QUrl> fileDropped = event->mimeData()->urls();
    QString droppedLocation;

    foreach (QUrl fileDroppedPosition, fileDropped)
    {

        // This will be edited when Batch operations is implemented.
        droppedLocation = fileDroppedPosition.toString();
        qDebug() << "Dropped: " + droppedLocation;

        if (!droppedLocation.contains(QString("file:///"))) // Block all files dropped by unknown position.
        {
            dialogStyle_info(tr("Posizione non valida."), tr("Il file <b>%1</b> possiede "
                                 "una posizione non valida, pertanto è stato ignorato al momento del "
                                 "trascinamento.").arg(droppedLocation));
            break;
        }

        droppedLocation.remove(QString("file:///")); // We need the file:/// prefix to mantain URL-style
                                                     // and block all unknown position.
#if defined(linux) || defined(unix) // On Linux/Unix system add '/' to mantain file system structure. It works on Mac?
        droppedLocation.insert(0, "/");
        qDebug() << "*** Linux/Unix system detected: mantaining file system structure.";
#endif

        ui->fileSelectLocation->setText(droppedLocation);
        ui->startCheck->setEnabled(true);
    }
}

void MainWindow::on_fileSelectBrowse_clicked()
{
    // Open a File Browse dialog and, if a file was selected, the button to start the check become available.

    QString stringFileName = QFileDialog::getOpenFileName(this, tr("Apri"), "", tr("Tutti (*.*)"));
    if (stringFileName.isNull() || stringFileName.isEmpty())
    {
        if (checkAndCompare == true) { checkAndCompare = false; } // Make global value deactivated, so Comparation Mode won't start.
    }

    else
    {
        ui->startCheck->setEnabled(true);
        ui->action_Check->setEnabled(true);
        if (checkAndCompare == false) { ui->fileSelectLocation->setText(stringFileName); }
        else
        {
            ui->comparationCheck->setChecked(false);
            fileNameCompare = stringFileName; // Using global value.
            on_startCheck_clicked();
        }
    }
}

void MainWindow::on_startCheck_clicked()
{

    // TODO: better code and more functions.

    // Select file, initialize the byteload and start processing the file, disabling the main window for security purpose.
    // Reached the ending of file, the checksum is showed to user.

    // Please note we use checkAndCompare boolean global value for the function inside the 'File' menu.
    // I can't make a function with variables because QT uses a closed system with signal slots, so I use this workaround.
    // If you have a better method, let me know on GitHub.

    QSettings settings("D-25" ,"MD5Checker");
    QTime timeCount;

    int checkType; // 0: MD5, 1: SHA-1, 2: SHA-256.
    QString checkTypeString;

    if (ui->radioMd5->isChecked())
    {
        checkType = 0;
        checkTypeString = "MD5";
    }

    else if (ui->radioSHA1->isChecked())
    {
        checkType = 1;
        checkTypeString = "SHA-1";
    }

    else if (ui->radioSHA256->isChecked())
    {
        checkType = 2;
        checkTypeString = "SHA-256";
    }


    int byteCheckSelected = settings.value("byteCheck", 262144).toInt();
    bool getFrozenStatus = settings.value("applyFrozenStatus", 0).toBool();
    bool showLastFile = settings.value("saveLastFile", 1).toBool();

    qDebug() << "Settings loaded: " << "byteCheckSelected..." << byteCheckSelected << "getFrozenStatus..." << getFrozenStatus << "showLastFile..." << showLastFile;
    loadStatistics();

    QString fileName = ui->fileSelectLocation->text();

    if (checkAndCompare == true) { fileName = fileNameCompare; qDebug() << "*** STARTING COMPARATION MODE ***"; }

    QFile fileSelected(fileName);

    // If user want to start checking between two MD5s, it start to check if the MD5 inputed
    // follow the HEX-system. If not, checking won't start.
    if (ui->comparationCheck->isChecked())
    {
        if (inputCheckHEX(checkType, ui->comparationString->text()) == 0)
        {
            dialogStyle_info(tr("Comparazione non possibile"), tr("La stringa inserita non è valida."
                                                                  "<ul><li>I numeri da 0 a 9 sono ammessi.</li>"
                                                                  "<li>Solo le lettere da A ad F sono ammesse.</li>"
                                                                  "<li>Le lettere non fanno distinzione da maiuscole a minuscole.</li>"
                                                                  "<li>Tipicamente trovi queste informazioni nelle pagine di Download.</li>"
                                                                  "<li>Utilizza la funzione Analizza e Compara nel menù File per generare un codice da comparare.</li>"
                                                                  "<li>Ogni tipo di analisi ha una lughezza prestabilita diversa.</li>"
                                                                  "<ul><li><b>MD5: </b> 32 caratteri.</li>"
                                                                  "<li><b>SHA-1: </b> 40 caratteri.</li>"
                                                                  "<li><b>SHA-256: </b> 64 caratteri.</li></ul></ul>"));

            qDebug() << "Checking stopped due invalid Checksum inserted by user.";
            return;
        }
    }

    if (getFrozenStatus == true)
    {
        if(dialogStyle_question(tr("Status di congelamento"), tr("L'applicazione verrà segnalata come <b>NON RISPONDE</b> dal sistema. Non chiudere il processo.<br/><br/>"
                                                                 "Vuoi continuare? Potrebbe richiedere parecchio tempo, a seconda della dimensione del File.<br/><br/>"
                                                                 "Puoi disabilitare questa funzione nelle Impostazioni."), tr("Si, continua"), tr("No, annulla")) == 0)
        {
            ui->checkInfo->setText(tr("Operazione annullata dall'utente."));
            qDebug() << "Operation aborted by user.";
            return;
        }
    }

    QCryptographicHash checkProcess0(QCryptographicHash::Md5);
    QCryptographicHash checkProcess1(QCryptographicHash::Sha1);
    QCryptographicHash checkProcess2(QCryptographicHash::Sha256);

        timeCount.start();
        fileSelected.open(QFile::ReadOnly);
        statistic_checkCounter++;
        statistic_lastFile = fileName;

        ui->checkingBar->setMaximum(100);

#ifdef Q_OS_WIN32
        taskbarButton = new QWinTaskbarButton(this);
        taskbarButton->setWindow(this->windowHandle());

        taskbarProgress = taskbarButton->progress();
        taskbarProgress->setVisible(true);
        taskbarProgress->setMaximum(100);
#endif


        qint64 byteReaden = 0;
        qint64 fileSize = fileSelected.size();


        int fileSizeKB = fileSize / 1024;
        int fileSizeMB = fileSizeKB / 1024;
        int fileSizeGB = fileSizeMB / 1024;

        QString checkText1 = tr("Analisi del File selezionato in corso...");
        QString checkText2 = tr("Dati analizzati");

        aborted = false;
        while(!fileSelected.atEnd())
        {
            switch (checkType)
            {
                case 0:
                {
                    checkProcess0.addData(fileSelected.read(byteCheckSelected));
                    break;
                }

                case 1:
                {
                    checkProcess1.addData(fileSelected.read(byteCheckSelected));
                    break;
                }

                case 2:
                {
                    checkProcess2.addData(fileSelected.read(byteCheckSelected));
                    break;
                }
            }


            byteReaden = byteReaden + byteCheckSelected; // Byte Readen by application is based of current byteCheckSelected.

            int byteReadenPerCent = (byteReaden * 100) / fileSize;
            int byteReadenKB = byteReaden / 1024;
            int byteReadenMB = byteReadenKB / 1024;
            int byteReadenGB = byteReadenMB / 1024;

            ui->checkingBar->setValue(byteReadenPerCent); // All numbers is converted for maximum 100%.
#ifdef Q_OS_WIN32
            taskbarProgress->setValue(byteReadenPerCent);
            taskbarProgress->show();
#endif

            if (getFrozenStatus == false) { QCoreApplication::processEvents(); }

            if (fileSizeMB < 10) // Show in Kilobyte
            {

                ui->checkInfo->setText(tr("%1<br/> <b>%2:</b> %3 KB su %4 KB").arg(checkText1).arg(checkText2).arg(byteReadenKB).arg(fileSizeKB));
            }

            else if (fileSizeGB < 2) // Show in Megabyte
            {
                ui->checkInfo->setText(tr("%1<br/> <b>%2:</b> %3 MB su %4 MB").arg(checkText1).arg(checkText2).arg(byteReadenMB).arg(fileSizeMB));
            }

            else // Show Megabyte + Gigabyte
            {
                ui->checkInfo->setText(tr("%1<br/> <b>%2:</b> %3 MB (%4 GB) su %5 MB (%6 GB)").arg(checkText1).arg(checkText2).arg(byteReadenMB).arg(byteReadenGB).arg(fileSizeMB).arg(fileSizeGB));
            }

            ui->checkingBar->setVisible(true);
            ui->abortButton->setVisible(true);

            disableAll();

            if (aborted == true)
            {
                break; // When AbortButton is pressed, the Checksum process is killed.
            }

        }

        statistic_totalDataChecked = statistic_totalDataChecked + byteReaden;
        enableAll();


#ifdef Q_OS_WIN32
        taskbarProgress->setValue(0);
#endif
        ui->checkingBar->setVisible(false);
        ui->abortButton->setVisible(false);

        QByteArray md5Data;

        switch (checkType)
        {
            case 0:
            {
                md5Data = checkProcess0.result();
                break;
            }

            case 1:
            {
                md5Data = checkProcess1.result();
                break;
            }

            case 2:
            {
                md5Data = checkProcess2.result();
                break;
            }
        }

        QString md5DataHEX = md5Data.toHex();

        if (aborted == false)
        {
            qDebug() << "Checking done! Time Elapsed (ms): " << timeCount.elapsed();
            QString timeString = timeCalc(timeCount.elapsed());

            if (checkAndCompare == false)
            {
                ui->checkInfo->setText(tr("<b>%1 del File scelto:</b> %2<br/><b>Tempo trascorso:</b> %3").arg(checkTypeString, md5DataHEX, timeString));

                if (ui->comparationCheck->isChecked())
                {
                    comparationStart(ui->comparationString->text(), md5DataHEX); // Check between two string, one inserted by user.
                }

                statistic_checkEnded++;
            }

            else // COMPARATION MODE
            {
                ui->checkInfo->setText(tr("%1 del File da comparare calcolato. Adesso scegli il File da comparare e avvia l'analisi.").arg(checkTypeString));

                if (ui->fileSelectLocation->text().isNull() || ui->fileSelectLocation->text().isEmpty()) // Check if a File is selected,
                                                                                                         // to make enabled START CHECK buttons.
                {
                    ui->startCheck->setEnabled(false);
                    ui->action_Check->setEnabled(false);
                }

                ui->comparationCheck->setChecked(true);
                ui->comparationString->setText(md5DataHEX);

                checkAndCompare = false;
                statistic_checkEnded++;
            }
        }

        else // ABORTED
        {
                ui->checkInfo->setText(tr("Operazione annullata dall'utente."));
                statistic_checkAborted++;
                if (checkAndCompare == true) { checkAndCompare = false; } // Make global value deactivated, so Comparation Mode won't start next time.
        }

        statistic_totalTime = statistic_totalTime + timeCount.elapsed();
        statistic_totalData = statistic_totalData + fileSize;
        settings.setValue("statistics/checkCounter", statistic_checkCounter);
        settings.setValue("statistics/checkEnded", statistic_checkEnded);
        settings.setValue("statistics/checkAborted", statistic_checkAborted);
        settings.setValue("statistics/totalTime", statistic_totalTime);
        settings.setValue("statistics/totalDataChecked", statistic_totalDataChecked);
        settings.setValue("statistics/totalData", statistic_totalData);
        if (showLastFile == true) { settings.setValue("statistics/lastFile", statistic_lastFile); }
        qDebug() << "Statistics updated!";
}

void MainWindow::on_abortButton_clicked()
{
    aborted = true;
}

// MENU BAR

void MainWindow::on_action_Settings_triggered() // SETTINGS
{
    SettingPage settings;
    settings.exec();
}

void MainWindow::on_action_Open_triggered() // OPEN (recall Browse button)
{
    on_fileSelectBrowse_clicked();
}

void MainWindow::on_action_Check_triggered() // CHECK (recall Start Check button)
{
    on_startCheck_clicked();
}

void MainWindow::on_action_OpenCompare_triggered() // Open & Compare (set TRUE the global value, then recall Browse button)
{
    checkAndCompare = true;
    on_fileSelectBrowse_clicked();
}

void MainWindow::on_action_Exit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_action_Statistics_triggered()
{

    // STATISTICS LOAD

    QSettings settings("D-25" ,"MD5Checker");
    bool showLastFile = settings.value("saveLastFile", 1).toBool();
    QString noinfo = tr("<i>nessun dato disponibile.</i>");
    QString notready = tr("<i>dati non ancora pronti.</i>");

    loadStatistics();

    // LAST FILE CHECKED

    QString lastFile;

    if (showLastFile == false)
    {
        lastFile = tr("<i>funzione disabilitata nelle impostazioni.</i>");
    }

    else
    {
        lastFile = statistic_lastFile;
    }

    QString totalTime = timeCalc(statistic_totalTime); // TOTAL TIME

    // DATA CALC

    QString dataChecked;
    QString dataTotal;
    QString dataRatioString;

    int dataCheckedKB = statistic_totalDataChecked / 1024;
    int dataCheckedMB = dataCheckedKB / 1024;
    int dataCheckedGB = dataCheckedMB / 1024;
    int dataTotalKB = statistic_totalData / 1024;
    int dataTotalMB = dataTotalKB / 1024;
    int dataTotalGB = dataTotalMB / 1024;
    int dataRatio = -1;

    if (statistic_totalData != 0)
    {
        dataRatio = (statistic_totalDataChecked * 100) / statistic_totalData;

        if (dataTotalMB < 5)
        {
            dataTotal = QString("%1 KB").arg(dataTotalKB);
        }

        else if (dataTotalGB < 1)
        {
            dataTotal = QString("%1 MB").arg(dataTotalMB);
        }

        else
        {
            dataTotal = QString("%1 GB (%2 MB)").arg(dataTotalGB).arg(dataTotalMB);
        }
    }

    else
    {
        dataTotal = noinfo;
    }

    if (statistic_totalDataChecked != 0)
    {
        if (dataCheckedMB < 5)
        {
            dataChecked = QString("%1 KB").arg(dataCheckedKB);
        }

        else if (dataCheckedGB < 1)
        {
            dataChecked = QString("%1 MB").arg(dataCheckedMB);
        }

        else
        {
            dataChecked = QString("%1 GB (%2 MB)").arg(dataCheckedGB).arg(dataCheckedMB);
        }
    }

    else
    {
        dataChecked = noinfo;
    }

    if (dataRatio == -1)
    {
        dataRatioString = noinfo;
    }

    else if (dataRatio > 100) // Due to a block-size checking, dataChecked may be higher than dataTotal, so it's hidden.
    {
        dataChecked = notready;
        dataTotal = notready;
        dataRatioString = notready;
    }

    else
    {
        dataRatioString = QString("%1 %").arg(dataRatio);
    }

    // SHOW STATISTICS

    dialogStyle_info(ui->action_Statistics->text().remove("&"), tr("<br/><b>Analisi iniziate: </b> %1<br/>"
                                                                   "<b>Analisi terminate: </b> %2<br/>"
                                                                   "<b>Analisi interrotte: </b> %3"
                                                                   "<hr>"
                                                                   "<b>Tempo totale di analisi: </b> %4<br/>"
                                                                   "<b>Dimensione totale file analizzati: </b> %5<br/>"
                                                                   "<b>Dimensione totale file scelti: </b>%6<br/>"
                                                                   "<b>Rapporto percentuale: </b>%7"
                                                                   "<hr>"
                                                                   "<b>Ultimo file analizzato: </b> %8<br/>").arg(QString::number(statistic_checkCounter), QString::number(statistic_checkEnded), QString::number(statistic_checkAborted), totalTime, dataChecked, dataTotal, dataRatioString, lastFile));
}
