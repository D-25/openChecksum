#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QDialog>

namespace Ui {
class SettingsPage;
}

class SettingsPage : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsPage(QWidget *parent = 0);
    ~SettingsPage();

private:
    Ui::SettingsPage *ui;
};

#endif // SETTINGSPAGE_H
