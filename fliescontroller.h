#ifndef FLIESCONTROLLER_H
#define FLIESCONTROLLER_H

#include "datamodel.h"
#include "singleflytask.h"

#include <QObject>
#include <QThreadPool>
#include <QList>

/**
 * @brief The FliesController class Класс контроллера решаемой задачи. Обеспечивает создание и управление потоками, в которых моделируется полет мухи, а также изменение мухоемкости в модели
 */
class FliesController : public QObject
{
    Q_OBJECT

public:
    FliesController();
    ~FliesController();

    /**
     * @brief setModel Установить указатель на общую модель данных
     * @param newModel Указатель на модель данных
     */
    void setModel(DataModel *newModel);

    /**
     * @brief quit Завершить работу контроллера и потребовать остановить поток, в котором он выполняется
     */
    void quit();

signals:
    /**
     * @brief simulationStopped Сигнал, транслирующийся, когда симуляция остановлена (потоки всех задач завершены)
     */
    void simulationStopped();

    /**
     * @brief quitRequest Сигнал, транслирующийся, когда контроллер требует остановить поток, в котором он выполняется
     */
    void quitRequest();

public slots:
    /**
     * @brief setFieldSize Установить размер поля
     * @param size Размер поля
     */
    void setFieldSize(int size);

    /**
     * @brief setFlyCapacity Установить мухоёмкость
     * @param capacity Мухоёмкость
     */
    void setFlyCapacity(int capacity);

    /**
     * @brief addFly Попытаться добавить муху в заданную клетку и в случае успеха создать задачу моделирования этой мухи (и если симуляция уже идет - запустить эту задачу)
     * @param cellX Первая координата стартовой клетки мухи на поле
     * @param cellY Вторая координата стартовой клетки мухи на поле
     * @param stupidity Степень врожденной тупости мухи
     */
    void addFly(int cellX, int cellY, int stupidity);

    /**
     * @brief startSimulation Начать симуляцию - запустить все задачи, ожидающие начала
     */
    void startSimulation();

    /**
     * @brief stopSimulation Остановить симуляцию - завершить все потоки моделирования полетов мух
     */
    void stopSimulation();

    /**
     * @brief reset Сбросить контроллер в исходное состояние - завершить выполняемые задачи, если они есть, или удалить ожидающие выполнения задачи, если они есть
     */
    void reset();

protected:
    bool isRunning = false;       //!< Флаг работы симулятора
    int fieldSize = 0;            //!< Размер поля
    int currentFlyCount = 0;      //!< Счетчик мух (нужен для генерации уникальных идентификаторов)
    DataModel *model = nullptr;   //!< Указатель на общую модель данных
    QThreadPool fliesThreadPool;  //!< Пул потоков, в которых выполняются задачи моделирования мух
    QList<SingleFlyTask*> waitingFlyTasks;  //!< Очередь задач, ожидающих выполнения (непустая только когда симуляции не происходит)
    QList<SingleFlyTask*> runningFlyTasks;  //!< Список выполняемых в данный момент задач (непустой только когда идет симуляция)
};

#endif // FLIESCONTROLLER_H
