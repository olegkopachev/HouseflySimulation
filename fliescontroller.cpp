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

void FliesController::addFly(int cellX, int cellY)
{
    if(model == nullptr)
        return;

    model->addNewFly(cellX, cellY);
}
