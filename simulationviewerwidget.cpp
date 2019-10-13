#include "simulationviewerwidget.h"
#include "flyiconwidget.h"
#include "stupiditysettingdialog.h"
#include "defines.h"

#include <QPainter>
#include <QLayout>
#include <QMouseEvent>

SimulationViewerWidget::SimulationViewerWidget(QWidget *parent) :
    QFrame(parent)
{
    setStyleSheet("QFrame { background-image: url(:/images/background2.jpg); }");
    currentIconName = QString(":/images/fly1.png");
}

SimulationViewerWidget::~SimulationViewerWidget()
{
    for(auto it = flies.begin(); it != flies.end(); it++)
    {
        delete (*it).animation;
        delete (*it).flyWidget;
    }
}

void SimulationViewerWidget::setBackgroundColor(int index)
{
    if(index == 0)
        setStyleSheet("QFrame { background-image: default; }");
    else
        setStyleSheet(QString("QFrame { background-image: url(:/images/background%1.jpg); }").arg(index));
}

void SimulationViewerWidget::setFlyIcon(int index)
{
    currentIconName = QString(":/images/fly%1.png").arg(index + 1);
    for(auto it = flies.begin(); it != flies.end(); it++)
    {
        (*it).flyWidget->setIcon(currentIconName);
    }
}

void SimulationViewerWidget::setFieldSize(int value)
{
    fieldSize = value;
    cellWidth = (width() - 10) / fieldSize;
    cellHeight = (height() - 10) / fieldSize;
    cellFlyCapacityX = cellWidth / FLY_ICON_WIDTH;
    if(cellFlyCapacityX == 0)
        cellFlyCapacityX++;
    cellFlyCapacityY = cellHeight / FLY_ICON_HEIGHT;
    if(cellFlyCapacityY == 0)
        cellFlyCapacityY++;
    for(int i = 0; i < fieldSize; i++)
        for(int j = 0; j < fieldSize; j++)
            fliesByCells[QPair<int,int>(i, j)] = QList<int>();
    repaint();
}

void SimulationViewerWidget::addNewFly(int flyID, int x, int y, int stupidity)
{
    FlyEnvelope newFly;
    newFly.cellX = x;
    newFly.cellY = y;
    newFly.flyWidget = new FlyIconWidget(flyID, double(stupidity) / maxStupidity, currentIconName, this);
    newFly.animation = new QPropertyAnimation();
    newFly.animation->setTargetObject(newFly.flyWidget);
    newFly.animation->setPropertyName("geometry");
    newFly.animation->setDuration(500);
    flies[flyID] = newFly;
    QPair<int, int> flyCoordinates = coordsForNewFlyInCell(x, y);
    fliesByCells[QPair<int, int>(x, y)].push_back(flyID);
    flies.last().flyWidget->move(flyCoordinates.first, flyCoordinates.second);
    flies.last().flyWidget->setVisible(true);
}

void SimulationViewerWidget::activateAddFlyMode(bool activate)
{
    isInAddFlyMode = activate;
    if(activate)
        setCursor(QCursor(QPixmap(":/images/target.png")));
    else
        setCursor(QCursor());

    emit flyModeChanged(activate);
}

void SimulationViewerWidget::paintEvent(QPaintEvent *event)
{
    if(fieldSize == 0)
        return;

    QPainter painter(this);
    painter.setPen(QPen(QBrush(QColor(Qt::black)), 3));
    painter.drawRect(5, 5, width() - 10, height() - 10);
    for(int i = 0; i < fieldSize - 1; i++)
    {
        painter.drawLine(5 + (i + 1) * (width() - 10) / fieldSize, 5, 5 + (i + 1) * (width() - 10) / fieldSize, height() - 5);
        painter.drawLine(5, 5 + (i + 1) * (height() - 10) / fieldSize, width() - 5, 5 + (i + 1) * (height() - 10) / fieldSize);
    }
}

void SimulationViewerWidget::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    if(fieldSize > 0)
    {
        cellWidth = (width() - 10) / fieldSize;
        cellHeight = (height() - 10) / fieldSize;
        cellFlyCapacityX = cellWidth / FLY_ICON_WIDTH;
        if(cellFlyCapacityX == 0)
            cellFlyCapacityX++;
        cellFlyCapacityY = cellHeight / FLY_ICON_HEIGHT;
        if(cellFlyCapacityY == 0)
            cellFlyCapacityY++;

        for(auto it = fliesByCells.begin(); it != fliesByCells.end(); it++)
            (*it).clear();

        for(int i = 0; i < flies.size(); i++)
        {
            QPair<int, int> flyCoordinates = coordsForNewFlyInCell(flies[i].cellX, flies[i].cellY);
            fliesByCells[QPair<int, int>(flies[i].cellX, flies[i].cellY)].push_back(i);
            flies[i].flyWidget->move(flyCoordinates.first, flyCoordinates.second);
        }
    }
}

void SimulationViewerWidget::mousePressEvent(QMouseEvent *event)
{
    if(!isInAddFlyMode)
        return;

    if(event->x() < 5 || event->x() >= width() - 5 || event->y() < 5 || event->y() >= height() - 5)
        return;

    QPair<int, int> cell = coordsToCell(event->x(), event->y());
    int stupidity;
    if(enterStupidityManually)
    {
        StupiditySettingDialog dialog;
        dialog.setStupidityRange(5, maxStupidity);
        int result = dialog.exec();
        if(result == QDialog::Accepted)
            stupidity = dialog.getStupidity();
        else
            return;
    }
    else
        stupidity = 5 + randomGenerator.bounded(maxStupidity - 5);

    emit addNewFlyRequest(cell.first, cell.second, stupidity);
    activateAddFlyMode(false);
}

QPair<int, int> SimulationViewerWidget::coordsToCell(int x, int y)
{
    int cellX = (x - 5) / cellWidth;
    int cellY = (y - 5) / cellHeight;
    return QPair<int, int>(cellX, cellY);
}

QPair<int, int> SimulationViewerWidget::coordsForNewFlyInCell(int cellX, int cellY)
{
    int cellLeft = 5 + cellX * cellWidth;
    int cellTop = 5 + cellY * cellHeight;
    int flyCountInCell = fliesByCells[QPair<int, int>(cellX, cellY)].size();
    flyCountInCell %= cellFlyCapacityX * cellFlyCapacityY;
    int rowForFly = flyCountInCell / cellFlyCapacityX;
    int columnForFly = flyCountInCell - rowForFly * cellFlyCapacityX;
    return QPair<int, int>(cellLeft + columnForFly * FLY_ICON_WIDTH, cellTop + rowForFly * FLY_ICON_HEIGHT);
}

void SimulationViewerWidget::moveFly(int flyID, int destCellX, int destCellY)
{
    QPair<int, int> newCoordinates = coordsForNewFlyInCell(destCellX, destCellY);
    fliesByCells[QPair<int, int>(flies[flyID].cellX, flies[flyID].cellY)].removeOne(flyID);
    flies[flyID].cellX = destCellX;
    flies[flyID].cellY = destCellY;
    fliesByCells[QPair<int, int>(flies[flyID].cellX, flies[flyID].cellY)].push_back(flyID);
    flies[flyID].animation->setStartValue(QRect(flies[flyID].flyWidget->x(), flies[flyID].flyWidget->y(), FLY_ICON_WIDTH, FLY_ICON_HEIGHT));
    flies[flyID].animation->setEndValue(QRect(newCoordinates.first, newCoordinates.second, FLY_ICON_WIDTH, FLY_ICON_HEIGHT));
    flies[flyID].animation->start();
}

void SimulationViewerWidget::killFly(int flyID)
{
    flies[flyID].flyWidget->drawDeadFly();
}

void SimulationViewerWidget::setFliesInfo(const QMap<int, DataModel::FlyInformation> &fliesInfo)
{
    for(auto it = fliesInfo.begin(); it != fliesInfo.end(); it++)
    {
        flies[it.key()].flyWidget->setFlyInfo(it.value());
    }
}

void SimulationViewerWidget::reset()
{
    activateAddFlyMode(false);
    for(auto it = flies.begin(); it != flies.end(); it++)
    {
        (*it).animation->stop();
        delete (*it).animation;
        delete (*it).flyWidget;
    }
    flies.clear();
    for(auto it = fliesByCells.begin(); it != fliesByCells.end(); it++)
        (*it).clear();

    repaint();
}

void SimulationViewerWidget::setAnimationDuration(int duration)
{
    animationDuration = duration;
    for(auto it = flies.begin(); it != flies.end(); it++)
        (*it).animation->setDuration(duration);
}

void SimulationViewerWidget::setManualInputOfStupidity(bool value)
{
    enterStupidityManually = value;
}

void SimulationViewerWidget::setMaxStupidity(int stupidity)
{
    maxStupidity = stupidity;
}
