#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QCryptographicHash>
#include <QFile>

#include "dialogstyle.h"
#include "comparechecksums.h"

/*
 * A primordial version of the program.
 * More functions coming soon, just stay tuned.
 * Please.
 */

bool aborted; // I know, this is ugly.

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->checkingBar->setVisible(false);
    ui->abortButton->setVisible(false);
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
    ui->fileSelectLabel->setEnabled(false);
    ui->fileSelectBrowse->setEnabled(false);
    ui->comparationCheck->setEnabled(false);
    ui->comparationString->setEnabled(false);
}

void MainWindow::enableAll()
{
    ui->startCheck->setEnabled(true);
    ui->fileSelectLabel->setEnabled(true);
    ui->fileSelectBrowse->setEnabled(true);
    ui->comparationCheck->setEnabled(true);
    ui->comparationString->setEnabled(true);
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
        aborted = false;
        while(!fileSelected.atEnd())
        {
            checkProcess.addData(fileSelected.read(byteLoad));
            QCoreApplication::processEvents();
            ui->checkInfo->setText(tr("Analisi dell'impronta del file selezionato in corso..."));

            ui->checkingBar->setVisible(true);
            ui->abortButton->setVisible(true);

            disableAll();

            if (aborted == true)
            {
                break;
            }

        }

        enableAll();

        ui->checkingBar->setVisible(false);
        ui->abortButton->setVisible(false);
        QByteArray md5Data = checkProcess.result();

        if (aborted == false)
        {
            ui->checkInfo->setText(tr("MD5 del File scelto: ") + md5Data.toHex() );

            if (ui->comparationCheck->isChecked())
            {
                comparationStart(ui->comparationString->text(), md5Data.toHex()); // Check between two string, one inserted by user.
            }
        }

        else
        {
                ui->checkInfo->setText(tr("Operazione annullata dall'utente."));
        }

        }

void MainWindow::on_abortButton_clicked()
{
    aborted = true;
}
