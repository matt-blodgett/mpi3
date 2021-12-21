#pragma once

#ifndef MFRAMECONTAINERS_H
#define MFRAMECONTAINERS_H


#include "mpi3/desktop/ui/frames/mframe.h"


#include "mpi3/core/mglobal.h"
class MTreeContainers;
class MModelContainers;


class MFrameContainers : public MFrame
{
    Q_OBJECT
    Q_PROPERTY(QString iconFolder MEMBER m_icnFolder NOTIFY iconFolderChanged)
    Q_PROPERTY(QString iconPlaylist MEMBER m_icnPlaylist NOTIFY iconPlaylistChanged)

public:
    void iconFolderChanged();
    void iconPlaylistChanged();

private:
    QString m_icnFolder;
    QString m_icnPlaylist;

public:
    explicit MFrameContainers(QWidget *parent = nullptr);

public:
    void itemDetails();
    void editItem();
    void cutItems();
    void copyItems();
    void pasteItems();
    void deleteItems();

    void newFolder();
    void newPlaylist();
    void importPlaylists();
    void duplicatePlaylist();

public:
    MTreeContainers *tree() const;
    MModelContainers *model() const;

    void setLibrary(MMediaLibrary *library = nullptr);

private:
    MMediaLibrary *m_mediaLibrary = nullptr;
    MTreeContainers *m_treeContainers = nullptr;
    MModelContainers *m_modelContainers = nullptr;

private:
    MFolder *getInsertFolder() const;
    void selectContainer();

private slots:
    void containerMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destinationIndex, int row);
    void containerInserted(const QModelIndex &parent, int first, int last);
    void containerCollapsed(const QModelIndex &index);

private slots:
    void contextMenuTreeview(const QPoint &point);

signals:
    void containerSelected(MContainer *container);
};


#endif
