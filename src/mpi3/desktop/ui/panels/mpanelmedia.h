#pragma once

#ifndef MPANELMEDIA_H
#define MPANELMEDIA_H


#include "mpanel.h"
QT_BEGIN_NAMESPACE
class QRadioButton;
QT_END_NAMESPACE


class MFrameSonglist;
class MFrameContainers;
class MTreeSettingsCollection;


class MPanelMedia : public MPanel
{
    Q_OBJECT

public:
    explicit MPanelMedia(QWidget *parent = nullptr);
    void setLibrary(MMediaLibrary *library) override;
    void load(QSettings *settings) override;
    void save(QSettings *settings) override;

public:
    MFrameSonglist *frameSonglist();
    MFrameContainers *frameContainers();
    MTreeSettingsCollection *treeSettingsCollection();

private:
    MMediaLibrary *m_mediaLibrary = nullptr;
    MFrameSonglist *m_frmSonglist = nullptr;
    MFrameContainers *m_frmContainers = nullptr;
    MTreeSettingsCollection *m_treeSettingsCollection = nullptr;

private:
    QRadioButton *m_btnSongs = nullptr;
    QRadioButton *m_btnArtists = nullptr;
    QRadioButton *m_btnAlbums = nullptr;
    QLabel *m_lblPlaylist = nullptr;
    QLabel *m_lblView = nullptr;

private slots:
    void viewChanged();
};


#endif
