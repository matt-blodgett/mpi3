#pragma once

#ifndef MMODELDRIVES_H
#define MMODELDRIVES_H


#include <QAbstractItemModel>
#include <QStorageInfo>
#include <QIcon>


#include "mglobal.h"


class MModelStorageDrives : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit MModelStorageDrives(QObject *parent = nullptr);

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
    QList<MMediaLibrary*> raspiLibraries() const;
    MMediaLibrary *raspiLibraryAt(const QModelIndex &idx);
    QString rootPathAt(const QModelIndex &idx) const;

    void refresh();

private:
    QStorageInfo m_storageInfo;
    QList<QString> m_headers;
    QList<QIcon> m_deviceIcons;
    QList<QVariant> m_deviceData;
    QMap<QString, MMediaLibrary*> m_raspiLibraries;
};


#endif
