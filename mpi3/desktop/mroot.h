#pragma once

#ifndef MROOT_H
#define MROOT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QPushButton;
class QTreeView;
QT_END_NAMESPACE


#include "mglobal.h"

class MStyleSheet;
class MMediaLibrary;
class MAudioEngine;
class MContentDelegate;

class MContextBar;
class MPanelPlayback;
class MPanelLibrary;
class MPanelDevice;
class MPanelMedia;

class MTreeContainers;
class MTreeSonglist;


class MRootDesktop : public QMainWindow
{
    Q_OBJECT

public:
    explicit MRootDesktop();
    ~MRootDesktop();
    void initialize();

private:
    void initializeObjects();
    void initializeMainMenu();
    void initializeLayout();
    void initializeState();
    void initializeStyle();
    void saveSettings();

private:
    MStyleSheet *m_styleSheet = nullptr;
    MMediaLibrary *m_mediaLibrary = nullptr;
    MAudioEngine *m_audioEngine = nullptr;

    MContextBar *m_contextBar = nullptr;
    MPanelPlayback *m_panelPlayback = nullptr;
    MPanelLibrary *m_panelLibrary = nullptr;
    MPanelDevice *m_panelDevice = nullptr;
    MPanelMedia *m_panelMedia = nullptr;

    MTreeContainers *m_treeContainers = nullptr;
    MTreeSonglist *m_treeSonglist = nullptr;

private:
    QPushButton *m_btnMaximize = nullptr;
    QPushButton *m_btnMinimize = nullptr;
    QPushButton *m_btnClose = nullptr;
    QWidget *m_menuWidget = nullptr;
    QPoint m_lastCursorPoint;
    QPoint m_lastRootPoint;
    QSize m_lastSize;
    // override redirect

    bool m_resizeWest;
    bool m_resizeEast;
    bool m_resizeNorth;
    bool m_resizeSouth;
    bool m_resizeActive;
    bool m_moveActive;
    bool m_maximizeActive;

    void toggleMaximized();

private:
    void setContextPanel();
    void setContainerDisplay();
    void setPlaybackSong(MSong *song);

private:
    void processAudioMediaStatus(Mpi3::MediaState state);
    void processAudioEngineStatus(Mpi3::EngineState state);
    void processAudioErrorStatus(Mpi3::ErrorState state);
    void processAudioRequestStatus(Mpi3::EngineState state);

private:
    void contextMenuHeader(const QPoint &point);
    void contextMenuSonglist(const QPoint &point);
    void contextMenuContainers(const QPoint &point);

private:
    void libImport();
    void libExport();
    void libSetSavePath();
    void libSetMediaPath();
    void libSetBackupPath();
    void libBackup();
    void libReset();

    void themeSet();
    void themeRefresh();

    void libNewFolder();
    void libNewPlaylist();
    void libImportPlaylists();
    void libImportSongs(QTreeView *treeParent);
    void libDownloadSongs(QTreeView *treeParent);

    void objPlay(const QModelIndex &idx);
    void objEdit(const QModelIndex &idx);
    void objDetails(const QModelIndex &idx);

    void objAddTo();
    void objRemoveFrom();
    void objDuplicate();
    void objOpenFileLocation(QTreeView *treeParent);
    void openFileLocation(const QString &path);

    void editUndo();
    void editRedo();

    void objCut(QTreeView *treeParent);
    void objCopy(QTreeView *treeParent);
    void objPaste(QTreeView *treeParent);
    void objDelete(QTreeView *treeParent);

protected:
    bool eventFilter(QObject *object, QEvent *event);
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
};


#endif
