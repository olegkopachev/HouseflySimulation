#ifndef SINGLEFLYTASK_H
#define SINGLEFLYTASK_H

#include "datamodel.h"

#include <QRunnable>
#include <QRandomGenerator>
#include <QTime>

class SingleFlyTask : public QRunnable
{
public:
    SingleFlyTask(int flyID, int startCellX, int startCellY, int fieldSize, int flyStupidity, int lifeSpan, DataModel *dataModel);
    ~SingleFlyTask();

    virtual void run() override;

    void stopRequest();

protected:
    int adjacentCellsCount();
    void getAdjacentCell(int cellCode, int *destCellX, int *destCellY);

protected:
    int ID;
    int currentCellX;
    int currentCellY;
    int sizeOfField;
    int stupidity;
    int lifetime;
    int cellsPassed = 0;
    DataModel *model;
    QRandomGenerator *randomGenerator;
    QTime timeConter;

    bool stopFlag = false;
};

#endif // SINGLEFLYTASK_H
