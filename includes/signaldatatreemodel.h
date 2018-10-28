#ifndef SIGNALDATATREEMODEL_H
#define SIGNALDATATREEMODEL_H

#include <QAbstractItemModel>


class Data;
class SignalDataTreeItem;

class SignalDataTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    SignalDataTreeModel(Data* parent = nullptr);
    ~SignalDataTreeModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    int readMDFFile(QString path);
    SignalDataTreeItem* getItem(const QModelIndex &index) const;

private:

    void setupModelData(const QStringList &lines, SignalDataTreeItem *parent);

    SignalDataTreeItem *m_rootItem;
    Data* m_parent;
};

#endif // SIGNALDATATREEMODEL_H
