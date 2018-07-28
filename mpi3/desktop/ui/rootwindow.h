#ifndef ROOTWINDOW_H
#define ROOTWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QMediaPlayer;
QT_END_NAMESPACE

class PanelLibrary;
class PanelPlayback;
class LibraryModel;
class SonglistModel;
class Mpi3TreeView;
class Mpi3Library;
class Mpi3Style;


class Mpi3RootDesktop : public QMainWindow
{
    Q_OBJECT

public:
    explicit Mpi3RootDesktop();
    ~Mpi3RootDesktop();
    void initialize();

private:
    void initializeObjects();
    void initializeActions();
    void initializeMainMenu();
    void initializeLayout();
    void initializeState();
    void saveSettings();

private:
    void headerContextMenu(const QPoint &point);
    void songlistContextMenu(const QPoint &point);
    void containersContextMenu(const QPoint &point);

private:
    PanelLibrary *m_panelLibview = nullptr;
    PanelPlayback *m_panelPlayback = nullptr;

    Mpi3TreeView *tree_containers = nullptr;
    Mpi3TreeView *tree_songlist = nullptr;

    LibraryModel *m_modelContainers = nullptr;
    SonglistModel *m_modelSonglist = nullptr;
    QMediaPlayer *m_audioOutput = nullptr;

    Mpi3Library *m_mediaLibrary = nullptr;
    Mpi3Style *m_qssStyle = nullptr;

private:
    QAction *act_editCut = nullptr;
    QAction *act_editCopy = nullptr;
    QAction *act_editPaste = nullptr;
    QAction *act_editDelete = nullptr;

private slots:
    void libraryViewChanged();
    void selectionChanged();

    void libImport();
    void libExport();
    void libReset();

    void themeSet();
    void themeRefresh();

    void setColumnVisibility(int column);

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
    void closeEvent(QCloseEvent *event);
};


#endif
