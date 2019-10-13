#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QMutex>
#include <QMap>
#include <QPair>

class DataModel : public QObject
{
    Q_OBJECT

public:
    DataModel();
    ~DataModel();

    void setFieldSize(int value);
    void setFlyCapacity(int value);

    void addNewFly(int cellX, int cellY);
    bool tryToMove(int flyID, int destCellX, int destCellY);

protected:
    inline int index(int x, int y);

signals:
    void flyAdded(int flyID, int cellX, int cellY);
    void flyMoved(int flyID, int destCellX, int destCellY);

protected:
    int fieldSize = 0;
    int flyCapacity = 0;
    QMap<int, QList<int>> fliesByCells;
    QMap<int, int> cellsByFlies;
    QMap<int, QMutex*> mutexes;
};

#endif // DATAMODEL_H
