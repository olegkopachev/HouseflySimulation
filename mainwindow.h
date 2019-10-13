#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "datamodel.h"
#include "settingsdialog.h"

#include <QMainWindow>
#include <QTextBrowser>

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

protected:
    void updateOverallInfoLabel();

signals:
    void setFieldSizeRequest(int fieldSize);
    void setFlyCapacityRequest(int capacity);
    void addNewFlyRequest(int cellX, int cellY, int stupidity);
    void startRequest();
    void stopRequest();
    void resetRequest();

public slots:
    void addNewFly(int flyID, int cellX, int cellY, int stupidity);
    void moveFly(int flyID, int destCellX, int destCellY);
    void killFly(int flyID);
    void onSimulationStopped();
    void onModelReset();

private slots:

    void setNewSettings();
    void restoreOldSettings();

    void on_settingsButton_clicked();

    void on_addFlyButton_clicked();

    void on_startStopButton_clicked();

    void on_resetButton_clicked();

    void setAddFlyMode(bool mode);

    void on_statisticsButton_clicked();

private:
    Ui::MainWindow *ui;

    SettingsDialog settingsDialog;

    SettingsDialog::Settings currentSettings;
    QTextBrowser statisticsViewer;

    bool isFieldSizeSet = false;
    bool simulationRunning = false;
    bool isInAddFlyMode = false;
    int flyCounter = 0;
    int deadFlyCounter = 0;

    const DataModel *model;
};
#endif // MAINWINDOW_H
