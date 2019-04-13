#pragma once

#ifndef MMODELCONTAINERS_H
#define MMODELCONTAINERS_H


#include <QAbstractItemModel>
#include <QIcon>
#include <QMap>
QT_BEGIN_NAMESPACE
class QMimeData;
QT_END_NAMESPACE


#include "mglobal.h"
class MModelContainersItem;


class MModelContainers : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit MModelContainers(QObject *parent = nullptr);
    ~MModelContainers() override;

public:
    // TODO: set these directly instead of from mframetreeview.cpp
    QIcon iconFolder;
    QIcon iconPlaylist;

public:
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDragActions() const override;
    Qt::DropActions supportedDropActions() const override;

    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;

    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

    QModelIndex index(int row, int column, const QModelIndex &index = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole) override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    QList<MModelContainersItem*> allItems(MModelContainersItem *parentItem = nullptr) const;
    MModelContainersItem *getItem(const QModelIndex &index) const;
    MModelContainersItem *getItem(const QString &pid) const;

public:
    QModelIndex getIndex(const QString &pid) const;
    QString getPID(const QModelIndex &index) const;

    bool itemIsPlaylist(const QModelIndex &index) const;
    bool itemIsFolder(const QModelIndex &index) const;

    MMediaLibrary *library() const;
    void setLibrary(MMediaLibrary *library);

private:
    MModelContainersItem *m_rootItem = nullptr;
    MMediaLibrary *m_mediaLibrary = nullptr;

private slots:
    void folderCreated(MFolder *f);
    void playlistCreated(MPlaylist *p);

    void folderDeleted(MFolder *f);
    void playlistDeleted(MPlaylist *p);

    void folderChanged(MFolder *f);
    void playlistChanged(MPlaylist *p);

    void parentFolderChanged(MContainer *c);
};


#endif
