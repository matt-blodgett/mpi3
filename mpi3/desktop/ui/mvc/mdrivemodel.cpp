#include "mdrivemodel.h"
#include "core/mmedialibrary.h"

#include <QFileIconProvider>
#include <QFileInfo>


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
    return m_raspiLibraries[rootPathAt(idx)];
}
QString MModelStorageDrives::rootPathAt(const QModelIndex &idx)const {
    QStorageInfo sInfo = m_storageInfo.mountedVolumes().at(idx.row());
    return sInfo.rootPath();
}

void MModelStorageDrives::refresh(){

    beginResetModel();

    m_deviceData.clear();
    m_deviceIcons.clear();

    m_storageInfo.refresh();

    QStringList rootPathList;
    foreach(QStorageInfo sInfo, m_storageInfo.mountedVolumes()){

        QString rootPath = sInfo.rootPath();
        rootPathList.append(rootPath);

        QString label = sInfo.name();
        QString letter = rootPath;
        label += " (" + letter.replace("/", "") + ")";

        if(!m_raspiLibraries.contains(rootPath)){
            m_raspiLibraries[rootPath] = nullptr;
        }

        MMediaLibrary *raspiLib = m_raspiLibraries[rootPath];
        bool raspiLibExists = MMediaLibrary::detectRaspiVolume(rootPath);

        if(!raspiLib && raspiLibExists){
            raspiLib = MMediaLibrary::loadRaspiVolume(rootPath);
            m_raspiLibraries[rootPath] = raspiLib;
        }
        else if(raspiLib && !raspiLibExists){
            m_raspiLibraries[rootPath] = nullptr;
            delete raspiLib;
            raspiLib = nullptr;
        }

        double bfree = sInfo.bytesFree();
        double btotal = sInfo.bytesTotal();

        QList<QString> deviceData;
        deviceData << label;
        deviceData << (raspiLib ? raspiLib->name() : "");
        deviceData << MMediaLibrary::sizeToString(bfree, 1);
        deviceData << MMediaLibrary::sizeToString(btotal, 1);
        deviceData << MMediaLibrary::percentToString(bfree / btotal, 1);
        deviceData << sInfo.fileSystemType();

        m_deviceData << QVariant(deviceData);
        m_deviceIcons << QFileIconProvider().icon(QFileInfo(sInfo.rootPath()));
    }

    QMap<QString, MMediaLibrary*>::const_iterator iter;
    for(iter = m_raspiLibraries.begin(); iter != m_raspiLibraries.end(); iter++){
        if(!rootPathList.contains(iter.key())){
            m_raspiLibraries.remove(iter.key());
            delete iter.value();
        }
    }

    endResetModel();
}


































