#ifndef MROOT_H
#define MROOT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QMediaPlayer;
class QTreeView;
class QAction;
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
    PanelLibrary *m_libview = nullptr;
    PanelPlayback *m_playback = nullptr;
    QTreeView *tree_library = nullptr;
    LibraryModel *m_model = nullptr;
    QMediaPlayer *m_audio = nullptr;
    Mpi3Library *m_library = nullptr;
    Mpi3Theme *m_theme = nullptr;

private:
    void initializeActions();
    void initializeMainMenu();

    void treeviewContextMenu(const QPoint &point);
    void headerContextMenu(const QPoint &point);

    QAction *act_audioSettings = nullptr;
    QAction *act_wndExit = nullptr;

    QAction *act_libImport = nullptr;
    QAction *act_libExport = nullptr;

    QAction *act_themeSet = nullptr;
    QAction *act_themeRefresh = nullptr;

    QAction *act_editUndo = nullptr;
    QAction *act_editRedo = nullptr;
    QAction *act_editCut = nullptr;
    QAction *act_editCopy = nullptr;
    QAction *act_editPaste = nullptr;
    QAction *act_editDelete = nullptr;

    QAction *act_viewMaximize = nullptr;

    QAction *act_raspiConnect = nullptr;

    QAction *act_toolsOptions = nullptr;

    QAction *act_helpAbout = nullptr;

    QAction *act_itemExpand = nullptr;
    QAction *act_itemCollapse = nullptr;
    QAction *act_itemExpandAll = nullptr;
    QAction *act_itemCollapseAll = nullptr;

    QAction *act_libDelete = nullptr;
    QAction *act_libNewFolder = nullptr;
    QAction *act_libNewPlaylist = nullptr;
    QAction *act_libImportPlaylists = nullptr;
    QAction *act_libImportSongs = nullptr;
    QAction *act_libDownloadSongs = nullptr;

    QAction *act_objPlay = nullptr;
    QAction *act_objEdit = nullptr;
    QAction *act_objDetails = nullptr;

    QAction *act_objAddTo = nullptr;
    QAction *act_objMoveTo = nullptr;
    QAction *act_objRemoveFrom = nullptr;
    QAction *act_objDuplicate = nullptr;

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

    void libDelete();
    void libNewFolder();
    void libNewPlaylist();
    void libImportPlaylists();

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






















