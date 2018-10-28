#include "selectmdfproperties.h"
#include "ui_selectmdfproperties.h"

SelectMDFProperties::SelectMDFProperties(QVector<int> gc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectMDFProperties), m_gc(gc)
{
    ui->setupUi(this);

    for(int i=0; i<gc.length(); i++){
        ui->comboBox_data_group->addItem(QString::number(i));
    }

    // assumption: at least one data group and channel group exist
    ui->comboBox_data_group->setCurrentIndex(0);
    ui->comboBox_channel_group->setCurrentIndex(0);


}

QPair<int,int> SelectMDFProperties::getProperties(){
    int data_group = ui->comboBox_data_group->itemData(ui->comboBox_data_group->currentIndex()).toInt();
    int channel_group = ui->comboBox_channel_group->itemData(ui->comboBox_channel_group->currentIndex()).toInt();
    return QPair<int,int> (data_group,channel_group);
}

SelectMDFProperties::~SelectMDFProperties()
{
    delete ui;
}

void SelectMDFProperties::on_comboBox_data_group_currentIndexChanged(int index)
{

    ui->comboBox_channel_group->clear();

    for(int i=0; i<m_gc[index]; i++){
        ui->comboBox_channel_group->addItem(QString::number(i));
    }
}
