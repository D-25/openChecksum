#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
