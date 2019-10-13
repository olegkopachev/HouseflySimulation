#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "settingsdialog.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void addNewFlyRequest(int cellX, int cellY, int stupidity);
    void startRequest();

public slots:
    void addNewFly(int flyID, int cellX, int cellY, int stupidity);
    void moveFly(int flyID, int destCellX, int destCellY);

private slots:

    void setNewSettings();

    void on_settingsButton_clicked();

    void on_addFlyButton_clicked();

    void on_startStopButton_clicked();

private:
    Ui::MainWindow *ui;

    SettingsDialog settingsDialog;
};
#endif // MAINWINDOW_H
