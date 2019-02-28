﻿#pragma once

#ifndef MMODELSONGLIST_H
#define MMODELSONGLIST_H


#include <QSortFilterProxyModel>
#include <QAbstractItemModel>
#include <QIcon>
QT_BEGIN_NAMESPACE
class QMimeData;
QT_END_NAMESPACE


#include "mglobal.h"


class MModelSonglist : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit MModelSonglist(QObject *parent = nullptr);
    ~MModelSonglist() override;

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
    enum View {
        ViewAllSongs,
        ViewArtists,
        ViewAlbums,
        ViewContainer
    };

    void viewAllSongs();
    void viewArtists();
    void viewAlbums();
    void viewContainer(MContainer *container);

    MModelSonglist::View currentView() const;
    Mpi3::ElementType currentType() const;
    QString currentPID() const;

    MMediaLibrary *library() const;
    void setLibrary(MMediaLibrary *library);

    QVector<MSong*> songlist() const;
    MSong *getSong(const QModelIndex &index) const;

private:
    MContainer *m_currentContainer = nullptr;

    QString m_currentPID;
    QStringList m_headers;
    QVector<MSong*> m_songlist;
    MMediaLibrary *m_mediaLibrary = nullptr;
    MModelSonglist::View m_currentView = MModelSonglist::ViewAllSongs;

signals:
    void viewChanged();

private slots:

};


class MModelSonglistProxy : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit MModelSonglistProxy(QObject *parent = nullptr);
};


#endif
