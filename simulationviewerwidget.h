#ifndef SIMULATIONVIEWERWIDGET_H
#define SIMULATIONVIEWERWIDGET_H

#include "flyiconwidget.h"
#include "datamodel.h"

#include <QFrame>
#include <QPair>
#include <QList>
#include <QMap>
#include <QPropertyAnimation>
#include <QRandomGenerator>

/**
 * @brief The SimulationViewerWidget class Класс виджета, в котором отображается поле с мухами и их передвижения
 */
class SimulationViewerWidget : public QFrame
{
    Q_OBJECT

    /**
     * @brief The FlyEnvelope struct Структура, служащая оберткой для виджета отдельной мухи
     */
    struct FlyEnvelope
    {
        int cellX;         //!< Первая координата мухи (относительно поля, не виджета)
        int cellY;         //!< Вторая координата мухи (относительно поля, не виджета)
        QPropertyAnimation *animation;  //!< Анимация перемещения мухи
        FlyIconWidget *flyWidget;       //!< Указатель на сам виджет отдельной мухи
    };

public:
    explicit SimulationViewerWidget(QWidget *parent = nullptr);
    ~SimulationViewerWidget();

    /**
     * @brief setBackgroundColor Установить фоновый рисунок
     * @param index Порядковый индекс фонового рисунка (см. resources.qrc) (0 - нет фона)
     */
    void setBackgroundColor(int index);

    /**
     * @brief setFlyIcon Установить иконки мух (актуально только для живых мух)
     * @param index Порядковый индекс иконки мухи (см. resources.qrc)
     */
    void setFlyIcon(int index);

    /**
     * @brief setFieldSize Установить размер поля
     * @param value Размер поля
     */
    void setFieldSize(int value);

    /**
     * @brief addNewFly Добавить виджет новой мухи на поле
     * @param flyID Уникальный идентификатор мухи
     * @param x Первая координата мухи (относительно поля, не виджета)
     * @param y Вторая координата мухи (относительно поля, не виджета)
     * @param stupidity Степень врожденной тупости мухи
     */
    void addNewFly(int flyID, int x, int y, int stupidity);

    /**
     * @brief activateAddFlyMode Изменить режим ожидания добавления мухи
     * @param activate true - включить режим ожидания добавления мухи, false - отключить
     */
    void activateAddFlyMode(bool activate);

    /**
     * @brief moveFly Переместить виджет мухи (с анимацией)
     * @param flyID Уникальный идентификатор мухи
     * @param destCellX Первая координата клетки, куда нужно переместить виджет мухи (относительно поля)
     * @param destCellY Вторая координата клетки, куда нужно переместить виджет мухи (относительно поля)
     */
    void moveFly(int flyID, int destCellX, int destCellY);

    /**
     * @brief killFly Отобразить смерть мухи в ее виджете
     * @param flyID Уникальный идентификатор мухи
     */
    void killFly(int flyID);

    /**
     * @brief setFliesInfo Передать полетную информацию по каждой мухе в соответствующий ей виджет
     * @param fliesInfo Ссылка на таблицу с полетной информацией
     */
    void setFliesInfo(const QMap<int, DataModel::FlyInformation> &fliesInfo);

    /**
     * @brief reset Сброс состояния поля (удаление всех виджетов мух)
     */
    void reset();

    /**
     * @brief setAnimationDuration Установка длительности анимации перемещения мухи
     * @param duration Длительность анимации (в миллисекундах)
     */
    void setAnimationDuration(int duration);

    /**
     * @brief setManualInputOfStupidity Установить способ задания тупости мухи
     * @param value true - вводить тупость мухи вручную, false - генерировать тупость автоматически
     */
    void setManualInputOfStupidity(bool value);

    /**
     * @brief setMaxStupidity Установить максимальную степень тупости мухи при ручном вводе
     * @param stupidity Максимальная степень тупости мухи
     */
    void setMaxStupidity(int stupidity);

protected:    
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief coordsToCell Преобразовать координаты относительно виджета в координаты относительно поля
     * @param x Первая координата относительно этого виджета
     * @param y Вторая координата относительно этого виджета
     * @return Координаты клетки относительно поля
     */
    QPair<int, int> coordsToCell(int x, int y);

    /**
     * @brief coordsForNewFlyInCell Найти координаты для виджета мухи относительно экрана в заданной клетке (с учетом нахождения там других мух)
     * @param cellX Первая координата клетки относительно поля
     * @param cellY Вторая координата клетки относительно поля
     * @return Координаты для виджета мухи относительно экрана
     */
    QPair<int, int> coordsForNewFlyInCell(int cellX, int cellY);

signals:
    /**
     * @brief addNewFlyRequest Сигнал, требующий добавления новой мухи в модель. Транслируется, когда пользователь сажает муху на поле
     * @param cellX Первая координата клетки относительно поля
     * @param cellY Вторая координата клетки относительно поля
     * @param stupidity Степень врожденной тупости мухи
     */
    void addNewFlyRequest(int cellX, int cellY, int stupidity);

    /**
     * @brief flyModeChanged Сигнал, транслирующийся, когда изменяется режим ожидания добавления мухи
     * @param value true - ожидание добавления мухи активировано, false - деактивировано
     */
    void flyModeChanged(bool value);

private:
    int fieldSize = 0;         //!< Размер поля
    int cellWidth = 0;         //!< Ширина одной клетки в пикселах
    int cellHeight = 0;        //!< Высота одной клетки в пикселах
    int cellFlyCapacityX = 0;  //!< Количество виджетов мух, умещающихся в одной клетке по горизонтали
    int cellFlyCapacityY = 0;  //!< Количество виджетов мух, умещающихся в одной клетке по вертикали
    bool isInAddFlyMode = false;  //!< Флаг нахождения в режиме ожидания добавления мухи
    int animationDuration = 0;  //!< Продолжительность анимации перемещения мухи (в мс)
    int maxStupidity = 10000;   //!< Максимальное значение врожденной тупости мух
    bool enterStupidityManually = true;  //!< true - вводить тупость мухи вручную, false - генерировать тупость автоматически
    QRandomGenerator randomGenerator;  //!< Генератор случайных чисел
    QString currentIconName;    //!< Имя текущей иконки живых мух
    QMap<int, FlyEnvelope> flies;  //!< Таблица виджетов мух. Ключом служит уникальный идентификатор мухи
    QMap<QPair<int, int>, QList<int>> fliesByCells; //!< Таблица распределения виджетов мух по клеткам. Ключом служат координаты клетки относительно поля
};

#endif // SIMULATIONVIEWERWIDGET_H
