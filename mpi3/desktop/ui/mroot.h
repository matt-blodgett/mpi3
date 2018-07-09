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
    void initializeLibrary();
    void initializeMainMenu();

private:
    PanelLibrary *m_libview = nullptr;
    PanelPlayback *m_playback = nullptr;

    QTreeView *tree_library = nullptr;
    QTreeView *tree_playlists = nullptr;

    LibraryModel *m_modelLibrary = nullptr;
    LibraryModel *m_modelPlaylists = nullptr;
    QMediaPlayer *m_audio = nullptr;

    Mpi3Library *m_library = nullptr;
    Mpi3Theme *m_theme = nullptr;

private:
    void headerContextMenu(const QPoint &point);
    void libraryContextMenu(const QPoint &point);
    void playlistContextMenu(const QPoint &point);

private slots:
    void libraryViewChanged();

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
    void libDelete();

    void objPlay();
    void objEdit();
    void objDetails();

    void objAddTo();
    void objMoveTo();
    void objRemoveFrom();
    void objDuplicate();

private:
    void paintEvent(QPaintEvent *event);

};


#endif // MROOT_H
