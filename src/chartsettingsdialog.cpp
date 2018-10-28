#include "chartsettingsdialog.h"
#include "ui_chartsettingsdialog.h"

ChartSettingsDialog::ChartSettingsDialog(ChartSettings *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChartSettingsDialog),
    m_settings(settings)
{
    ui->setupUi(this);

    // load settings
}

ChartSettingsDialog::~ChartSettingsDialog()
{
    delete ui;
}
