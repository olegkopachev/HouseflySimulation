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
        int fieldSize = 0;
        int flyCapacity = 0;
        int maxStupidity = 0;
        bool enterStupidityManually = false;
        int animationDuration = 0;
    };

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    void disableFieldSizeOption();

    void setSettings(const Settings &settings);
    Settings getSettings() const;

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
