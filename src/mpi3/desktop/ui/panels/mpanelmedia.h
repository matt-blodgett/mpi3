#pragma once

#ifndef MPANELMEDIA_H
#define MPANELMEDIA_H


#include "mpanel.h"

class MFrameContainers;
class MFrameSonglist;
class MTreeSettingsCollection;


QT_BEGIN_NAMESPACE
class QRadioButton;
QT_END_NAMESPACE


class MPanelMedia : public MPanel
{
    Q_OBJECT

public:
    explicit MPanelMedia(QWidget *parent = nullptr);
    void setLibrary(MMediaLibrary *library) override;
    void load(QSettings *settings) override;
    void save(QSettings *settings) override;

public:
    enum View {
        ViewAllSongs,
        ViewArtists,
        ViewAlbums,
        ViewContainer
    };

    MPanelMedia::View currentView() const;
    MPanelMedia::View previousView() const;
    void changeView(MPanelMedia::View view, MMediaContainer *container = nullptr);

    MFrameContainers *frameContainers();
    MFrameSonglist *frameSonglist();
    MTreeSettingsCollection *treeSettingsCollection();

private:
    MMediaLibrary *m_mediaLibrary = nullptr;

    MPanelMedia::View m_currentView;
    MPanelMedia::View m_previousView;

    MFrameContainers *m_frmContainers = nullptr;
    MFrameSonglist *m_frmSonglist = nullptr;
    MTreeSettingsCollection *m_treeSettingsCollection = nullptr;

    void changeContainer(MMediaContainer *container);
    void setContainer(MMediaContainer *container, const QString &title = QString());

private:
    QRadioButton *m_btnSongs = nullptr;
    QRadioButton *m_btnArtists = nullptr;
    QRadioButton *m_btnAlbums = nullptr;
    QLabel *m_lblPlaylist = nullptr;
    QLabel *m_lblView = nullptr;
};


#endif
