#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QMutex>
#include <QMap>
#include <QPair>

/**
 * @brief The DataModel class Класс модели данных, используемой в приложении
 */
class DataModel : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief The FlyInformation struct Структура с информацией об отдельной мухе
     */
    struct FlyInformation
    {
        /**
         * @brief FlyInformation Конструктор
         * @param cellIndex Начальная координата мухи (о способе задания координаты - см. метод index(int,int))
         */
        FlyInformation(int cellIndex = 0);

        int cellIndex;   //!< Текущая координата мухи (о способе задания координаты - см. метод index(int,int))
        bool isAlive;    //!< Флаг, обозначающий, жива ли муха
        int lifetime;    //!< Возраст мухи (в миллисекундах)
        int cellsPassed; //!< Количество пройденных клеток (пробег)
    };

public:
    DataModel();
    ~DataModel();

    /**
     * @brief reset Сбросить модель в исходное состояние - удалить всех мух
     */
    void reset();

    /**
     * @brief setFieldSize Установить размер поля
     * @param value Размер поля
     */
    void setFieldSize(int value);

    /**
     * @brief setFlyCapacity Установить мухоёмкость
     * @param value Мухоёмкость
     */
    void setFlyCapacity(int value);

    /**
     * @brief addNewFly Попытаться добавить на поле новую муху с учетом мухоемкости
     * @param flyID Уникальный идентификатор мухи
     * @param cellX Первая координата начальной клетки мухи на поле
     * @param cellY Вторая координата начальной клетки мухи на поле
     * @param stupidity Степень врожденной тупости мухи
     * @return Успешность добавления мухи на поле (true - муха добавлена, false - муха не добавлена, так как в клетке превышена мухоёмкость)
     */
    bool addNewFly(int flyID, int cellX, int cellY, int stupidity);

    /**
     * @brief tryToMove Попытаться передвинуть муху на заданную клетку
     * @param flyID Уникальный идентификатор мухи
     * @param destCellX Первая координата клетки на поле, куда надо переместить муху
     * @param destCellY Вторая координата клетки на поле, куда надо переместить муху
     * @return Успешность перемещения мухи (true - муха перемещена, false - муха не перемещена, так как в клетке назначения превышена мухоёмкость)
     */
    bool tryToMove(int flyID, int destCellX, int destCellY);

    /**
     * @brief killFly Убить муху
     * @param flyID Уникальный идентификатор мухи
     */
    void killFly(int flyID);

    /**
     * @brief setFlyLifeAndRouteInfo Установить возраст и пробег мухи
     * @param flyID Уникальный идентификатор мухи
     * @param lifetime Возраст мухи (в миллисекундах)
     * @param cellsPassed Пробег мухи (количество пройденных клеток)
     */
    void setFlyLifeAndRouteInfo(int flyID, int lifetime, int cellsPassed);

    /**
     * @brief getFliesInfo Получить константную ссылку на информацию обо всех мухах
     * @return Константная ссылка на информацию о мухах
     */
    const QMap<int, FlyInformation>& getFliesInfo() const;

protected:
    /**
     * @brief index Вычислить код клетки на основании двух ее координат на поле
     * @param x Первая координата клетки на поле
     * @param y Вторая координата клетки на поле
     * @return Код клетки
     */
    inline int index(int x, int y);

signals:
    /**
     * @brief flyAdded Сигнал, транслирующийся, когда новая муха добавлена на поле
     * @param flyID Уникальный идентификатор мухи
     * @param cellX Первая координата начальной клетки мухи на поле
     * @param cellY Вторая координата начальной клетки мухи на поле
     * @param stupidity Степень врожденной тупости мухи
     */
    void flyAdded(int flyID, int cellX, int cellY, int stupidity);

    /**
     * @brief flyMoved Сигнал, транслирующийся, когда муха перемещена в другую клетку
     * @param flyID Уникальный идентификатор мухи
     * @param destCellX Первая координата клетки, в которую перемещена муха
     * @param destCellY Вторая координата клетки, в которую перемещена муха
     */
    void flyMoved(int flyID, int destCellX, int destCellY);

    /**
     * @brief flyDied Сигнал, транслирующийся, когда муха умирает
     * @param flyID Уникальный идентификатор мухи
     */
    void flyDied(int flyID);

    /**
     * @brief modelReset Сигнал, транслирующийся, когда модель сброшена в исходное состояние
     */
    void modelReset();

protected:
    int fieldSize = 0;             //!< Размер поля
    int flyCapacity = 0;           //!< Мухоёмкость
    QMap<int, QList<int>> cellsInfo;      //!< Таблица распределения мух по клеткам. Ключом служит код клетки (см. ф-цию index(int,int))
    QMap<int, FlyInformation> fliesInfo;  //!< Таблица с информацией о мухах. Ключом служит уникальный идентификатор мухи
    QMap<int, QMutex*> mutexes;    //!< Мьютексы для каждой клетки на поле. Ключом служит код клетки (см. ф-цию index(int,int))
};

#endif // DATAMODEL_H
