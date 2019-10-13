#include "singleflytask.h"

#include <QThread>

SingleFlyTask::SingleFlyTask(int flyID, int startCellX, int startCellY, int fieldSize, int flyStupidity, int lifeSpan, DataModel *dataModel) : QRunnable()
{
    ID = flyID;
    currentCellX = startCellX;
    currentCellY = startCellY;
    sizeOfField = fieldSize;
    stupidity = flyStupidity;
    lifetime = lifeSpan;
    model = dataModel;
    randomGenerator = new QRandomGenerator(quint32(QTime::currentTime().msecsSinceStartOfDay()));
}

SingleFlyTask::~SingleFlyTask()
{
    delete randomGenerator;
}

void SingleFlyTask::run()
{
    timeConter.start();
    cellsPassed = 0;

    while(!stopFlag && timeConter.elapsed() < lifetime)
    {
        int waitTime = randomGenerator->bounded(stupidity);
        QThread::msleep(unsigned(waitTime));

        if(stopFlag)
            break;

        int destCellCode = randomGenerator->bounded(adjacentCellsCount());
        int destCellX, destCellY;
        getAdjacentCell(destCellCode, &destCellX, &destCellY);

        bool result = model->tryToMove(ID, destCellX, destCellY);
        if(result)
        {
            currentCellX = destCellX;
            currentCellY = destCellY;
            cellsPassed++;
        }
    }
}

void SingleFlyTask::stopRequest()
{
    stopFlag = true;
}

int SingleFlyTask::adjacentCellsCount()
{
    int result = 8; // в обычной ситуации у нас 8 соседних клеток

    // если по одной координате мы находимся на границе поля, то число соседних клеток уже не может быть больше 5
    if(currentCellX == 0 || currentCellX == sizeOfField - 1)
        result = 5;

    // проверка, не находимся ли мы по второй координате на границе поля
    if(currentCellY == 0 || currentCellY == sizeOfField - 1)
    {
        if(result == 5)   // в этой ситуации мы находимся в углу поля, поэтому соседних клеток всего 3
            result = 3;
        else              // тут мы находимся не в углу поля, но на границе, поэтому соседних клеток 5
            result = 5;
    }
    return result;
}

void SingleFlyTask::getAdjacentCell(int cellCode, int *destCellX, int *destCellY)
{
    static int defaultDirections[] = {-1, -1,
                                      -1,  0,
                                      -1,  1,
                                       0, -1,
                                       0,  1,
                                       1, -1,
                                       1,  0,
                                       1,  1};

    static int leftEdgeDirections[] = { 0, -1,
                                        0,  1,
                                        1, -1,
                                        1,  0,
                                        1,  1};

    static int rightEdgeDirections[] = {-1, -1,
                                        -1,  0,
                                        -1,  1,
                                         0, -1,
                                         0,  1};

    static int topEdgeDirections[] = {-1,  0,
                                      -1,  1,
                                       0,  1,
                                       1,  0,
                                       1,  1};

    static int bottomEdgeDirections[] = {-1, -1,
                                         -1,  0,
                                          0, -1,
                                          1, -1,
                                          1,  0};

    static int topLeftCornerDirections[] = { 0,  1,
                                             1,  0,
                                             1,  1};

    static int topRightCornerDirections[] = {-1,  0,
                                             -1,  1,
                                              0,  1};

    static int bottomLeftCornerDirections[] = { 0, -1,
                                                1, -1,
                                                1,  0};

    static int bottomRightCornerDirections[] = {-1, -1,
                                                -1,  0,
                                                 0, -1};

    if(currentCellX == 0 && currentCellY == 0)
    {
        *destCellX = currentCellX + topLeftCornerDirections[cellCode * 2];
        *destCellY = currentCellY + topLeftCornerDirections[cellCode * 2 + 1];
    }
    else if(currentCellX == 0 && currentCellY == sizeOfField - 1)
    {
        *destCellX = currentCellX + bottomLeftCornerDirections[cellCode * 2];
        *destCellY = currentCellY + bottomLeftCornerDirections[cellCode * 2 + 1];
    }
    else if(currentCellX == sizeOfField - 1 && currentCellY == 0)
    {
        *destCellX = currentCellX + topRightCornerDirections[cellCode * 2];
        *destCellY = currentCellY + topRightCornerDirections[cellCode * 2 + 1];
    }
    else if(currentCellX == sizeOfField - 1 && currentCellY == sizeOfField - 1)
    {
        *destCellX = currentCellX + bottomRightCornerDirections[cellCode * 2];
        *destCellY = currentCellY + bottomRightCornerDirections[cellCode * 2 + 1];
    }
    else if(currentCellX == 0)
    {
        *destCellX = currentCellX + leftEdgeDirections[cellCode * 2];
        *destCellY = currentCellY + leftEdgeDirections[cellCode * 2 + 1];
    }
    else if(currentCellX == sizeOfField - 1)
    {
        *destCellX = currentCellX + rightEdgeDirections[cellCode * 2];
        *destCellY = currentCellY + rightEdgeDirections[cellCode * 2 + 1];
    }
    else if(currentCellY == 0)
    {
        *destCellX = currentCellX + topEdgeDirections[cellCode * 2];
        *destCellY = currentCellY + topEdgeDirections[cellCode * 2 + 1];
    }
    else if(currentCellY == sizeOfField - 1)
    {
        *destCellX = currentCellX + bottomEdgeDirections[cellCode * 2];
        *destCellY = currentCellY + bottomEdgeDirections[cellCode * 2 + 1];
    }
    else
    {
        *destCellX = currentCellX + defaultDirections[cellCode * 2];
        *destCellY = currentCellY + defaultDirections[cellCode * 2 + 1];
    }
}
