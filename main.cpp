#include "mainwindow.h"
#include "datamodel.h"
#include "fliescontroller.h"

#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // model
    DataModel model;

    // controller
    QThread fliesControllerThread;
    FliesController fliesController;
    fliesController.setModel(&model);
    fliesController.moveToThread(&fliesControllerThread);
    QObject::connect(&fliesController, SIGNAL(quitRequest()), &fliesControllerThread, SLOT(quit()));
    fliesControllerThread.start();

    // view
    MainWindow w;
    w.setModel(&model);

    // связи между model, view и controller
    QObject::connect(&w, SIGNAL(setFieldSizeRequest(int)), &fliesController, SLOT(setFieldSize(int)));
    QObject::connect(&w, SIGNAL(setFlyCapacityRequest(int)), &fliesController, SLOT(setFlyCapacity(int)));
    QObject::connect(&w, SIGNAL(addNewFlyRequest(int, int, int)), &fliesController, SLOT(addFly(int, int, int)));
    QObject::connect(&w, SIGNAL(startRequest()), &fliesController, SLOT(startSimulation()));
    QObject::connect(&w, SIGNAL(stopRequest()), &fliesController, SLOT(stopSimulation()));
    QObject::connect(&w, SIGNAL(resetRequest()), &fliesController, SLOT(reset()));
    QObject::connect(&fliesController, SIGNAL(simulationStopped()), &w, SLOT(onSimulationStopped()));
    QObject::connect(&model, SIGNAL(flyAdded(int, int, int, int)), &w, SLOT(addNewFly(int, int, int, int)));
    QObject::connect(&model, SIGNAL(flyMoved(int, int, int)), &w, SLOT(moveFly(int, int, int)));
    QObject::connect(&model, SIGNAL(flyDied(int)), &w, SLOT(killFly(int)));
    QObject::connect(&model, SIGNAL(modelReset()), &w, SLOT(onModelReset()));

    w.show();

    int result = a.exec();

    fliesController.quit();
    fliesControllerThread.wait();

    return result;
}
