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

signals:
    void simulationStopped();

public slots:
    void addFly(int cellX, int cellY, int stupidity);
    void startSimulation();
    void stopSimulation();

protected:
    bool isRunning = false;
    DataModel *model = nullptr;
    QThreadPool fliesThreadPool;
    QList<SingleFlyTask*> waitingFlyTasks;
    QList<SingleFlyTask*> runningFlyTasks;
};

#endif // FLIESCONTROLLER_H
