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
    if(index.row() < rowCount()) {
        return m_storageInfo.mountedVolumes().at(index.row());
    }
    return QStorageInfo();
}

void MModelDrives::refresh()
{
    beginResetModel();

    m_deviceData.clear();
    m_deviceIcons.clear();

    m_storageInfo.refresh();

    QStringList rootPathList;
    for(QStorageInfo sInfo : m_storageInfo.mountedVolumes()) {
        QString rootPath = sInfo.rootPath();
        rootPathList.append(rootPath);

        QString label = " (" + rootPath.replace("/", "") + ") " + sInfo.name();

        double bfree = static_cast<double>(sInfo.bytesFree());
        double btotal = static_cast<double>(sInfo.bytesTotal());

        QList<QString> deviceData;
        deviceData << label;
        deviceData << Mpi3::Util::sizeToString(bfree, 1);
        deviceData << Mpi3::Util::sizeToString(btotal, 1);
        deviceData << Mpi3::Util::percentToString(bfree / btotal, 1);
        deviceData << sInfo.fileSystemType();

        m_deviceData << QVariant(deviceData);
        m_deviceIcons << QFileIconProvider().icon(QFileInfo(sInfo.rootPath()));
    }

    endResetModel();
}


