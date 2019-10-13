#include "flyiconwidget.h"
#include "defines.h"

#include <QImage>
#include <QMessageBox>

FlyIconWidget::FlyIconWidget(int flyID, double stupidityPercent, QWidget *parent) :
    QLabel(parent)
{
    ID = flyID;
    stupidityRate = stupidityPercent;

    setFixedSize(FLY_ICON_WIDTH, FLY_ICON_HEIGHT);
    setAttribute(Qt::WA_TranslucentBackground);

    QPixmap pixmap(":/images/fly.png");
    QImage image = pixmap.toImage();
    for(int i = 0; i < image.width(); i++)
    {
        for (int j = 0; j < image.height(); j++)
        {
            QColor color = image.pixelColor(i, j);
            int newBlue = color.blue() + int((255 - color.blue()) * stupidityPercent);
            color.setBlue(newBlue);
            image.setPixelColor(i, j, color);
        }
    }
    setPixmap(QPixmap::fromImage(image));
}

FlyIconWidget::~FlyIconWidget()
{
}

void FlyIconWidget::drawDeadFly()
{
    QPixmap pixmap(":/images/deadfly.png");
    QImage image = pixmap.toImage();
    for(int i = 0; i < image.width(); i++)
    {
        for (int j = 0; j < image.height(); j++)
        {
            QColor color = image.pixelColor(i, j);
            int newBlue = color.blue() + int((255 - color.blue()) * stupidityRate);
            color.setBlue(newBlue);
            image.setPixelColor(i, j, color);
        }
    }
    setPixmap(QPixmap::fromImage(image));
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
                       arg(cellsPassed == 0 ? QString("N/A") : QString::number(double(lifetime) / cellsPassed)));
    QMessageBox::information(nullptr, tr("Статистика по мухе"), statistics);
}
