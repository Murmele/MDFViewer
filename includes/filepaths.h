#ifndef FILEPATHS_H
#define FILEPATHS_H

#include <QAbstractListModel>

struct paths{
    QString path; // complete path with filename
    QString filename; // only filename, which will be displayed
};

class FilePaths : public QAbstractListModel
{
    Q_OBJECT
public:
    FilePaths(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override ;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool insertRows(int row, int count, struct paths path, const QModelIndex &parent = QModelIndex()); // wieso darf hier kein override stehen?
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    void addFilePath(QString path);
    QString getPath(int index){return m_path[index].path;}
private:
    QList<struct paths> m_path;
};
#endif // FILEPATHS_H
