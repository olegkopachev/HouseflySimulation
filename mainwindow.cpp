#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->overallInfoLabel->setText(tr("Настройки игры не установлены"));
    ui->hintLabel->setText("Для начала симуляции нажмите кнопку \"Настройки\" и задайте параметры");
    ui->startStopButton->setEnabled(false);
    ui->resetButton->setEnabled(false);
    ui->addFlyButton->setEnabled(false);

    connect(ui->flySimulationWidget, SIGNAL(addNewFlyRequest(int, int, int)), this, SIGNAL(addNewFlyRequest(int, int, int)));
    connect(ui->flySimulationWidget, SIGNAL(flyModeChanged(bool)), this, SLOT(setAddFlyMode(bool)));
    connect(&settingsDialog, SIGNAL(accepted()), this, SLOT(setNewSettings()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setModel(DataModel *dataModel)
{
    model = dataModel;
}

void MainWindow::addNewFly(int flyID, int cellX, int cellY, int stupidity)
{
    ui->flySimulationWidget->addNewFly(flyID, cellX, cellY, stupidity);
}

void MainWindow::moveFly(int flyID, int destCellX, int destCellY)
{
    ui->flySimulationWidget->moveFly(flyID, destCellX, destCellY);
}

void MainWindow::killFly(int flyID)
{
    ui->flySimulationWidget->killFly(flyID);
}

void MainWindow::onSimulationStopped()
{
    ui->resetButton->setEnabled(true);
    const QMap<int, DataModel::FlyInformation> &fliesInfo = model->getFliesInfo();
    ui->flySimulationWidget->setFliesInfo(fliesInfo);
}

void MainWindow::onModelReset()
{
    ui->flySimulationWidget->reset();
    ui->startStopButton->setEnabled(true);
    ui->addFlyButton->setEnabled(true);
}

void MainWindow::setNewSettings()
{
    SettingsDialog::Settings settings = settingsDialog.getSettings();

    if(!isFieldSizeSet)
    {
        ui->flySimulationWidget->setFieldSize(settings.fieldSize);
        settingsDialog.disableFieldSizeOption();
        isFieldSizeSet = true;
        emit setFieldSizeRequest(settings.fieldSize);
    }
    emit setFlyCapacityRequest(settings.flyCapacity);

    ui->flySimulationWidget->setMaxStupidity(settings.maxStupidity);
    ui->flySimulationWidget->setManualInputOfStupidity(settings.enterStupidityManually);
    ui->flySimulationWidget->setAnimationDuration(settings.animationDuration);

    ui->startStopButton->setEnabled(true);
    ui->resetButton->setEnabled(true);
    ui->addFlyButton->setEnabled(true);
}


void MainWindow::on_settingsButton_clicked()
{
    settingsDialog.show();
}

void MainWindow::on_addFlyButton_clicked()
{
    if(!isInAddFlyMode)
        ui->flySimulationWidget->activateAddFlyMode(true);
    else
        ui->flySimulationWidget->activateAddFlyMode(false);
}

void MainWindow::on_startStopButton_clicked()
{
    if(!simulationRunning)
    {
        simulationRunning = true;
        ui->startStopButton->setText(tr("Стоп"));
        ui->resetButton->setEnabled(false);
        emit startRequest();
    }
    else
    {
        simulationRunning = false;
        ui->flySimulationWidget->activateAddFlyMode(false);
        ui->startStopButton->setText(tr("Старт"));
        ui->startStopButton->setEnabled(false);
        ui->addFlyButton->setEnabled(false);
        ui->hintLabel->setText(tr("Остановка симуляции..."));
        emit stopRequest();
    }
}

void MainWindow::on_resetButton_clicked()
{
    emit resetRequest();
}

void MainWindow::setAddFlyMode(bool mode)
{
    if(mode)
    {
        isInAddFlyMode = true;
        ui->addFlyButton->setText(tr("Отмена добавления мухи"));
        ui->hintLabel->setText(tr("Выберите клетку, в которую хотите посадить новую муху, или нажмите кнопку \"Отмена добавления мухи\" для отказа от действия"));
    }
    else
    {
        isInAddFlyMode = false;
        ui->addFlyButton->setText(tr("Добавить муху"));
        ui->hintLabel->setText(QString());
    }
}
