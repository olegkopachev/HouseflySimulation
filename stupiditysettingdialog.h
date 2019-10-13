#ifndef STUPIDITYSETTINGDIALOG_H
#define STUPIDITYSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class StupiditySettingDialog;
}

/**
 * @brief The StupiditySettingDialog class Диалоговое окно выбора степени врожденной тупости мухи. Появляется, когда пользователь сажает муху на поле и при этом включена опция "Устанавливать тупость вручную"
 */
class StupiditySettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StupiditySettingDialog(QWidget *parent = nullptr);
    ~StupiditySettingDialog();

    /**
     * @brief setStupidityRange Установить границы допустимой врожденной тупости
     * @param min Минимальное значение
     * @param max Максимальное значение
     */
    void setStupidityRange(int min, int max);

    /**
     * @brief getStupidity Получить установленное пользователем значение врожденной тупости
     * @return Установленная пользователем степень врожденной тупости мухи
     */
    int getStupidity() const;

private:
    Ui::StupiditySettingDialog *ui;  //!< Пользовательский интерфейс
};

#endif // STUPIDITYSETTINGDIALOG_H
