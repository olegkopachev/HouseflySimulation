#include "fliescontroller.h"

FliesController::FliesController()
{
    fliesThreadPool.setMaxThreadCount(100);
}

FliesController::~FliesController()
{

}

void FliesController::setModel(DataModel *newModel)
{
    model = newModel;
}

void FliesController::reset()
{
    stopSimulation();
    currentFlyCount = 0;
    model->reset();
}

void FliesController::quit()
{
    stopSimulation();
    emit quitRequest();
}

void FliesController::setFieldSize(int size)
{
    if(isRunning)
        return;

    fieldSize = size;
    model->setFieldSize(size);
}

void FliesController::setFlyCapacity(int capacity)
{
    model->setFlyCapacity(capacity);
}

void FliesController::startSimulation()
{
    isRunning = true;
    while(!waitingFlyTasks.isEmpty())
    {
        runningFlyTasks.push_back(waitingFlyTasks.first());
        if(fliesThreadPool.activeThreadCount() > fliesThreadPool.maxThreadCount() - 2)
            fliesThreadPool.setMaxThreadCount(fliesThreadPool.maxThreadCount() + 50);
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

    int newFlyID = currentFlyCount;
    if(model->addNewFly(newFlyID, cellX, cellY, stupidity))
    {
        currentFlyCount++;
        SingleFlyTask* newFlyTask = new SingleFlyTask(newFlyID, cellX, cellY, fieldSize, stupidity, model);
        if(isRunning)
        {
            runningFlyTasks.push_back(newFlyTask);
            if(fliesThreadPool.activeThreadCount() > fliesThreadPool.maxThreadCount() - 2)
                fliesThreadPool.setMaxThreadCount(fliesThreadPool.maxThreadCount() + 50);
            fliesThreadPool.start(newFlyTask);
        }
        else
            waitingFlyTasks.push_back(newFlyTask);
    }
}
