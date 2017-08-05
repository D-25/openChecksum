#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCryptographicHash>
#include <QMainWindow>

#ifdef Q_OS_WIN32
    #include <QtWinExtras/QWinTaskbarButton>
    #include <QtWinExtras/QWinTaskbarProgress>
#endif

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
    void loadStatistics();

    void on_action_Settings_triggered();

    void on_action_Open_triggered();

    void on_action_Check_triggered();

    void on_action_OpenCompare_triggered();

    void on_action_Exit_triggered();

    void on_action_Statistics_triggered();


    void on_action_D25Website_triggered();

    void on_action_D25FacebookPage_triggered();

    void on_action_About_triggered();

    void on_action_Github_triggered();

    void on_action_Sourceforge_triggered();

private:
    Ui::MainWindow *ui;

#ifdef Q_OS_WIN32
    QWinTaskbarButton *taskbarButton;
    QWinTaskbarProgress *taskbarProgress;
#endif

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // MAINWINDOW_H
