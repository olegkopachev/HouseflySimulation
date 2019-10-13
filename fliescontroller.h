#ifndef FLIESCONTROLLER_H
#define FLIESCONTROLLER_H

#include "datamodel.h"
#include "singleflytask.h"

#include <QObject>
#include <QThreadPool>
#include <QList>

class FliesController : public QObject
{
    Q_OBJECT

public:
    FliesController();
    ~FliesController();

    void setModel(DataModel *newModel);
    void quit();

signals:
    void simulationStopped();
    void quitRequest();

public slots:
    void setFieldSize(int size);
    void setFlyCapacity(int capacity);
    void addFly(int cellX, int cellY, int stupidity);
    void startSimulation();
    void stopSimulation();
    void reset();

protected:
    bool isRunning = false;
    int fieldSize = 0;
    int currentFlyCount = 0;
    DataModel *model = nullptr;
    QThreadPool fliesThreadPool;
    QList<SingleFlyTask*> waitingFlyTasks;
    QList<SingleFlyTask*> runningFlyTasks;
};

#endif // FLIESCONTROLLER_H
