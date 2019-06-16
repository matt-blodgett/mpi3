#pragma once

#ifndef MFRAMETREEVIEW_H
#define MFRAMETREEVIEW_H


#include "mframe.h"


#include "mglobal.h"
class MTreeContainers;
class MModelContainers;
class MTreeSonglist;
class MModelSonglist;
class MModelSonglistProxy;
class MTreeViewLayoutSettings;


class MFrameTreeView : public MFrame
{
    Q_OBJECT

public:
    explicit MFrameTreeView(QWidget *parent = nullptr);

public:
    virtual void itemDetails();
    virtual void editItem();
    virtual void cutItems();
    virtual void copyItems();
    virtual void pasteItems();
    virtual void deleteItems();
};


class MFrameContainers : public MFrameTreeView
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
//    void itemDetails() override;
//    void editItem() override;
//    void cutItems() override;
//    void copyItems() override;
//    void pasteItems() override;
    void deleteItems() override;

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


class MFrameSonglist : public MFrameTreeView
{
    Q_OBJECT

public:
    explicit MFrameSonglist(QWidget *parent = nullptr);

public:
//    void itemDetails() override;
//    void editItem() override;
//    void cutItems() override;
//    void copyItems() override;
//    void pasteItems() override;
    void deleteItems() override;

    void importSongs();
    void downloadSongs();

    void playItem();
    void addItemsTo();
    void removeItemsFrom();
    void openItemFileLocation();

public:
    MTreeSonglist *tree();
    MModelSonglist *model();
    MModelSonglistProxy *modelProxy();

    void setLibrary(MMediaLibrary *library);
    void setPlaylist(MPlaylist *playlist);

    void setLayoutSettings(MTreeViewLayoutSettings *layoutSettings);
    void saveLayoutSettings();

private:
    MMediaLibrary *m_mediaLibrary = nullptr;
    MTreeSonglist *m_treeSonglist = nullptr;
    MModelSonglist *m_modelSonglist = nullptr;
    MModelSonglistProxy *m_modelSonglistProxy = nullptr;
    MTreeViewLayoutSettings *m_layoutSettings = nullptr;

private slots:
    void moveSelected(int row);

private slots:
    void contextMenuHeader(const QPoint &point);
    void contextMenuTreeview(const QPoint &point);
};


#endif
