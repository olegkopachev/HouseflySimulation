#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    struct Settings
    {
        int fieldSize;
        int flyCapacity;
        int maxStupidity;
        bool enterStupidityManually;
        int animationDuration;
    };

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    void disableFieldSizeOption();

    Settings getSettings() const;

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
