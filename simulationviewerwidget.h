#ifndef SIMULATIONVIEWERWIDGET_H
#define SIMULATIONVIEWERWIDGET_H

#include "flyiconwidget.h"
#include "datamodel.h"

#include <QFrame>
#include <QPair>
#include <QList>
#include <QMap>
#include <QPropertyAnimation>
#include <QRandomGenerator>


class SimulationViewerWidget : public QFrame
{
    Q_OBJECT

    struct FlyEnvelope
    {
        int cellX;
        int cellY;
        QPropertyAnimation *animation;
        FlyIconWidget *flyWidget;
    };

public:
    explicit SimulationViewerWidget(QWidget *parent = nullptr);
    ~SimulationViewerWidget();

    void setFieldSize(int value);
    void addNewFly(int flyID, int x, int y, int stupidity, int maxStupidity);
    void activateAddFlyMode(bool activate);
    void moveFly(int flyID, int destCellX, int destCellY);
    void killFly(int flyID);
    void setFliesInfo(const QMap<int, DataModel::FlyInformation> &fliesInfo);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;

    QPair<int, int> coordsToCell(int x, int y);
    QPair<int, int> coordsForNewFlyInCell(int cellX, int cellY);

signals:
    void addNewFlyRequest(int cellX, int cellY, int stupidity);

private:
    int fieldSize = 0;
    int cellWidth = 0;
    int cellHeight = 0;
    int cellFlyCapacityX = 0;
    int cellFlyCapacityY = 0;
    bool isInAddFlyMode = false;
    bool enterStupidityManually = true;
    QRandomGenerator randomGenerator;
    QMap<int, FlyEnvelope> flies;
    QMap<QPair<int, int>, QList<int>> fliesByCells;
};

#endif // SIMULATIONVIEWERWIDGET_H
