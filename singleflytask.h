#ifndef SINGLEFLYTASK_H
#define SINGLEFLYTASK_H

#include "datamodel.h"

#include <QRunnable>
#include <QRandomGenerator>
#include <QTime>

/**
 * @brief The SingleFlyTask class Класс задачи моделирования полета отдельной мухи (выполняется в соответствующем ей потоке)
 */
class SingleFlyTask : public QRunnable
{
public:

    /**
     * @brief SingleFlyTask Конструктор
     * @param flyID Уникальный идентификатор мухи
     * @param startCellX Первая координата начальной клетки мухи
     * @param startCellY Вторая координата начальной клетки мухи
     * @param fieldSize Размер поля
     * @param flyStupidity Степень врожденной тупости мухи
     * @param dataModel Указатель на общую модель данных
     */
    SingleFlyTask(int flyID, int startCellX, int startCellY, int fieldSize, int flyStupidity, DataModel *dataModel);
    ~SingleFlyTask();

    /**
     * @brief run Моделирование полета мухи. Выполняется до тех пор, пока муха не умрет, либо пока потоку не прикажут завершиться
     */
    virtual void run() override;

    /**
     * @brief stopRequest Требование завершения выполняемой задачи в ближайшее возможное время
     */
    void stopRequest();

protected:

    /**
     * @brief adjacentCellsCount Получить количество соседних клеток на поле относительно текущей позиции мухи
     * @return Количество соседних клеток (примечание: здесь не проверяется, есть ли другие мухи на них или нет)
     */
    int adjacentCellsCount();

    /**
     * @brief getAdjacentCell Получить координаты соседней клетки по ее коду
     * @param cellCode Код соседней клетки (от 0 до 7 в общем случае, от 0 до 4, если мы находимся на границе, и от 0 до 2, если мы находимся в углу)
     * @param destCellX Указатель на переменную, куда будет записана первая координата искомой соседней клетки
     * @param destCellY Указатель на переменную, куда будет записана вторая координата искомой соседней клетки
     */
    void getAdjacentCell(int cellCode, int *destCellX, int *destCellY);

protected:
    int ID;               //!< Уникальный идентификатор мухи
    int currentCellX;     //!< Первая координата текущей клетки
    int currentCellY;     //!< Вторая координата текущей клетки
    int sizeOfField;      //!< Размер поля
    int stupidity;        //!< Степень врожденной тупости
    int lifetime;         //!< Срок жизни
    int cellsPassed = 0;  //!< Количество пройденных клеток (пробег)
    DataModel *model;     //!< Указатель на общую модель данных
    QRandomGenerator *randomGenerator;  //!< Указатель на генератор случайных чисел
    QTime timeCounter;    //!< Счетчик времени (измеряет время жизни мухи)

    bool stopFlag = false;  //!< Флаг требования о завершении задачи в ближайшее возможное время
};

#endif // SINGLEFLYTASK_H
