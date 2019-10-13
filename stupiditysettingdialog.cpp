#include "stupiditysettingdialog.h"
#include "ui_stupiditysettingdialog.h"

StupiditySettingDialog::StupiditySettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StupiditySettingDialog)
{
    ui->setupUi(this);
}

StupiditySettingDialog::~StupiditySettingDialog()
{
    delete ui;
}

void StupiditySettingDialog::setStupidityRange(int min, int max)
{
    if(min >= max)
        return;

    ui->minStupidityLabel->setText(QString::number(min));
    ui->maxStupidityLabel->setText(QString::number(max));
    ui->stupiditySlider->setRange(min, max);
    ui->stupiditySlider->setValue((max - min) / 2);
}

int StupiditySettingDialog::getStupidity() const
{
    return ui->stupiditySlider->value();
}
