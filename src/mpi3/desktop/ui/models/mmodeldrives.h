#pragma once

#ifndef MMODELDRIVES_H
#define MMODELDRIVES_H


#include <QAbstractItemModel>
#include <QStorageInfo>
#include <QIcon>


class MModelDrives : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit MModelDrives(QObject *parent = nullptr);

public:
//    Qt::ItemFlags flags(const QModelIndex &index) const override;
//    Qt::DropActions supportedDragActions() const override;
//    Qt::DropActions supportedDropActions() const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex index(int row, int column, const QModelIndex &index = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

public:
    QStorageInfo storageInfoAt(const QModelIndex &index);

    void refresh();

private:
    QList<QString> m_headers;
    QStorageInfo m_storageInfoRoot;
    QVector<QStorageInfo> m_storageInfoList;
    QList<QVariant> m_deviceData;
    QList<QIcon> m_deviceIcons;
};


#endif
