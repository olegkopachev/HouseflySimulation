#include "flyiconwidget.h"
#include "defines.h"

#include <QImage>
#include <QMessageBox>

FlyIconWidget::FlyIconWidget(int flyID, int stupidity, int maxStupidity, QWidget *parent) :
    QLabel(parent)
{
    ID = flyID;

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

void FlyIconWidget::setFlyInfo(const DataModel::FlyInformation &flyInfo)
{
    hasInfo = true;
    isAlive = flyInfo.isAlive;
    lifetime = flyInfo.lifetime;
    cellsPassed = flyInfo.cellsPassed;
}

void FlyIconWidget::mousePressEvent(QMouseEvent *event)
{
    if(!hasInfo)
        return;

    QString statistics(tr("Муха №%1\nЖивая: %2\nПродолжительность жизни: %3 мс\nКлеток пройдено: %4\nСредняя скорость: %5 мс/клетку").
                       arg(ID).
                       arg(isAlive ? tr("Да") : tr("Нет")).
                       arg(lifetime).
                       arg(cellsPassed).
                       arg(double(lifetime) / cellsPassed));
    QMessageBox::information(this, tr("Статистика по мухе"), statistics);
}
