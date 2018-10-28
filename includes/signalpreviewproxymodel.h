#ifndef SIGNALPREVIEWPROXYMODEL_H
#define SIGNALPREVIEWPROXYMODEL_H

#include <QSortFilterProxyModel>

class SignalPreviewProxyModel : public QSortFilterProxyModel
{
public:
    SignalPreviewProxyModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override ;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent)const override;
};

#endif // SIGNALPREVIEWPROXYMODEL_H
