#include "about.h"
#include "ui_about.h"

about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);

    ui->label_AppVer->setText(QString("<h1>%1 <sup>%2</sup></h1>").arg(QApplication::applicationName(), QApplication::applicationVersion()));
    this->setWindowTitle(tr("Informazioni su %1").arg(QApplication::applicationName()));
}

about::~about()
{
    delete ui;
}
