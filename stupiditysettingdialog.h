#ifndef STUPIDITYSETTINGDIALOG_H
#define STUPIDITYSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class StupiditySettingDialog;
}

class StupiditySettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StupiditySettingDialog(QWidget *parent = nullptr);
    ~StupiditySettingDialog();

    void setStupidityRange(int min, int max);
    int getStupidity() const;

private:
    Ui::StupiditySettingDialog *ui;
};

#endif // STUPIDITYSETTINGDIALOG_H
