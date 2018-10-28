#ifndef SELECTMDFPROPERTIES_H
#define SELECTMDFPROPERTIES_H

#include <QDialog>

namespace Ui {
class SelectMDFProperties;
}

class SelectMDFProperties : public QDialog
{
    Q_OBJECT

public:
    explicit SelectMDFProperties(QVector<int> gc,QWidget *parent = nullptr);
    QPair<int,int> getProperties();
    ~SelectMDFProperties();

private slots:
    void on_comboBox_data_group_currentIndexChanged(int index);

private:
    Ui::SelectMDFProperties *ui;
    QVector<int> m_gc;
};

#endif // SELECTMDFPROPERTIES_H
