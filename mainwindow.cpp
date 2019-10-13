#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"

#include <QTextBrowser>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/images/fly6.png"));

    ui->overallInfoLabel->setText(tr("Настройки игры не установлены"));
    ui->hintLabel->setText(tr("Для начала симуляции нажмите кнопку \"Настройки\" и задайте параметры"));
    ui->startStopButton->setEnabled(false);
    ui->resetButton->setEnabled(false);
    ui->addFlyButton->setEnabled(false);
    ui->statisticsButton->setEnabled(false);

    ui->settingsButton->setStyleSheet("QPushButton {background-color: rgb(230, 230, 230);}"
                                       "QPushButton:disabled {background-color: rgb(180, 180, 180);}");
    ui->startStopButton->setStyleSheet("QPushButton {background-color: rgb(255, 150, 150);}"
                                       "QPushButton:disabled {background-color: rgb(180, 180, 180);}");
    ui->resetButton->setStyleSheet("QPushButton {background-color: rgb(255, 80, 80);}"
                                       "QPushButton:disabled {background-color: rgb(180, 180, 180);}");
    ui->addFlyButton->setStyleSheet("QPushButton {background-color: rgb(200, 255, 200);}"
                                    "QPushButton:disabled {background-color: rgb(180, 180, 180);}");
    ui->statisticsButton->setStyleSheet("QPushButton {background-color: rgb(230, 230, 230);}"
                                       "QPushButton:disabled {background-color: rgb(180, 180, 180);}");

    ui->backgroundComboBox->setIconSize(QSize(50, 30));
    for(int i = 1; i <= 6; i++)
        ui->backgroundComboBox->setItemIcon(i, QIcon(QString(":/images/background%1.jpg").arg(i)));

    ui->flyIconComboBox->setIconSize(QSize(50, 30));
    for(int i = 0; i < 8; i++)
        ui->flyIconComboBox->setItemIcon(i, QIcon(QString(":/images/fly%1.png").arg(i + 1)));

    statisticsViewer.setWindowTitle("Статистика по мухам");

    connect(ui->flySimulationWidget, SIGNAL(addNewFlyRequest(int, int, int)), this, SIGNAL(addNewFlyRequest(int, int, int)));
    connect(ui->flySimulationWidget, SIGNAL(flyModeChanged(bool)), this, SLOT(setAddFlyMode(bool)));
    connect(&settingsDialog, SIGNAL(accepted()), this, SLOT(setNewSettings()));
    connect(&settingsDialog, SIGNAL(rejected()), this, SLOT(restoreOldSettings()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setModel(DataModel *dataModel)
{
    model = dataModel;
}

void MainWindow::updateOverallInfoLabel()
{
    QString overallInfo = tr("Размер поля: %1x%1\nМухоёмкость: %2\nОбщее количество мух: %3\nИз них дохлых: %4").
            arg(currentSettings.fieldSize).arg(currentSettings.flyCapacity).arg(flyCounter).arg(deadFlyCounter);
    ui->overallInfoLabel->setText(overallInfo);
}

void MainWindow::addNewFly(int flyID, int cellX, int cellY, int stupidity)
{
    ui->flySimulationWidget->addNewFly(flyID, cellX, cellY, stupidity);
    flyCounter++;
    updateOverallInfoLabel();
}

void MainWindow::moveFly(int flyID, int destCellX, int destCellY)
{
    ui->flySimulationWidget->moveFly(flyID, destCellX, destCellY);
}

void MainWindow::killFly(int flyID)
{
    ui->flySimulationWidget->killFly(flyID);
    deadFlyCounter++;
    updateOverallInfoLabel();
}

void MainWindow::onSimulationStopped()
{
    ui->resetButton->setEnabled(true);
    ui->statisticsButton->setEnabled(true);
    const QMap<int, DataModel::FlyInformation> &fliesInfo = model->getFliesInfo();
    ui->flySimulationWidget->setFliesInfo(fliesInfo);
    ui->hintLabel->setText(tr("Симуляция остановлена. Вы можете посмотреть статистику по всем мухам, нажав на кнопку \"Статистика\" или по каждой отдельной мухе, нажав на её иконку"));
}

void MainWindow::onModelReset()
{
    flyCounter = 0;
    deadFlyCounter = 0;
    ui->flySimulationWidget->reset();
    ui->startStopButton->setEnabled(true);
    ui->addFlyButton->setEnabled(true);
    ui->statisticsButton->setEnabled(false);
    ui->hintLabel->setText(tr("Добавьте на поле мух и нажмите кнопку \"Старт\""));
    updateOverallInfoLabel();
}

void MainWindow::setNewSettings()
{
    currentSettings = settingsDialog.getSettings();

    if(!isFieldSizeSet)
    {
        ui->flySimulationWidget->setFieldSize(currentSettings.fieldSize);
        settingsDialog.disableFieldSizeOption();
        isFieldSizeSet = true;
        ui->hintLabel->setText(tr("Добавьте на поле мух и нажмите кнопку \"Старт\""));
        emit setFieldSizeRequest(currentSettings.fieldSize);

        ui->startStopButton->setEnabled(true);
        ui->resetButton->setEnabled(true);
        ui->addFlyButton->setEnabled(true);
    }
    emit setFlyCapacityRequest(currentSettings.flyCapacity);

    ui->flySimulationWidget->setMaxStupidity(currentSettings.maxStupidity);
    ui->flySimulationWidget->setManualInputOfStupidity(currentSettings.enterStupidityManually);
    ui->flySimulationWidget->setAnimationDuration(currentSettings.animationDuration);

    updateOverallInfoLabel();
}

void MainWindow::restoreOldSettings()
{
    settingsDialog.setSettings(currentSettings);
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
        ui->hintLabel->setText(tr("Идет симуляция поведения мух. Вы можете динамически добавлять мух на поле и изменять все настройки, кроме размера поля. Для остановки симуляции и просмотра статистики нажмите кнопку \"Стоп\""));
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
        if(simulationRunning)
            ui->hintLabel->setText(tr("Идет симуляция поведения мух. Вы можете динамически добавлять мух на поле и изменять все настройки, кроме размера поля. Для остановки симуляции и просмотра статистики нажмите кнопку \"Стоп\""));
        else
            ui->hintLabel->setText(tr("Добавьте на поле мух и нажмите кнопку \"Старт\""));
    }
}

void MainWindow::on_statisticsButton_clicked()
{
    QString statistics;
    const QMap<int, DataModel::FlyInformation> &fliesInfo = model->getFliesInfo();
    if(fliesInfo.isEmpty())
        statistics = tr("Нет информации о мухах");
    for(auto it = fliesInfo.begin(); it != fliesInfo.end(); it++)
    {
        statistics.append(tr("Муха №%1\nЖивая: %2\nПродолжительность жизни: %3 мс\nКлеток пройдено: %4\nСредняя скорость: %5 мс/клетку\n\n").
                           arg(it.key()).
                           arg(it->isAlive ? tr("Да") : tr("Нет")).
                           arg(it->lifetime).
                           arg(it->cellsPassed).
                           arg(it->cellsPassed == 0 ? QString("N/A") : QString::number(double(it->lifetime) / it->cellsPassed)));
    }
    statisticsViewer.setText(statistics);
    statisticsViewer.resize(300, 500);
    statisticsViewer.show();
}

void MainWindow::on_backgroundComboBox_currentIndexChanged(int index)
{
    ui->flySimulationWidget->setBackgroundColor(index);
}

void MainWindow::on_flyIconComboBox_currentIndexChanged(int index)
{
    ui->flySimulationWidget->setFlyIcon(index);
}
