#include "dialogstyle.h"
#include <iostream>
#include <QMessageBox>

// This functions is used to reduce code when Md5Checker need to call a MessageBox. Style shows a big blue title
// with no icon.

void dialogStyle_info(QString title, QString message)
{

    // Simple dialog.

    QMessageBox messageBox;
    messageBox.setText("<span style=\"color:#006699;\"><span style=\"font-size:20px;\">" + title + "</span></span><br/>" + message);
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
    messageBox.setText("<span style=\"color:#006699;\"><span style=\"font-size:20px;\">" + title + "</span></span><br/>" + message);
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setButtonText(QMessageBox::Yes, button_Yes);
    messageBox.setButtonText(QMessageBox::No, button_No);
    messageBox.setDefaultButton(QMessageBox::Yes);
    messageBox.setIcon(QMessageBox::Question);

    if (messageBox.exec() == QMessageBox::Yes)
    {
        yesSelected = 1;
    }

    return yesSelected;
}
