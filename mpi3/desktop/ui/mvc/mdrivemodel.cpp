#include "mdrivemodel.h"


#include "core/mmedialibrary.h"


#include <QFileInfo>
#include <QStorageInfo>


#include <QFileIconProvider>



#include <QDebug>



MModelStorageDrives::MModelStorageDrives(QObject *parent) : QAbstractItemModel(parent){
    m_headers << "Drive";
    m_headers << "Library";
    m_headers << "Free Space";
    m_headers << "Total Size";
    m_headers << "Available";

    refresh();
}

int MModelStorageDrives::rowCount(const QModelIndex &) const{
    return m_deviceData.size();
}
int MModelStorageDrives::columnCount(const QModelIndex &) const{
    return m_headers.size();
}

QModelIndex MModelStorageDrives::index(int row, int column, const QModelIndex &) const{
    return createIndex(row, column);
}
QModelIndex MModelStorageDrives::parent(const QModelIndex &) const{
    return QModelIndex();
}

QVariant MModelStorageDrives::data(const QModelIndex &index, int role) const{

    if(index.isValid()){

        if(role == Qt::DisplayRole){
            QList<QVariant> deviceData = m_deviceData.at(index.row()).toList();
            return deviceData.at(index.column());
        }

        else if(role == Qt::TextAlignmentRole){

            if(index.column() > 1){
                return Qt::AlignRight;
            }
        }

        else if(role == Qt::DecorationRole && index.column() == 0){
            return m_deviceIcons.at(index.row());
        }
    }

    return QVariant();
}
QVariant MModelStorageDrives::headerData(int section, Qt::Orientation orientation, int role) const{

    if(orientation == Qt::Horizontal){

        if(role == Qt::DisplayRole){
            return m_headers.at(section);
        }
        else if(role == Qt::TextAlignmentRole){

            if(section > 1){
                return Qt::AlignCenter;
            }
        }
    }

    return QVariant();
}

QList<MMediaLibrary*> MModelStorageDrives::raspiLibraries() const{
    return m_raspiLibraries.values();
}
MMediaLibrary *MModelStorageDrives::raspiLibraryAt(const QModelIndex &idx) {
    QString label = data(index(idx.row(), 0)).toString();
    if(m_raspiLibraries.keys().contains(label)){
        return m_raspiLibraries[label];
    }

    return nullptr;
}

void MModelStorageDrives::refresh(){

    beginResetModel();

    m_deviceData.clear();
    m_deviceIcons.clear();

    m_storageInfo.refresh();

    foreach(QStorageInfo sInfo, m_storageInfo.mountedVolumes()){

        QList<QString> deviceData;

        QString label = sInfo.name();
        QString letter = sInfo.rootPath().replace("/", "");
        label += " (" + letter + ")";

        deviceData.append(label);

        if(QDir().exists(sInfo.rootPath() + "mpi3")){
            MMediaLibrary *raspiLib = new MMediaLibrary();
            raspiLib->modify(raspiLib->pid(), "Raspi Lib");

            m_raspiLibraries[label] = raspiLib;
            deviceData.append(raspiLib->name());
        }
        else {
            deviceData.append("");
        }

        double bfree = sInfo.bytesFree();
        double btotal = sInfo.bytesTotal();

        deviceData.append(MMediaLibrary::sizeToString(sInfo.bytesFree(), 1));
        deviceData.append(MMediaLibrary::sizeToString(sInfo.bytesTotal(), 1));
        deviceData.append(MMediaLibrary::percentToString(bfree / btotal, 1));
        deviceData.append(sInfo.fileSystemType());

        m_deviceData.append(QVariant(deviceData));
        m_deviceIcons.append(QFileIconProvider().icon(QFileInfo(sInfo.rootPath())));
    }

    endResetModel();
}

































