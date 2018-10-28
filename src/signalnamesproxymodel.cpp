#include "signalnamesproxymodel.h"
#include <QAbstractItemModel>

SignalNamesProxyModel::SignalNamesProxyModel(QObject *parent): QSortFilterProxyModel (parent){
}

int SignalNamesProxyModel::rowCount(const QModelIndex &parent) const {
    int rowcount = sourceModel()->rowCount(QModelIndex()); // of root item (number of signals)
    return rowcount;
}

int SignalNamesProxyModel::columnCount(const QModelIndex &parent) const{
    return 1;
}

QVariant SignalNamesProxyModel::data(const QModelIndex &index, int role) const {

    //int row = index.row();
    //QString value = sourceModel()->data(model->index(-1,index.row()),role).toString();
    //QString value = "";



    //QModelIndex index2 = sourceModel()->index(row,0,QModelIndex()); // child "row" of m_root_item of sourceModel
    QString value = sourceModel()->data(index).toString();

    return value;
}

QModelIndex SignalNamesProxyModel::index(int row, int column, const QModelIndex &parent)const {
    QModelIndex index2 = sourceModel()->index(row,0,QModelIndex()); // child "row" of m_root_item of sourceModel (signal)
    return index2;
}

QModelIndex SignalNamesProxyModel::mapFromSource(const QModelIndex &sourceIndex) const{
    int row = sourceIndex.row();

    QModelIndex index2 = createIndex(row,0,sourceModel());
    return index2;
}
