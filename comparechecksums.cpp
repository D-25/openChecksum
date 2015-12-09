#include "comparechecksums.h"
#include <iostream>
#include <QObject>
#include <QDebug>
#include <QRegularExpression>
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

    /*                          //check if the original string is a correct HEX string - don't work =(
    QRegularExpression hexMatcher("^[0-9A-F]{6}$", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = hexMatcher.match(original);
    if (match.hasMatch())
    {
        dialogStyle_info(QObject::tr("Codice scorretto"), QObject::tr("<b>il codice da verificare non va bene.</b><br/>hai inserito il codice : %1<br/>"
                            "Il codice inserito contiene caratteri non ammessi nel sistema esadecimale, reinserire e controllare il codice.").arg(original));
         return comparationSuccess;
    }
    */


    //check if original string is smaller or bigger than 32 chars
    if(original.length() != 32){
        dialogStyle_info(QObject::tr("Risultati della comparazione"), QObject::tr("<b>il codice da verificare non va bene.</b><br/>hai inserito il codice : %1<br/>"
                            "Non avendo i consueti 32 caratteri, il codice è stato scartato e la comparazione non può essere eseguita, reinserire e controllare il codice.").arg(original));
         return comparationSuccess;
    }

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
