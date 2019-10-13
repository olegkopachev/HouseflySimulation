#include "mainwindow.h"
#include "datamodel.h"
#include "fliescontroller.h"

#include <QApplication>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DataModel model;
    model.setFieldSize(6);
    model.setFlyCapacity(10);

    QThread fliesControllerThread;
    FliesController fliesController;
    fliesController.setModel(&model);
    fliesController.moveToThread(&fliesControllerThread);
    fliesControllerThread.start();

    MainWindow w;

    QObject::connect(&w, SIGNAL(addNewFlyRequest(int, int)), &fliesController, SLOT(addFly(int, int)));
    QObject::connect(&model, SIGNAL(flyAdded(int, int, int)), &w, SLOT(addNewFly(int, int, int)));
    w.show();
    return a.exec();
}
