#ifndef MROOT_H
#define MROOT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QMediaPlayer;
class QTreeView;
QT_END_NAMESPACE

class PanelLibrary;
class PanelPlayback;
class LibraryModel;
class SonglistModel;
class Mpi3Library;
class Mpi3Theme;


class Mpi3RootDesktop : public QMainWindow
{
    Q_OBJECT

public:
    Mpi3RootDesktop();
    ~Mpi3RootDesktop();
    void initialize();

private:
    void initializeObjects();
    void initializeSharedActions();
    void initializeMainMenu();
    void initializeLibrary();

private:
    void headerContextMenu(const QPoint &point);
    void songlistContextMenu(const QPoint &point);
    void containersContextMenu(const QPoint &point);

private:
    PanelLibrary *m_libview = nullptr;
    PanelPlayback *m_playback = nullptr;

    QTreeView *tree_containers = nullptr;
    QTreeView *tree_songlist = nullptr;

    LibraryModel *m_modelContainers = nullptr;
    SonglistModel *m_modelSonglist = nullptr;
    QMediaPlayer *m_audio = nullptr;

    Mpi3Library *m_library = nullptr;
    Mpi3Theme *m_theme = nullptr;

private:
    QAction *act_editCut;
    QAction *act_editCopy;
    QAction *act_editPaste;
    QAction *act_editDelete;

private slots:
    void libraryViewChanged();
    void selectionChanged();

    void libImport();
    void libExport();

    void themeSet();
    void themeRefresh();

    void setColumnVisibility(const int &column);

    void itemExpand();
    void itemCollapse();
    void itemExpandAll();
    void itemCollapseAll();

    void libNewFolder();
    void libNewPlaylist();
    void libImportPlaylists();
    void libImportSongs();
    void libDownloadSongs();

    void objPlay();
    void objEdit();
    void objDetails();
    void objDelete();

    void objAddTo();
    void objMoveTo();
    void objRemoveFrom();
    void objDuplicate();

private:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
};


#endif // MROOT_H
