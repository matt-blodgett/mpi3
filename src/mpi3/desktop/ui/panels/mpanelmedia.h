#pragma once

#ifndef MPANELMEDIA_H
#define MPANELMEDIA_H


#include <QMap>


#include "mpi3/desktop/ui/panels/mpanel.h"

QT_BEGIN_NAMESPACE
class QRadioButton;
QT_END_NAMESPACE


class MFrameContainers;
class MFrameSonglist;
class MTreeSonglistLayoutSettings;


class MPanelMedia : public MPanel
{
    Q_OBJECT

public:
    explicit MPanelMedia(QWidget *parent = nullptr);
    void setLibrary(MMediaLibrary *library) override;
    void load(QSettings *settings) override;
    void save(QSettings *settings) override;

public:
    void viewAllSongs();
    void viewArtists();
    void viewAlbums();
    void viewContainer(MContainer *container);

private:
    void viewChanged();

public:
    MFrameSonglist *frameSonglist();
    MFrameContainers *frameContainers();

private:
    MFrameSonglist *m_frmSonglist = nullptr;
    MFrameContainers *m_frmContainers = nullptr;

    QRadioButton *m_btnSongs = nullptr;
    QRadioButton *m_btnArtists = nullptr;
    QRadioButton *m_btnAlbums = nullptr;
    QLabel *m_lblPlaylist = nullptr;
    QLabel *m_lblView = nullptr;

    MMediaLibrary *m_mediaLibrary = nullptr;
    MTreeSonglistLayoutSettings *m_layoutSettingsManager = nullptr;
};


#endif
