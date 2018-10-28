#include "filepaths.h"
#include <QFileInfo>

FilePaths::FilePaths(QObject *parent): QAbstractListModel (parent){

}

int FilePaths::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    int length = m_path.length();
    return length;
}
QVariant FilePaths::data(const QModelIndex &index, int role) const {
    int row = index.row();

    if (role == Qt::DisplayRole){
        return m_path[row].filename;
    }

    return QVariant();
}

void FilePaths::addFilePath(QString path){
    QFileInfo fi(path);

    struct paths temp;
    temp.path = path;
    temp.filename = fi.baseName();
    //m_path.append(temp);
    insertRows(m_path.count(),1,temp,QModelIndex());
}

bool FilePaths::insertRows(int row, int count, struct paths path, const QModelIndex &parent){
    beginInsertRows(parent, row, row + count-1);
    for ( int i = row; i < (row + count); i++ ){
        m_path.insert(i, path);
    }
    endInsertRows();
    return true;
}

bool FilePaths::removeRows(int row, int count, const QModelIndex &parent){
    beginRemoveRows(parent, row, row + count-1);
    for ( int i = (row + count - 1); i >= row; i-- )
        m_path.removeAt(i);
    endRemoveRows();
    return true;
}
