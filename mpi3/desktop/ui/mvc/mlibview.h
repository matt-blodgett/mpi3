#pragma once

#ifndef MLIBVIEW_H
#define MLIBVIEW_H

#include <QTreeView>

QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
QT_END_NAMESPACE


#include "mglobal.h"

class MModelContainers;
class MModelSonglist;
class MStyle;


class MTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit MTreeView(QWidget *parent = nullptr);

protected:
    MStyle *m_drawStyle = nullptr;
};


class MTreeContainers : public MTreeView
{
    Q_OBJECT

public:
    explicit MTreeContainers(QWidget *parent = nullptr);

public:
    MModelContainers *modelContainers();

private:
    MModelContainers *m_modelContainers = nullptr;

private:
    void dropEvent(QDropEvent *event);
};


class MTreeSonglist : public MTreeView
{
    Q_OBJECT

public:
    explicit MTreeSonglist(QWidget *parent = nullptr);

public:
    void autoFitColumns();

public:
    QSortFilterProxyModel *modelSortFilter();
    MModelSonglist *modelSonglist();

    void setContainer(MMediaContainer *container);

private:
    QSortFilterProxyModel *m_modelSortFilter = nullptr;
    MModelSonglist *m_modelSonglist = nullptr;

    MMediaContainer *m_playbackContainer = nullptr;
    MSong *m_playbackSong = nullptr;

    QVector<MSong*> m_playbackQueue;

    QMap<QString, QVariant> m_containerSettings;

public slots:
    void playNextItem();
    void playPrevItem();

private:
    void playItem(const QModelIndex &idx);
    void shiftItem(int offset);

    void populateSettings();
    void populateSettings(MMediaContainer *container);

    void populateQueue();
    void sortChanged(int logicalIndex, Qt::SortOrder order);

signals:
    void playbackChanged(MSong *song);

private:
    bool allowDragMove();
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
};


#endif




















