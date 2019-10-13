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
    fliesByCells.clear();
    mutexes.clear();
    for(int i = 0; i < fieldSize; i++)
        for(int j = 0; j < fieldSize; j++)
        {
            fliesByCells.insert(index(i, j), QList<int>());
            mutexes.insert(index(i, j), new QMutex());
        }
}

void DataModel::setFlyCapacity(int value)
{
    flyCapacity = value;
}

void DataModel::addNewFly(int cellX, int cellY)
{
    int cellIndex = index(cellX, cellY);
    bool success = false;
    int newFlyID = -1;

    mutexes[cellIndex]->lock();

    if(fliesByCells[cellIndex].size() < flyCapacity)
    {
        int newFlyID = cellsByFlies.size();
        cellsByFlies[newFlyID] = cellIndex;
        fliesByCells[cellIndex].push_back(newFlyID);
        success = true;
    }

    mutexes[cellIndex]->unlock();

    if(success == true)
        emit flyAdded(newFlyID, cellX, cellY);
}

bool DataModel::tryToMove(int flyID, int destCellX, int destCellY)
{
    bool result;

    int sourceCellIndex = cellsByFlies[flyID];
    int destCellIndex = index(destCellX, destCellY);

    mutexes[qMin(sourceCellIndex, destCellIndex)]->lock();
    mutexes[qMax(sourceCellIndex, destCellIndex)]->lock();

    if(fliesByCells[destCellIndex].size() >= flyCapacity)
        result = false;
    else
    {
        fliesByCells[sourceCellIndex].removeOne(flyID);
        fliesByCells[destCellIndex].push_back(flyID);
        cellsByFlies[flyID] = destCellIndex;
        result = true;
    }

    mutexes[qMin(sourceCellIndex, destCellIndex)]->unlock();
    mutexes[qMax(sourceCellIndex, destCellIndex)]->unlock();

    if(result == true)
        emit flyMoved(flyID, destCellX, destCellY);

    return result;
}

inline int DataModel::index(int x, int y)
{
    return x * fieldSize + y;
}
