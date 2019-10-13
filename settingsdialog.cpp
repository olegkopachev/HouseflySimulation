#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::disableFieldSizeOption()
{
    ui->fieldSizeSpinBox->setEnabled(false);
}

SettingsDialog::Settings SettingsDialog::getSettings() const
{
    Settings result;
    result.fieldSize = ui->fieldSizeSpinBox->value();
    result.flyCapacity = ui->flyCapacitySpinBox->value();
    result.maxStupidity = ui->maxStupiditySpinBox->value();
    result.enterStupidityManually = ui->stupiditySettingMethodComboBox->currentIndex() == 1 ? true : false;
    result.animationDuration = ui->animationDurationSpinBox->value();

    return result;
}
