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
    struct FlyInformation
    {
        FlyInformation(int cellIndex = 0);

        int cellIndex;
        bool isAlive;
        int lifetime;
        int cellsPassed;
    };

public:
    DataModel();
    ~DataModel();

    void setFieldSize(int value);
    void setFlyCapacity(int value);

    void addNewFly(int flyID, int cellX, int cellY, int stupidity);
    bool tryToMove(int flyID, int destCellX, int destCellY);
    void killFly(int flyID);
    void setFlyLifeAndRouteInfo(int flyID, int lifetime, int cellsPassed);

    const QMap<int, FlyInformation>& getFliesInfo() const;

protected:
    inline int index(int x, int y);

signals:
    void flyAdded(int flyID, int cellX, int cellY, int stupidity);
    void flyMoved(int flyID, int destCellX, int destCellY);
    void flyDied(int flyID);

protected:
    int fieldSize = 0;
    int flyCapacity = 0;
    QMap<int, QList<int>> cellsInfo;
    QMap<int, FlyInformation> fliesInfo;
    QMap<int, QMutex*> mutexes;
};

#endif // DATAMODEL_H
