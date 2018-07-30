#ifndef ROOTWINDOW_H
#define ROOTWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QMediaPlayer;
QT_END_NAMESPACE

class Mpi3PanelLibrary;
class Mpi3PanelPlayback;
class Mpi3ModelContainers;
class Mpi3ModelSonglist;
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
    void initializeMainMenu();
    void initializeLayout();
    void initializeState();
    void saveSettings();

private:
    void headerContextMenu(const QPoint &point);
    void songlistContextMenu(const QPoint &point);
    void containersContextMenu(const QPoint &point);

private:
    Mpi3PanelLibrary *m_panelLibview = nullptr;
    Mpi3PanelPlayback *m_panelPlayback = nullptr;

    Mpi3TreeView *tree_containers = nullptr;
    Mpi3TreeView *tree_songlist = nullptr;

    Mpi3ModelContainers *m_modelContainers = nullptr;
    Mpi3ModelSonglist *m_modelSonglist = nullptr;

    QMediaPlayer *m_audioOutput = nullptr;
    Mpi3Library *m_mediaLibrary = nullptr;
    Mpi3Style *m_qssStyle = nullptr;

private:
    void setColumnVisibility(int column);
    void openFileLocation(const QString &path);
    void libraryViewChanged();

    void libImport();
    void libExport();
    void libReset();

    void themeSet();
    void themeRefresh();

    void libNewFolder();
    void libNewPlaylist();
    void libImportPlaylists();
    void libImportSongs(Mpi3TreeView *treeParent);
    void libDownloadSongs(Mpi3TreeView *treeParent);

    void objPlay();
    void objEdit();
    void objDetails();

    void objAddTo();
    void objRemoveFrom();
    void objDuplicate();
    void objOpenFileLocation(Mpi3TreeView *treeParent);

    void actUndo();
    void actRedo();

    void objCut();
    void objCopy();
    void objPaste();
    void objDelete(Mpi3TreeView *treeParent);

private:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
};


#endif
