#include "signalpreviewproxymodel.h"

SignalPreviewProxyModel::SignalPreviewProxyModel(QObject *parent): QSortFilterProxyModel (parent)
{

}

int SignalPreviewProxyModel::rowCount(const QModelIndex &parent) const {
    return 2;
}

int SignalPreviewProxyModel::columnCount(const QModelIndex &parent) const{

    //QModelIndex index2 = sourceModel()->index(0,0,QModelIndex()); // assumption: all data rows have same number of elements (use row0 to determine columncount)
    //index2 = sourceModel()->index(2,0,index2); // data model index (child of signal)
    return sourceModel()->columnCount(parent);
}

QVariant SignalPreviewProxyModel::data(const QModelIndex &index, int role) const {

    int row = index.row();
    int column = index.column();

    // This job already index do
    //QModelIndex index2 = sourceModel()->index(row,0,QModelIndex()); // child "row" of m_root_item of sourceModel (signal)
    //index2 = sourceModel()->index(2,0,index2); // data model index (child of signal)
    //sourceModel()->data(index2);

    return  sourceModel()->data(index,role);
}

QModelIndex SignalPreviewProxyModel::index(int row, int column, const QModelIndex &parent)const {

    QModelIndex index2 = sourceModel()->index(row,0,QModelIndex()); // child "row" of m_root_item of sourceModel (signal)
    return sourceModel()->index(2,0,index2); // data model index (child of signal)
}

