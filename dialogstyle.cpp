#include "dialogstyle.h"
#include <iostream>
#include <QMessageBox>
#include <QString>

// This functions is used to reduce code when Md5Checker need to call a MessageBox. Style shows a big blue title
// with no icon.

void dialogStyle_info(QString title, QString message)
{

    // Simple dialog.

    QMessageBox messageBox;
    messageBox.setText(QString("<span style=\"color:#006699;\"><span style=\"font-size:20px;\">%1</span></span><br/>%2").arg(title, message));
    messageBox.setIcon(QMessageBox::NoIcon);
    messageBox.exec();
}


int dialogStyle_question(QString title, QString message, QString button_Yes, QString button_No)
{
    // Show the message, then return 0 if user pressed NO in the dialog, else it return 1.

    /* Usage:
     *
     * if (dialogStyle_question("title", "message", "yesbuttontext", "nobuttontext") == 1)
     * {
     *      do something.
     * }
     */


    int yesSelected = 0;

    QMessageBox messageBox;
    messageBox.setText(QString("<span style=\"color:#006699;\"><span style=\"font-size:20px;\">%1</span></span><br/>%2").arg(title, message));
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setButtonText(QMessageBox::Yes, button_Yes);
    messageBox.setButtonText(QMessageBox::No, button_No);
    messageBox.setDefaultButton(QMessageBox::Yes);
#ifdef Q_OS_MAC
    messageBox.setIcon(QMessageBox::Warning);
#else
    messageBox.setIcon(QMessageBox::Question);
#endif

    if (messageBox.exec() == QMessageBox::Yes)
    {
        yesSelected = 1;
    }

    return yesSelected;
}
