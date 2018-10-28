#ifndef DATA_H
#define DATA_H
#include "filepaths.h"
#include "signaldata.h"
#include "signaldatatreemodel.h"

class MainWindow;

class Data: public QObject
{
    Q_OBJECT
public:
    Data(MainWindow* parent);
    void addDataFile(QString path);
    void removeDataFile(int row);
    FilePaths* getFilePaths(){return &m_filepaths;}
    SignalDataTreeModel* getSignalModel(){return &m_signal_model;}
    void readDataFile(int index);
    QPair<int, int> getMDFProperties(QVector<int> gc); // returns group channel, channel index
    ~Data();
private:
    FilePaths m_filepaths;
    SignalDataTreeModel m_signal_model;
    MainWindow* m_parent;


};

#endif // DATA_H
