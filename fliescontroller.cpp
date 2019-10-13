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
    while(!flyTasks.isEmpty())
    {
        fliesThreadPool.start(flyTasks.first());
        flyTasks.pop_front();
    }
}

void FliesController::addFly(int cellX, int cellY, int stupidity)
{
    if(model == nullptr)
        return;

    static int counter = 0;
    model->addNewFly(counter, cellX, cellY, stupidity);
    SingleFlyTask* newFlyTask = new SingleFlyTask(counter, cellX, cellY, 6, stupidity, 60000, model);
    counter++;
    flyTasks.push_back(newFlyTask);
}
