#pragma once

#ifndef MROOT_H
#define MROOT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QTreeView;
class QPushButton;
QT_END_NAMESPACE


class MStyleSheet;
class MMediaLibrary;
class MAudioEngine;
class MContentDelegate;

class MPanelLibrary;
class MModelContainers;
class MTreeContainers;

class MPanelPlayback;
class MModelSonglist;
class MTreeSonglist;

class MContextBar;


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
    MContentDelegate *m_contentDelegate = nullptr;

    MPanelLibrary *m_panelLibview = nullptr;
    MModelContainers *m_modelContainers = nullptr;
    MTreeContainers *m_treeContainers = nullptr;

    MPanelPlayback *m_panelPlayback = nullptr;
    MModelSonglist *m_modelSonglist = nullptr;
    MTreeSonglist *m_treeSonglist = nullptr;

    MContextBar *m_contextBar = nullptr;

private:
    QPushButton *m_btnMaximize = nullptr;
    QPushButton *m_btnMinimize = nullptr;
    QPushButton *m_btnClose = nullptr;
    QWidget *m_menuWidget = nullptr;

    // override redirect
    QPoint m_lastCursorPoint;
    QPoint m_lastRootPoint;
    QSize m_lastSize;

    bool m_resizeWest;
    bool m_resizeEast;
    bool m_resizeNorth;
    bool m_resizeSouth;
    bool m_resizeActive;
    bool m_moveActive;
    bool m_maximizeActive;
    void toggleMaximized();

private:
    void currentSongChanged();
    void libraryViewChanged();
    void setCurrentContent(const QModelIndex &idx);
    void openFileLocation(const QString &path);

private:
    void headerContextMenu(const QPoint &point);
    void songlistContextMenu(const QPoint &point);
    void containersContextMenu(const QPoint &point);

    void libImport();
    void libExport();
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

    void editUndo();
    void editRedo();

    void objCut(QTreeView *treeParent);
    void objCopy(QTreeView *treeParent);
    void objPaste(QTreeView *treeParent);
    void objDelete(QTreeView *treeParent);

private:
    bool eventFilter(QObject *object, QEvent *event);
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
};


#endif
