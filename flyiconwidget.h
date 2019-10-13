#ifndef FLYICONWIDGET_H
#define FLYICONWIDGET_H

#include "datamodel.h"

#include <QLabel>

/**
 * @brief The FlyIconWidget class Класс виджета, отображающего муху на поле
 */
class FlyIconWidget : public QLabel
{
    Q_OBJECT

public:
    explicit FlyIconWidget(int flyID, double stupidityPercent, QString iconName, QWidget *parent = nullptr);
    ~FlyIconWidget();

    void setIcon(QString iconName);
    void drawDeadFly();
    void setFlyInfo(const DataModel::FlyInformation &flyInfo);

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;

protected:
    int ID = 0;
    double stupidityRate = 0.0;
    bool hasInfo = false;
    bool isAlive = true;
    int lifetime = 0;
    int cellsPassed = 0;
};

#endif // FLYICONWIDGET_H
