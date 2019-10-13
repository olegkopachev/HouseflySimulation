#ifndef SIMULATIONVIEWERWIDGET_H
#define SIMULATIONVIEWERWIDGET_H

#include "flyiconwidget.h"

#include <QFrame>
#include <QPair>
#include <QList>
#include <QMap>
#include <QPropertyAnimation>


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
    void addNewFly(int flyID, int x, int y, unsigned int stupidity, unsigned int maxStupidity);
    void activateAddFlyMode(bool activate);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;

    QPair<int, int> coordsToCell(int x, int y);
    QPair<int, int> coordsForNewFlyInCell(int cellX, int cellY);

signals:
    void addNewFlyRequest(int cellX, int cellY);

public slots:
    void moveFly(int flyID, int destCellX, int destCellY);

private:
    int fieldSize = 0;
    int cellWidth = 0;
    int cellHeight = 0;
    int cellFlyCapacityX = 0;
    int cellFlyCapacityY = 0;
    bool isInAddFlyMode = false;
    QMap<int, FlyEnvelope> flies;
    QMap<QPair<int, int>, QList<int>> fliesByCells;
};

#endif // SIMULATIONVIEWERWIDGET_H
