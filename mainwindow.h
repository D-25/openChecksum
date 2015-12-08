#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "SettingWindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_fileSelectBrowse_clicked();

    void on_startCheck_clicked();

    void on_abortButton_clicked();
    void disableAll();
    void enableAll();



    void on_SpeedSelect_currentIndexChanged(int index);

    void on_SettingButton_clicked();

private:
    Ui::MainWindow *ui;

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // MAINWINDOW_H
