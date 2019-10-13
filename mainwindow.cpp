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
    ui->flySimulationWidget->addNewFly(flyID, cellX, cellY, stupidity, 100);
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
    const QMap<int, DataModel::FlyInformation> &fliesInfo = model->getFliesInfo();
    ui->flySimulationWidget->setFliesInfo(fliesInfo);
}

void MainWindow::setNewSettings()
{
    ui->flySimulationWidget->setFieldSize(settingsDialog.fieldSize());

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
    ui->flySimulationWidget->activateAddFlyMode(true);
    ui->hintLabel->setText("Выберите клетку, в которую хотите посадить новую муху, или нажмите кнопку \"Отмена добавления мухи\" для отказа от действия");
}

void MainWindow::on_startStopButton_clicked()
{
    if(!simulationRunning)
    {
        simulationRunning = true;
        ui->startStopButton->setText(tr("Стоп"));
        emit startRequest();
    }
    else
    {
        simulationRunning = false;
        ui->startStopButton->setText(tr("Старт"));
        emit stopRequest();
    }
}
