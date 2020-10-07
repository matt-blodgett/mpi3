#pragma once

#ifndef MROOT_H
#define MROOT_H


#include <QMainWindow>


QT_BEGIN_NAMESPACE
class QMediaPlayer;
QT_END_NAMESPACE


#include "mpi3/desktop/mdesktop.h"
#ifdef MPI3_BUILD_SHARED
class MPI3_EXPORT_UI MRootDesktop;
#endif


#include "mpi3/core/mglobal.h"
class MStyleSheet;
class MMediaLibrary;
class MFrameContextBar;
class MFramePlayback;
class MPanelLibrary;
class MPanelDevice;
class MPanelMedia;


class MRootDesktop : public QMainWindow
{
    Q_OBJECT

public:
    explicit MRootDesktop();
    ~MRootDesktop();

public:
    void initialize();

private:
    void initializeObjects();
    void initializeMainMenu();
    void initializeLayout();
    void initializeState();
    void saveSettings();

private:
    void setContextPanel();
    void setPlaybackSongInitial();
    void setPlaybackSong(const QString &pid);
    void setTheme();
    void refreshTheme();

private:
    MStyleSheet *m_styleSheet = nullptr;
    MMediaLibrary *m_mediaLibrary = nullptr;
    QMediaPlayer *m_mediaPlayer = nullptr;
    MFrameContextBar *m_frameContextBar = nullptr;
    MFramePlayback *m_framePlayback = nullptr;
    MPanelLibrary *m_panelLibrary = nullptr;
    MPanelDevice *m_panelDevice = nullptr;
    MPanelMedia *m_panelMedia = nullptr;

protected:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif
