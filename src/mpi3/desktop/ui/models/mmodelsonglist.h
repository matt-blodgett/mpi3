#pragma once

#ifndef MMODELSONGLIST_H
#define MMODELSONGLIST_H


#include "mglobal.h"


#include <QSortFilterProxyModel>
#include <QAbstractItemModel>
#include <QIcon>

QT_BEGIN_NAMESPACE
class QMimeData;
QT_END_NAMESPACE


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
    void setLibrary(MMediaLibrary *library);
    void setContainer(MMediaContainer *container);

    MMediaLibrary *library() const;
    MMediaContainer *container() const;
    QVector<MSong*> songlist() const;
    MSong *getSong(const QModelIndex &index) const;

private:
    QStringList m_headers;
    MMediaLibrary *m_mediaLibrary = nullptr;
    MMediaContainer *m_mediaContainer = nullptr;
    QVector<MSong*> m_songlist;

private slots:
    void elementModified(MMediaElement *elemModified);
    void elementInserted(MMediaElement *elemInserted, MMediaContainer *elemParent);
//    void elementRemoved(MMediaElement *elemRemoved, MMediaContainer *elemParent);
    void elementMoved(MMediaElement *elemMoved, MMediaContainer *elemParent);
    void elementDeleted(MMediaElement *elemDeleted);
};


class MModelSonglistProxy : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit MModelSonglistProxy(QObject *parent = nullptr);
};


#endif
