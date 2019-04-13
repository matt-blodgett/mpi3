#pragma once

#ifndef MMODELSONGLIST_H
#define MMODELSONGLIST_H


#include <QSortFilterProxyModel>
#include <QAbstractTableModel>

QT_BEGIN_NAMESPACE
class QMimeData;
QT_END_NAMESPACE


#include "mmedialibrary.h"


class MModelSonglistItem;
class MModelSonglist : public QAbstractTableModel
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

    QModelIndex index(int row, int column, const QModelIndex &index = QModelIndex()) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole) override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

public:
    MMediaLibrary *library() const;
    void setLibrary(MMediaLibrary *library);

    void setSonglist(MSongList songs, const QString &pid);
    QString currentPID() const;

private:
    QStringList m_headers;
    QList<MModelSonglistItem> m_songlist;

    MMediaLibrary *m_mediaLibrary = nullptr;
    QString m_pid;

private slots:
    void playlistContentsChanged(MPlaylist *p);
};


#endif
