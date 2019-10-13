#include "fliescontroller.h"

FliesController::FliesController()
{
    fliesThreadPool.setMaxThreadCount(50);
}

FliesController::~FliesController()
{

}

void FliesController::setModel(DataModel *newModel)
{
    model = newModel;
}

void FliesController::startSimulation()
{
    isRunning = true;
    while(!waitingFlyTasks.isEmpty())
    {
        runningFlyTasks.push_back(waitingFlyTasks.first());
        fliesThreadPool.start(waitingFlyTasks.first());
        waitingFlyTasks.pop_front();
    }
}

void FliesController::stopSimulation()
{
    foreach (SingleFlyTask* task, runningFlyTasks)
    {
        task->stopRequest();
    }
    foreach (SingleFlyTask* task, waitingFlyTasks)
    {
        delete task;
    }
    waitingFlyTasks.clear();

    fliesThreadPool.waitForDone();

    foreach (SingleFlyTask* task, runningFlyTasks)
    {
        delete task;
    }
    runningFlyTasks.clear();
    isRunning = false;

    emit simulationStopped();
}

void FliesController::addFly(int cellX, int cellY, int stupidity)
{
    if(model == nullptr)
        return;

    static int counter = 0;
    model->addNewFly(counter, cellX, cellY, stupidity);
    SingleFlyTask* newFlyTask = new SingleFlyTask(counter, cellX, cellY, 6, stupidity, model);
    counter++;
    if(isRunning)
    {
        runningFlyTasks.push_back(newFlyTask);
        fliesThreadPool.start(newFlyTask);
    }
    else
        waitingFlyTasks.push_back(newFlyTask);
}
