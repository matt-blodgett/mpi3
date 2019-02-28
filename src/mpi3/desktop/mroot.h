#pragma once

#ifndef MROOT_H
#define MROOT_H


#include <QMainWindow>


#include "mglobal.h"
#include "mdesktop.h"
#ifdef MPI3_BUILD_SHARED
class MPI3_EXPORT_UI MRootDesktop;
#endif


class MStyleSheet;
class MMediaLibrary;
class MAudioEngine;
class MFrameContextBar;
class MFramePlayback;
class MPanelLibrary;
class MPanelDevice;
class MPanelMedia;
class MMenuBar;


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
    void setPlaybackSong(MSong *song);

    void setTheme();
    void refreshTheme();

private:
    MStyleSheet *m_styleSheet = nullptr;
    MMediaLibrary *m_mediaLibrary = nullptr;
    MAudioEngine *m_audioEngine = nullptr;

private:
    MFrameContextBar *m_contextBar = nullptr;
    MFramePlayback *m_panelPlayback = nullptr;
    MPanelLibrary *m_panelLibrary = nullptr;
    MPanelDevice *m_panelDevice = nullptr;
    MPanelMedia *m_panelMedia = nullptr;
    MMenuBar *m_menuBar = nullptr;

protected:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
};


#endif
