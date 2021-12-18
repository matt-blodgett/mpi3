#include "mpi3/desktop/ui/models/mmodeldrives.h"
#include "mpi3/core/mmedialibrary.h"
#include "mpi3/util/mformat.h"


#include <QFileIconProvider>
#include <QFileInfo>


#include <QDebug>


MModelDrives::MModelDrives(QObject *parent) : QAbstractItemModel(parent)
{
    m_headers << "Drive";
    m_headers << "Free Space";
    m_headers << "Total Size";
    m_headers << "Available";
    m_headers << "File System Type";
    refresh();
}

int MModelDrives::rowCount(const QModelIndex &) const
{
    return m_deviceData.size();
}
int MModelDrives::columnCount(const QModelIndex &) const
{
    return m_headers.size();
}

QModelIndex MModelDrives::index(int row, int column, const QModelIndex &) const
{
    return createIndex(row, column);
}
QModelIndex MModelDrives::parent(const QModelIndex &) const
{
    return QModelIndex();
}

QVariant MModelDrives::data(const QModelIndex &index, int role) const
{
    if(index.isValid()) {
        if(role == Qt::DisplayRole) {
            QList<QVariant> deviceData = m_deviceData.at(index.row()).toList();
            return deviceData.at(index.column());
        }
        else if(role == Qt::TextAlignmentRole) {
            if(index.column() > 1) {
                return Qt::AlignRight;
            }
        }
        else if(role == Qt::DecorationRole && index.column() == 0) {
            return m_deviceIcons.at(index.row());
        }
    }

    return QVariant();
}
QVariant MModelDrives::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal) {
        if(role == Qt::DisplayRole) {
            return m_headers.at(section);
        }
        else if(role == Qt::TextAlignmentRole) {
            if(section > 1) {
                return Qt::AlignCenter;
            }
        }
    }

    return QVariant();
}


QStorageInfo MModelDrives::storageInfoAt(const QModelIndex &index)
{
    if(index.row() >= 0 && index.row() < rowCount()) {
        return m_storageInfoList.at(index.row());
    }
    return QStorageInfo();
}

void MModelDrives::refresh()
{
    beginResetModel();

    m_storageInfoRoot.refresh();
    m_storageInfoList.clear();
    m_deviceData.clear();
    m_deviceIcons.clear();

    for(const QStorageInfo &storageInfo : m_storageInfoRoot.mountedVolumes()) {
        if(!storageInfo.name().isEmpty()) {
            m_storageInfoList.append(storageInfo);

            QString rootPath = storageInfo.rootPath();

            // Windows
            // QString label = " (" + rootPath.replace("/", "") + ") " + sInfo.name();

            // Linux
            QString label = " (" + rootPath + ") " + storageInfo.name();

            double bfree = static_cast<double>(storageInfo.bytesFree());
            double btotal = static_cast<double>(storageInfo.bytesTotal());

            QList<QString> deviceData;
            deviceData << label;
            deviceData << Mpi3::Util::sizeToString(bfree, 1);
            deviceData << Mpi3::Util::sizeToString(btotal, 1);
            deviceData << Mpi3::Util::percentToString(bfree / btotal, 1);
            deviceData << storageInfo.fileSystemType();

            m_deviceData << QVariant(deviceData);
            m_deviceIcons << QFileIconProvider().icon(QFileInfo(storageInfo.rootPath()));
        }
    }

    endResetModel();
}
