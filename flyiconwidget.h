#ifndef FLYICONWIDGET_H
#define FLYICONWIDGET_H

#include <QLabel>

class FlyIconWidget : public QLabel
{
    Q_OBJECT

public:
    explicit FlyIconWidget(int stupidity, int maxStupidity, QWidget *parent = nullptr);
    ~FlyIconWidget();

    void drawDeadFly();

private:
};

#endif // FLYICONWIDGET_H
