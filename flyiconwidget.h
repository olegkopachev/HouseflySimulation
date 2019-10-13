#ifndef FLYICONWIDGET_H
#define FLYICONWIDGET_H

#include "datamodel.h"

#include <QLabel>

class FlyIconWidget : public QLabel
{
    Q_OBJECT

public:
    explicit FlyIconWidget(int flyID, int stupidity, int maxStupidity, QWidget *parent = nullptr);
    ~FlyIconWidget();

    void drawDeadFly();
    void setFlyInfo(const DataModel::FlyInformation &flyInfo);

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;

protected:
    int ID = 0;
    bool hasInfo = false;
    bool isAlive = true;
    int lifetime = 0;
    int cellsPassed = 0;
};

#endif // FLYICONWIDGET_H
