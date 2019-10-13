#ifndef FLIESCONTROLLER_H
#define FLIESCONTROLLER_H

#include "datamodel.h"

#include <QObject>
#include <QThreadPool>

class FliesController : public QObject
{
    Q_OBJECT

public:
    FliesController();
    ~FliesController();

    void setModel(DataModel *newModel);

signals:
    void flyMoved(int flyID, int destCellX, int destCellY);

public slots:
    void addFly(int cellX, int cellY);

protected:
    bool isRunning = false;
    DataModel *model = nullptr;
    QThreadPool fliesThreadPool;
};

#endif // FLIESCONTROLLER_H
