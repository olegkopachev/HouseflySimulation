#include "datamodel.h"

DataModel::DataModel()
{

}

DataModel::~DataModel()
{
    for(auto it = mutexes.begin(); it != mutexes.end(); it++)
    {
        delete (*it);
    }
}

void DataModel::setFieldSize(int value)
{
    fieldSize = value;
    cellsInfo.clear();
    mutexes.clear();
    for(int i = 0; i < fieldSize; i++)
        for(int j = 0; j < fieldSize; j++)
        {
            cellsInfo.insert(index(i, j), QList<int>());
            mutexes.insert(index(i, j), new QMutex());
        }
}

void DataModel::setFlyCapacity(int value)
{
    flyCapacity = value;
}

void DataModel::addNewFly(int flyID, int cellX, int cellY, int stupidity)
{
    int cellIndex = index(cellX, cellY);
    bool success = false;

    mutexes[cellIndex]->lock();

    if(cellsInfo[cellIndex].size() < flyCapacity)
    {
        fliesInfo[flyID] = FlyInformation(cellIndex);
        cellsInfo[cellIndex].push_back(flyID);
        success = true;
    }

    mutexes[cellIndex]->unlock();

    if(success == true)
        emit flyAdded(flyID, cellX, cellY, stupidity);
}

bool DataModel::tryToMove(int flyID, int destCellX, int destCellY)
{
    bool result;

    int sourceCellIndex = fliesInfo[flyID].cellIndex;
    int destCellIndex = index(destCellX, destCellY);

    mutexes[qMin(sourceCellIndex, destCellIndex)]->lock();
    mutexes[qMax(sourceCellIndex, destCellIndex)]->lock();

    if(cellsInfo[destCellIndex].size() >= flyCapacity)
        result = false;
    else
    {
        cellsInfo[sourceCellIndex].removeOne(flyID);
        cellsInfo[destCellIndex].push_back(flyID);
        fliesInfo[flyID].cellIndex = destCellIndex;
        result = true;
    }

    mutexes[qMin(sourceCellIndex, destCellIndex)]->unlock();
    mutexes[qMax(sourceCellIndex, destCellIndex)]->unlock();

    if(result == true)
        emit flyMoved(flyID, destCellX, destCellY);

    return result;
}

void DataModel::killFly(int flyID)
{
    fliesInfo[flyID].isAlive = false;
    emit flyDied(flyID);
}

void DataModel::setFlyLifeAndRouteInfo(int flyID, int lifetime, int cellsPassed)
{
    fliesInfo[flyID].lifetime = lifetime;
    fliesInfo[flyID].cellsPassed = cellsPassed;
}

const QMap<int, DataModel::FlyInformation> &DataModel::getFliesInfo() const
{
    return fliesInfo;
}

inline int DataModel::index(int x, int y)
{
    return x * fieldSize + y;
}

DataModel::FlyInformation::FlyInformation(int startCellIndex) :
    cellIndex(startCellIndex), isAlive(true), lifetime(0), cellsPassed(0)
{
}
