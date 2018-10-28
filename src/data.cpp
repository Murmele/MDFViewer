#include "data.h"
#include "mainwindow.h"

Data::Data(MainWindow* parent) :m_filepaths(nullptr), m_signal_model(this), m_parent(parent)
{
}

void Data::addDataFile(QString path){
    m_filepaths.addFilePath(path);
}

void Data::removeDataFile(int row){
    m_filepaths.removeRows(row,1,QModelIndex());
}

void Data::readDataFile(int index){
    m_signal_model.readMDFFile(m_filepaths.getPath(index));
}

// opens the form to select group and channel of the mdf file
QPair<int, int> Data::getMDFProperties(QVector<int> gc){
    return m_parent->getMDFFileSettings(gc);
}

Data::~Data(){

}
