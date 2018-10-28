#ifndef SIGNALDATA_H
#define SIGNALDATA_H

#include <QAbstractListModel>

class Data;

class SignalData : public QAbstractTableModel
{
    Q_OBJECT
public:
    SignalData(Data *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override ;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()); // wieso darf hier kein override stehen?
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    int readMDFFile(QString path);
private:
    Data* m_parent;

    std::vector<std::vector<double> > m_data;
    std::vector<QString> m_column_names;
    std::vector<QString> m_column_units;
};
#endif // SIGNALDATA_H
