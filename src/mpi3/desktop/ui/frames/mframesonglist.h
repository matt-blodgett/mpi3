#pragma once

#ifndef MFRAMESONGLIST_H
#define MFRAMESONGLIST_H


#include "mpi3/desktop/ui/frames/mframe.h"


#include "mpi3/core/mglobal.h"
class MTreeSonglist;
class MModelSonglist;
class MModelSonglistProxy;
class MTreeLayoutSettings;


class MFrameSonglist : public MFrame
{
    Q_OBJECT

public:
    explicit MFrameSonglist(QWidget *parent = nullptr);

public:
    void itemDetails();
    void editItem();
    void cutItems();
    void copyItems();
    void pasteItems();
    void deleteItems();

    void importSongs();

    void playItemSelected();
    void playItemShift(const int &shift);
    void playItemNext();
    void playItemPrev();

    void addItemsTo();
    void removeItemsFrom();
    void openItemFileLocation();

public:
    MTreeSonglist *tree();
    MModelSonglist *model();
    MModelSonglistProxy *modelProxy();

    void setLibrary(MMediaLibrary *library);
    void setPlaylist(MPlaylist *playlist);

    void setLayoutSettings(MTreeLayoutSettings *layoutSettings);
    void saveLayoutSettings();

private:
    MMediaLibrary *m_mediaLibrary = nullptr;
    MTreeSonglist *m_treeSonglist = nullptr;
    MModelSonglist *m_modelSonglist = nullptr;
    MModelSonglistProxy *m_modelSonglistProxy = nullptr;
    MTreeLayoutSettings *m_layoutSettings = nullptr;
    QString m_pidCurrentPlayingSong;

private slots:
    void moveSelected(int row);

private slots:
    void contextMenuHeader(const QPoint &point);
    void contextMenuTreeview(const QPoint &point);

signals:
    void currentPlayingSongChanged(const QString &pid);
};


#endif
