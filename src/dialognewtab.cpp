#include "dialognewtab.h"
#include "ui_dialognewtab.h"

DialogNewTab::DialogNewTab(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewTab)
{
    ui->setupUi(this);
}

DialogNewTab::~DialogNewTab()
{
    delete ui;
}

QString DialogNewTab::getTabName(){
    return ui->txt_tabname->text();
}
