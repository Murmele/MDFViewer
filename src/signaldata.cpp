#include "signaldata.h"
#include "libmdf4.h"
#include <QMessageBox>
#include "data.h"

SignalData::SignalData(Data* parent): QAbstractTableModel (parent), m_parent(parent){

}

int SignalData::columnCount(const QModelIndex &parent) const{
    if(m_data.empty()){
        return 0;
    }
    return m_data[0].size();
}

int SignalData::rowCount(const QModelIndex &parent) const {

    int size = m_data.size();
    return size;
}
QVariant SignalData::data(const QModelIndex &index, int role) const {

    int row = index.row();
    int column = index.column();


    if (role == Qt::DisplayRole){
        if(row >= 0){
            return m_data[row][column];
        }else if(row == -1){
            return m_column_names[column];
        }else if(row == -2){
            return m_column_units[column];
        }
    }

    return QVariant();
}

bool SignalData::insertRows(int row, int count, const QModelIndex &parent){

    return true;
}

bool SignalData::removeRows(int row, int count, const QModelIndex &parent){
    beginRemoveRows(parent, row, row + count-1);
    for ( int i = (row + count - 1); i >= row; i-- )
        m_column_names.erase(m_column_names.begin()+i);
    endRemoveRows();
    return true;
}

// from libmdf/mdf4-export/main.cpp
int SignalData::readMDFFile(QString path){
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

    m_data.clear();
    removeRows(0,m_column_names.size(),QModelIndex());
    m_column_units.clear();



    beginInsertRows(QModelIndex(), 0, channel_list.size()-1);

    for (int channel_index : channel_list) {
        const mdf::channel& ch = channels[channel_index];
        m_column_names.emplace_back(QString::fromStdString(ch.get_name()));
        m_column_units.emplace_back(QString::fromStdString(ch.get_metadata_unit()));

        m_data.emplace_back();
        ch.get_data_real(m_data.back());
    }

    endInsertRows();
    return 0;
}
