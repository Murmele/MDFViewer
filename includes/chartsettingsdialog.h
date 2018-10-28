#ifndef CHARTSETTINGSDIALOG_H
#define CHARTSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class ChartSettingsDialog;
}

class ChartSettings;

class ChartSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChartSettingsDialog(ChartSettings* settings, QWidget *parent = nullptr);
    ~ChartSettingsDialog();

private:
    Ui::ChartSettingsDialog *ui;
    ChartSettings* m_settings;

};

#endif // CHARTSETTINGSDIALOG_H
