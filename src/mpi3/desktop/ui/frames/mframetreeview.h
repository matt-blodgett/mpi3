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
class MTreeSettings;


class MFrameTreeView : public MFrame
{
    Q_OBJECT

public:
    explicit MFrameTreeView(QWidget *parent = nullptr);

public:
    virtual void importSongs();
    virtual void downloadSongs();
    virtual void itemDetails();
    virtual void editItem();
    virtual void cutItems();
    virtual void copyItems();
    virtual void pasteItems();
    virtual void deleteItems();
    virtual void duplicateItems();
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
    void importSongs() override;
    void downloadSongs() override;
    void itemDetails() override;
    void editItem() override;
    void cutItems() override;
    void copyItems() override;
    void pasteItems() override;
    void deleteItems() override;
    void duplicateItems() override;

    void newFolder();
    void newPlaylist();
    void importPlaylists();

private:
    MFolder *getInsertFolder();

public:
    MTreeContainers *tree();
    MModelContainers *model();
    MMediaLibrary *library();

private:
    MTreeContainers *m_treeContainers = nullptr;
    MModelContainers *m_modelContainers = nullptr;

private:
    void selectContainer();
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
    void importSongs() override;
    void downloadSongs() override;
    void itemDetails() override;
    void editItem() override;
    void cutItems() override;
    void copyItems() override;
    void pasteItems() override;
    void deleteItems() override;
    void duplicateItems() override;

    void playItem();
    void addItemsTo();
    void removeItemsFrom();
    void openItemFileLocation();

public:
    MTreeSonglist *tree();
    MModelSonglist *model();
    MMediaLibrary *library();
    MModelSonglistProxy *modelProxy();

    void saveTreeSettings();
    void setTreeSettings(MTreeSettings *treeSettings);

private:
    MTreeSettings *m_treeSettings = nullptr;
    MTreeSonglist *m_treeSonglist = nullptr;
    MModelSonglist *m_modelSonglist = nullptr;
    MModelSonglistProxy *m_modelSonglistProxy = nullptr;

private:
    void contextMenuHeader(const QPoint &point);
    void contextMenuTreeview(const QPoint &point);
};


#endif
