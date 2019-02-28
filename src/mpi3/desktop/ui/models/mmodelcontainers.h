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
class MModelItem;


class MModelContainers : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit MModelContainers(QObject *parent = nullptr);
    ~MModelContainers() override;

public:
    // TODO: set these directly instead of
    // TODO: from mframetreeview.cpp
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

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex index(int row, int column, const QModelIndex &index = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    bool insertRows(int position, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int position, int count, const QModelIndex &parent = QModelIndex()) override;

    bool removeRows(int position, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int count, const QModelIndex &parent = QModelIndex()) override;

public:
    QModelIndex getIndex(const QString &pid) const;
    MModelItem *getItem(const QModelIndex &index) const;
    MModelItem *getItem(const QString &pid) const;
    QString getPID(const QModelIndex &index) const;
    QString getPID(MModelItem *item) const;

    MContainer *getContainer(const QModelIndex &index) const;
    MPlaylist *getPlaylist(const QModelIndex &index) const;
    MFolder *getFolder(const QModelIndex &index) const;
    MFolder *getParentFolder(const QModelIndex &index) const;

    MMediaLibrary *library() const;
    void setLibrary(MMediaLibrary *library);

private:
    void populate(MFolder *parentFolder = nullptr, MModelItem *parentItem = nullptr);

    // TODO: reimplement these
//    void playlistInserted(MPlaylist *childPlaylist, MMediaElement *parentElement);
//    void folderInserted(MFolder *childFolder, MMediaElement *parentElement);

private:
    QMap<QString, MModelItem*> m_libItems;
    MMediaLibrary *m_mediaLibrary = nullptr;
    MModelItem *m_rootItem = nullptr;

private slots:
    // TODO: connect slots to m_mediaLibrary
};


#endif
