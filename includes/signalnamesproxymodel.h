#ifndef SIGNALNAMESPROXYMODEL_H
#define SIGNALNAMESPROXYMODEL_H

#include <QSortFilterProxyModel>
// Proxy is needed, because the data in the model is sorted in a tree model and the Signal List is a Listview
class SignalNamesProxyModel: public QSortFilterProxyModel
{
public:
    SignalNamesProxyModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override ;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent)const override;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
};

#endif // SIGNALNAMESPROXYMODEL_H
