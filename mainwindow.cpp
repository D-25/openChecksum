#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QCryptographicHash>
#include <QFile>

#include "dialogstyle.h"

/*
 * A primordial version of the program.
 * More functions coming soon, just stay tuned.
 * Please.
 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_fileSelectBrowse_clicked()
{
    // Open a File Browse dialog and, if a file was selected, the button to start the check become available.

    QString stringFileName = QFileDialog::getOpenFileName(this, tr("Apri"), "", tr("Tutti (*.*)"));
    if (stringFileName.isNull() || stringFileName.isEmpty())
    { }
    else
    {
        ui->startCheck->setEnabled(true);
        ui->fileSelectLocation->setText(stringFileName);
    }
}

void MainWindow::on_startCheck_clicked()
{

    // TODO: better code and more functions.

    // Select file, initialize the byteload and start processing the file, disabling the main window for security purpose.
    // Reached the ending of file, the checksum is showed to user.

    QString fileName = ui->fileSelectLocation->text();
    QFile fileSelected(fileName);

    int byteLoad = 262144;

    QCryptographicHash checkProcess(QCryptographicHash::Md5);

        fileSelected.open(QFile::ReadOnly);
        while(!fileSelected.atEnd())
        {
            checkProcess.addData(fileSelected.read(byteLoad));
            QCoreApplication::processEvents();
            ui->checkInfo->setText(tr("Analisi dell'impronta del file selezionato in corso..."));
            this->setEnabled(false);
        }

        this->setEnabled(true);
        QByteArray md5Data = checkProcess.result();

        ui->checkInfo->setText(tr("MD5 del File scelto: ") + md5Data.toHex() );
}
