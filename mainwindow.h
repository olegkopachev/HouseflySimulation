#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "datamodel.h"
#include "settingsdialog.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The MainWindow class Класс главного окна приложения
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setModel(DataModel *dataModel);

signals:
    void addNewFlyRequest(int cellX, int cellY, int stupidity);
    void startRequest();
    void stopRequest();

public slots:
    void addNewFly(int flyID, int cellX, int cellY, int stupidity);
    void moveFly(int flyID, int destCellX, int destCellY);
    void killFly(int flyID);
    void onSimulationStopped();

private slots:

    void setNewSettings();

    void on_settingsButton_clicked();

    void on_addFlyButton_clicked();

    void on_startStopButton_clicked();

private:
    Ui::MainWindow *ui;

    SettingsDialog settingsDialog;

    bool simulationRunning = false;

    const DataModel *model;
};
#endif // MAINWINDOW_H
