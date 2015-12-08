#include "comparechecksums.h"
#include <iostream>
#include <QObject>

#include "dialogstyle.h"

int comparationStart(QString original, QString checked)
{
    // To keep code clean.

    /*
     * Usage:
     *
     * if (comparationStart(original, checked) == 1)
     * {
     *      do something.
     * }
     *
     */

    int comparationSuccess = 0;

    if (original == checked)
    {
        comparationSuccess = 1;

        dialogStyle_info(QObject::tr("Risultati della comparazione"), QObject::tr("Il controllo è riuscito: i due valori sono uguali."));
    }

    else
    {
       dialogStyle_info(QObject::tr("Risultati della comparazione"), QObject::tr("<b>I due valori non combaciano.</b><br/><br/>Originale: %1<br/>File analizzato: %2<br/><br/>"
                           "Utilizzare questo File potrebbe portare a risultati indesiderati: riscaricare il File se necessario fino a quando il controllo non riesce.").arg(original, checked));
    }

    return comparationSuccess;
}
