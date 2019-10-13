#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/fly6.png"));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::disableFieldSizeOption()
{
    ui->fieldSizeSpinBox->setEnabled(false);
}

void SettingsDialog::setSettings(const SettingsDialog::Settings &settings)
{
    if(settings.fieldSize == 0)
        return;

    ui->fieldSizeSpinBox->setValue(settings.fieldSize);
    ui->flyCapacitySpinBox->setValue(settings.flyCapacity);
    ui->maxStupiditySpinBox->setValue(settings.maxStupidity);
    ui->stupiditySettingMethodComboBox->setCurrentIndex(settings.enterStupidityManually ? 1 : 0);
    ui->animationDurationSpinBox->setValue(settings.animationDuration);
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
