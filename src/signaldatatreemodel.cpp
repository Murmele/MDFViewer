#include "signaldatatreemodel.h"
#include "signaldatatreeitem.h"
#include "libmdf4.h"
#include <QMessageBox>
#include "data.h"

SignalDataTreeModel::SignalDataTreeModel(Data* parent)
    : QAbstractItemModel(parent),m_parent(parent)
{
    m_rootItem = new SignalDataTreeItem(nullptr);
}

SignalDataTreeModel::~SignalDataTreeModel()
{
    delete m_rootItem;
}

QModelIndex SignalDataTreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    SignalDataTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<SignalDataTreeItem*>(parent.internalPointer());

    SignalDataTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex SignalDataTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    SignalDataTreeItem *childItem = static_cast<SignalDataTreeItem*>(index.internalPointer());
    SignalDataTreeItem *parentItem = childItem->parentItem();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int SignalDataTreeModel::rowCount(const QModelIndex &parent) const
{
    SignalDataTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<SignalDataTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int SignalDataTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<SignalDataTreeItem*>(parent.internalPointer())->columnCount();
    else
        return m_rootItem->columnCount();
}

QVariant SignalDataTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    SignalDataTreeItem *item = static_cast<SignalDataTreeItem*>(index.internalPointer());

    return item->data(index.column());
}

// model is read only
Qt::ItemFlags SignalDataTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return nullptr;

    return QAbstractItemModel::flags(index);
}

QVariant SignalDataTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_rootItem->data(section);

    return QVariant();
}

bool SignalDataTreeModel::removeRows(int row, int count, const QModelIndex &parent){

    SignalDataTreeItem* parentItem = getItem(parent);
    bool success;

    beginRemoveRows(parent, row, row + count-1);
    success = parentItem->removeChildren(row,count);
    endRemoveRows();
    return success;
}

SignalDataTreeItem *SignalDataTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        SignalDataTreeItem *item = static_cast<SignalDataTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_rootItem;
}

// from libmdf/mdf4-export/main.cpp
int SignalDataTreeModel::readMDFFile(QString path){
    mdf::file mdf_file;
    int data_group_index = -1;
    int channel_group_index = -1;

    try {
        mdf_file.open(path.toStdString());
    } catch(const mdf::io_error& e){
        QMessageBox::information(nullptr,tr("Reading MDF file"),tr("Cannot load data file: ")+QString::fromUtf8(e.what()));
        return -1;
    }

    // parse number of groups and channels and show dialog where the user can select group and channel
    const auto& data_groups = mdf_file.get_data_groups();
    QVector<int> cgs;
    for(int i=0; i<data_groups.size(); i++){
        const auto& data_group = data_groups[i];
        const auto& channel_groups = data_group.get_channel_groups();
            cgs.append(channel_groups.size());
    }
    QPair<int,int> gc = m_parent->getMDFProperties(cgs);
    data_group_index = gc.first;
    channel_group_index = gc.second;

    if(data_group_index <0 || channel_group_index <0){
        // failure or in dialog was pressed cancel
        return -1;
    }

    const auto& data_group = data_groups[data_group_index];
    const auto& channel_groups = data_group.get_channel_groups();
    const auto& channel_group = channel_groups[channel_group_index];
    const auto& channels = channel_group.get_channels();

    // parse channel ranges
    QVector<int> channel_list;
    for (std::size_t i = 0; i < channels.size(); i++){
        channel_list.push_back(i);
    }

    removeRows(0,rowCount(),createIndex(0,0,m_rootItem));
    delete m_rootItem; // delete all items
    // create new root item which is empty
    m_rootItem = new SignalDataTreeItem();


    QModelIndex parent = createIndex(m_rootItem->row(),0,m_rootItem);
    beginInsertRows(parent, 0, 0+channel_list.size()-1);
    QModelIndex first_index;
    QModelIndex last_index;

    for (int channel_index : channel_list) {
        const mdf::channel& ch = channels[channel_index];

        QList<QVariant> signalname = {QVariant::fromValue<QString>(QString::fromStdString(ch.get_name()))};
        SignalDataTreeItem *signal = new SignalDataTreeItem(signalname);
        if(channel_index == 0){
            first_index = createIndex(channel_index,0,signal);
        }else if(channel_index == channel_list.length()-1){
            last_index = createIndex(channel_index,0,signal);
        }

        QList<QVariant> unit_string = {QVariant::fromValue<QString>(QString::fromStdString(ch.get_metadata_unit()))};
        SignalDataTreeItem *unit = new SignalDataTreeItem(unit_string,signal);
        signal->appendChild(unit);


        // TODO: optimize this step!
        std::vector<double> column;
        ch.get_data_real(column);
        QList<QVariant> datas;
        foreach(double d, column){
            datas.append(QVariant(d));
        }
        SignalDataTreeItem *data = new SignalDataTreeItem(datas,signal);
        signal->appendChild(data);

        m_rootItem->appendChild(signal);
    }

    endInsertRows();
    dataChanged(first_index,last_index); // brauchts das?
    return 0;
}
