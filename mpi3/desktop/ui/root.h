#ifndef ROOT_H
#define ROOT_H

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

//#include <QSettings>

//bool readXmlFile(QIODevice &device, QSettings::SettingsMap &map);
//bool writeXmlFile(QIODevice &device, const QSettings::SettingsMap &map);



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
    void initializeState();
    void initializeLayout();

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
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
};


#endif
