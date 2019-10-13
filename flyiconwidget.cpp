#include "flyiconwidget.h"
#include "defines.h"

#include <QImage>

FlyIconWidget::FlyIconWidget(int stupidity, int maxStupidity, QWidget *parent) :
    QLabel(parent)
{
    setFixedSize(FLY_ICON_WIDTH, FLY_ICON_HEIGHT);
    setPixmap(QPixmap(":/images/fly.png"));
    setAttribute(Qt::WA_TranslucentBackground);

    QImage image = pixmap()->toImage();
}

FlyIconWidget::~FlyIconWidget()
{
}

void FlyIconWidget::drawDeadFly()
{
    setPixmap(QPixmap(":/images/deadfly.png"));
}
