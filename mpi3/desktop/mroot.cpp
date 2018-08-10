#include "mroot.h"
#include "mglobal.h"

#include "core/mlibrary.h"
#include "core/maudio.h"

#include "util/mstyle.h"
#include "util/msettings.h"

#include "ui/maudiocontrol.h"
#include "ui/mlibrarydisplay.h"

#include "ui/mvc/mlibmodel.h"
#include "ui/mvc/mlibview.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QClipboard>
#include <QMimeData>
#include <QProcess>

#include <QHeaderView>
#include <QMenuBar>

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>

#include <QStandardPaths>
#include <QFileDialog>
#include <QDir>



#include <QDebug>



MRootDesktop::MRootDesktop(){}
MRootDesktop::~MRootDesktop(){
    m_audioEngine->stop();
    delete m_audioEngine;
}

void MRootDesktop::initialize(){
    initializeObjects();
    initializeMainMenu();
    initializeLayout();
    initializeState();


//    Mpi3Song *song = m_mediaLibrary->newSong("C:/Users/Matt/Desktop/songs/Prayer in C.mp3");
//    m_mediaLibrary->insert(song);

//    m_mediaLibrary->modify(song, "Prayer in C", Mpi3Song::SongName);
//    m_mediaLibrary->modify(song, "Lilly Wood & The Prick (Ft.) Robin Schulz)", Mpi3Song::SongArtist);


    MSong *song = m_mediaLibrary->libSongs->at(0);
    m_panelPlayback->setDisplay(song);



    m_audioEngine = new MAudioEngine(this);


    connect(m_panelPlayback, &MPanelPlayback::audioPlay, m_audioEngine, &MAudioEngine::play);
    connect(m_panelPlayback, &MPanelPlayback::audioPause, m_audioEngine, &MAudioEngine::pause);
    connect(m_panelPlayback, &MPanelPlayback::changeVolume, m_audioEngine, &MAudioEngine::gain);

    connect(m_audioEngine, &MAudioEngine::notifyEngineStatus, m_panelPlayback, &MPanelPlayback::setState);
    connect(m_audioEngine, &MAudioEngine::notifyPosition, this, &MRootDesktop::mediaControlPosition);
    connect(m_audioEngine, &MAudioEngine::notifyVolume, m_panelPlayback, &MPanelPlayback::setVolume);

    m_audioEngine->gain(50);
    m_audioEngine->open(song->path());
    m_audioEngine->start();


    centralWidget()->show();
}

void MRootDesktop::initializeObjects(){
    m_qssStyleSheet = new MStyleSheet();

    m_panelLibview = new MPanelLibrary(this);
    m_panelPlayback = new MPanelPlayback(this);

    m_treeContainers = findChild<MTreeContainers*>("ContainersTreeview");
    m_treeSonglist = findChild<MTreeSonglist*>("SonglistTreeview");

    m_modelContainers = new MModelContainers();
    m_modelSonglist = new MModelSonglist();

//    m_audioEngine = new MAudioEngine(this);
    m_mediaLibrary = new MMediaLibrary();

    m_treeSonglist->setModel(m_modelSonglist);
    m_treeContainers->setModel(m_modelContainers);

//    connect(m_panelPlayback, &Mpi3PanelPlayback::audioPlay, this, &Mpi3RootDesktop::mediaControlPlay);
//    connect(m_panelPlayback, &Mpi3PanelPlayback::audioPause, this, &Mpi3RootDesktop::mediaControlPause);
//    connect(m_panelPlayback, &Mpi3PanelPlayback::navigateNext, this, &Mpi3RootDesktop::mediaControlNext);
//    connect(m_panelPlayback, &Mpi3PanelPlayback::navigatePrev, this, &Mpi3RootDesktop::mediaControlPrev);
//    connect(m_panelPlayback, &Mpi3PanelPlayback::changeVolume, this, &Mpi3RootDesktop::mediaControlVolume);
//    connect(m_audioEngine, &MAudioEngine::updatePosition, this, &Mpi3RootDesktop::mediaControlPosition);

    connect(m_mediaLibrary, &MMediaLibrary::elementModified, m_panelPlayback, &MPanelPlayback::elementModified);
    connect(m_panelLibview, &MPanelLibrary::viewChanged, this, &MRootDesktop::libraryViewChanged);

    connect(m_treeContainers, &QTreeView::customContextMenuRequested, this, &MRootDesktop::containersContextMenu);
    connect(m_treeSonglist, &QTreeView::customContextMenuRequested, this, &MRootDesktop::songlistContextMenu);
    connect(m_treeSonglist->header(), &QHeaderView::customContextMenuRequested, this, &MRootDesktop::headerContextMenu);
}
void MRootDesktop::initializeMainMenu(){
    QMenuBar *menu_main = menuBar();
    menu_main->setObjectName("MainMenuBar");

    QAction *act_audioSettings = new QAction(menu_main);
    QAction *act_wndExit = new QAction(menu_main);

    QAction *act_libImport = new QAction(menu_main);
    QAction *act_libExport = new QAction(menu_main);
    QAction *act_libReset = new QAction(menu_main);
    QAction *act_libOpenFileLocation = new QAction(menu_main);
    QAction *act_libNewFolder = new QAction(menu_main);
    QAction *act_libNewPlaylist = new QAction(menu_main);
    QAction *act_libImportPlaylists = new QAction(menu_main);
    QAction *act_libImportSongs = new QAction(menu_main);
    QAction *act_libDownloadSongs = new QAction(menu_main);

    QAction *act_themeSet = new QAction(menu_main);
    QAction *act_themeRefresh = new QAction(menu_main);

    QAction *act_editUndo = new QAction(menu_main);
    QAction *act_editRedo = new QAction(menu_main);

    QAction *act_editCut = new QAction(this);
    QAction *act_editCopy = new QAction(this);
    QAction *act_editPaste = new QAction(this);
    QAction *act_editDelete = new QAction(this);
    QAction *act_treeSelectAll = new QAction(this);
    QAction *act_treeSelectNone = new QAction(this);

    QAction *act_raspiConnect = new QAction(menu_main);

    QAction *act_toolsOptions = new QAction(menu_main);

    QAction *act_helpAbout = new QAction(menu_main);

    act_audioSettings->setText("Audio Settings");
    act_wndExit->setText("Exit");

    act_libImport->setText("Import Library");
    act_libExport->setText("Export Library");
    act_libReset->setText("Reset Library");
    act_libOpenFileLocation->setText("Open File Location");
    act_libNewFolder->setText("New Folder");
    act_libNewPlaylist->setText("New Playlist");
    act_libImportPlaylists->setText("Import Playlists");
    act_libImportSongs->setText("Import Songs");
    act_libDownloadSongs->setText("Download Songs");

    act_themeSet->setText("Set Theme");
    act_themeRefresh->setText("Refresh");

    act_editUndo->setText("Undo");
    act_editRedo->setText("Redo");
    act_editCut->setText("Cut");
    act_editCopy->setText("Copy");
    act_editPaste->setText("Paste");
    act_editDelete->setText("Delete");
    act_treeSelectAll->setText("Select All");
    act_treeSelectNone->setText("Select None");

    act_raspiConnect->setText("Connect");

    act_toolsOptions->setText("Options");

    act_helpAbout->setText("About");

    QMenu *menu_file = new QMenu(menu_main);
    QMenu *menu_library = new QMenu(menu_main);
    QMenu *menu_theme = new QMenu(menu_main);

    QMenu *menu_edit = new QMenu(menu_main);
    QMenu *menu_device = new QMenu(menu_main);
    QMenu *menu_tools = new QMenu(menu_main);
    QMenu *menu_help = new QMenu(menu_main);

    menu_file->setTitle("File");
    menu_library->setTitle("Library");
    menu_theme->setTitle("Themes");
    menu_edit->setTitle("Edit");
    menu_device->setTitle("Device");
    menu_tools->setTitle("Tools");
    menu_help->setTitle("Help");

    menu_main->addMenu(menu_file);
    menu_library->addAction(act_libImport);
    menu_library->addAction(act_libExport);
    menu_library->addAction(act_libReset);
    menu_library->addAction(act_libOpenFileLocation);
    menu_library->addSeparator();
    menu_library->addAction(act_libNewFolder);
    menu_library->addAction(act_libNewPlaylist);
    menu_library->addSeparator();
    menu_library->addAction(act_libImportPlaylists);
    menu_library->addAction(act_libImportSongs);
    menu_library->addAction(act_libDownloadSongs);

    menu_theme->addAction(act_themeSet);
    menu_theme->addAction(act_themeRefresh);

    menu_file->addAction(act_audioSettings);
    menu_file->addMenu(menu_library);
    menu_file->addMenu(menu_theme);
    menu_file->addSeparator();
    menu_file->addAction(act_wndExit);

    menu_main->addMenu(menu_edit);
    menu_edit->addAction(act_editUndo);
    menu_edit->addAction(act_editRedo);
    menu_edit->addSeparator();
    menu_edit->addAction(act_editCut);
    menu_edit->addAction(act_editCopy);
    menu_edit->addAction(act_editPaste);
    menu_edit->addAction(act_editDelete);
    menu_edit->addSeparator();
    menu_edit->addAction(act_treeSelectAll);
    menu_edit->addAction(act_treeSelectNone);

    menu_main->addMenu(menu_device);
    menu_device->addAction(act_raspiConnect);

    menu_main->addMenu(menu_tools);
    menu_tools->addAction(act_toolsOptions);

    menu_main->addMenu(menu_help);
    menu_help->addAction(act_helpAbout);

//    connect(act_audioSettings
    connect(act_wndExit, &QAction::triggered, this, [this](){window()->close();});

    connect(act_libImport, &QAction::triggered, this, [this](){libImport();});
    connect(act_libExport, &QAction::triggered, this, [this](){libExport();});
    connect(act_libReset, &QAction::triggered, this, [this](){libReset();});
    connect(act_libOpenFileLocation, &QAction::triggered, [=](){openFileLocation(m_mediaLibrary->filepath());});
    connect(act_libNewPlaylist, &QAction::triggered, this, [this](){libNewPlaylist();});
    connect(act_libNewFolder, &QAction::triggered, this, [this](){libNewFolder();});
    connect(act_libImportPlaylists, &QAction::triggered, this, [this](){libImportPlaylists();});
    connect(act_libImportSongs, &QAction::triggered, this, [this](){libImportSongs(m_treeContainers);});
    connect(act_libDownloadSongs, &QAction::triggered, this, [this](){libDownloadSongs(m_treeContainers);});

    connect(act_themeSet, &QAction::triggered, this, [this](){themeSet();});
    connect(act_themeRefresh, &QAction::triggered, this, [this](){themeRefresh();});

//    connect(act_editUndo, &QAction::triggered, this, [this](){editUndo();});
//    connect(act_editRedo, &QAction::triggered, this, [this](){editRedo();});
    connect(act_editCut, &QAction::triggered, this, [this](){objCut(m_treeSonglist);});
    connect(act_editCopy, &QAction::triggered, this, [this](){objCopy(m_treeSonglist);});
    connect(act_editPaste, &QAction::triggered, this, [this](){objPaste(m_treeSonglist);});
    connect(act_treeSelectAll, &QAction::triggered, this, [this](){m_treeSonglist->selectAll();});
    connect(act_treeSelectNone, &QAction::triggered, this, [this](){m_treeSonglist->selectionModel()->clear();});

//    connect(act_raspiConnect

//    connect(act_toolsOptions

//    connect(act_helpAbout

    connect(m_treeSonglist->selectionModel(), &QItemSelectionModel::selectionChanged, act_editCut, [=](){act_editCut->setDisabled(m_treeSonglist->selectionModel()->selectedRows().size() == 0);});
    connect(m_treeSonglist->selectionModel(), &QItemSelectionModel::selectionChanged, act_editCopy, [=](){act_editCut->setDisabled(m_treeSonglist->selectionModel()->selectedRows().size() == 0);});
    connect(m_treeSonglist->selectionModel(), &QItemSelectionModel::selectionChanged, act_editDelete, [=](){act_editCut->setDisabled(m_treeSonglist->selectionModel()->selectedRows().size() == 0);});

    act_editCut->setDisabled(true);
    act_editCopy->setDisabled(true);
    act_editDelete->setDisabled(true);
}
void MRootDesktop::initializeLayout(){
    QWidget *windowMain = new QWidget(this);
    QGridLayout *layoutMain = new QGridLayout(windowMain);
    layoutMain->addWidget(m_panelPlayback, 0, 0, 1, 1);
    layoutMain->addWidget(m_panelLibview, 1, 0, 1, 1);
    layoutMain->setColumnStretch(0, 1);
    layoutMain->setRowStretch(1, 1);
    layoutMain->setMargin(0);
    layoutMain->setHorizontalSpacing(0);
    layoutMain->setVerticalSpacing(0);
    windowMain->setLayout(layoutMain);
    setCentralWidget(windowMain);
    setMinimumHeight(200);
    setMinimumWidth(700);
}
void MRootDesktop::initializeState(){
    QString appDir = QApplication::applicationDirPath();
    MSettingsXml settings(appDir + "/profile.xml");

    settings.beginGroup("RootWindow");
    QRect screenSize = QApplication::desktop()->availableGeometry(this);
    int d_rootx = (screenSize.width() / 2) - 400;
    int d_rooty = (screenSize.height() / 2) - 300;
    int wnd_rootx = settings.value("rootx", d_rootx).toInt();
    int wnd_rooty = settings.value("rooty", d_rooty).toInt();
    int wnd_width = settings.value("width", 800).toInt();
    int wnd_height = settings.value("height", 600).toInt();
    bool wnd_maximized = settings.value("maximized").toBool();
    settings.endGroup();

    settings.beginGroup("UserApplicationPaths");
    QString qss_path = settings.value("style", ":/styles/default.qss").toString();
    QString lib_path = settings.value("library", appDir + "/newlibrary.mpi3lib").toString();
    settings.endGroup();

    settings.beginGroup("UserApplicationValues");
    int val_volume = settings.value("volume", 50).toInt();
    settings.endGroup();

    if(wnd_maximized){
        showMaximized();
    }
    else{
        move(wnd_rootx, wnd_rooty);
        resize(wnd_width, wnd_height);
    }

    m_qssStyleSheet->load(qss_path);
    setStyleSheet(m_qssStyleSheet->qssStyle());

    m_mediaLibrary->load(lib_path);
    m_modelContainers->setLibrary(m_mediaLibrary);
    m_modelSonglist->setLibrary(m_mediaLibrary);
    m_treeContainers->expandAll();

    m_panelLibview->changeView(MPanelLibrary::ViewAllSongs);
    m_panelPlayback->setVolume(val_volume);
}
void MRootDesktop::saveSettings(){
    QString appDir = QApplication::applicationDirPath();
    QDir().remove(appDir + "/profile.xml");
    MSettingsXml *settings = new MSettingsXml(appDir + "/profile.xml");

    settings->beginGroup("RootWindow");
    settings->setValue("rootx", x());
    settings->setValue("rooty", y());
    settings->setValue("width", width());
    settings->setValue("height", height());
    settings->setValue("maximized", isMaximized());
    settings->endGroup();

    settings->beginGroup("UserApplicationPaths");
    settings->setValue("style", m_qssStyleSheet->qssPath());
    settings->setValue("library", m_mediaLibrary->filepath());
    settings->setValue("media", "");
    settings->setValue("downloads", "");
    settings->endGroup();

    settings->beginGroup("UserApplicationValues");
    settings->setValue("volume", m_panelPlayback->volume());
    settings->endGroup();

    m_mediaLibrary->save();
}

void MRootDesktop::headerContextMenu(const QPoint &point){
    QMenu *menu_context = new QMenu(this);
    for(int i = 0; i < m_modelSonglist->columnCount(); i++){
        QAction *act = new QAction(menu_context);
        act->setText(m_modelSonglist->headerData(i, Qt::Horizontal).toString());

        act->setCheckable(true);
        act->setChecked(!m_modelSonglist->columnVisibility[i]);

        connect(act, &QAction::triggered, this, [=](){setColumnVisibility(i);});
        menu_context->addAction(act);
    }

    menu_context->exec(m_treeSonglist->mapToGlobal(point));
    delete menu_context;
}
void MRootDesktop::songlistContextMenu(const QPoint &point){
    QMenu *menu_context = new QMenu(this);

    QAction *act_objPlay = new QAction(menu_context);
    QAction *act_objEdit = new QAction(menu_context);
    QAction *act_objDetails = new QAction(menu_context);
    QMenu *menu_addto = new QMenu(menu_context);
    QAction *act_editCut = new QAction(menu_context);
    QAction *act_editCopy = new QAction(menu_context);
    QAction *act_editPaste = new QAction(menu_context);
    QAction *act_objOpenFileLocation = new QAction(menu_context);
    QAction *act_treeSelectAll = new QAction(menu_context);
    QAction *act_treeSelectNone = new QAction(menu_context);
    QAction *act_libImportSongs = new QAction(menu_context);
    QAction *act_libDownloadSongs = new QAction(menu_context);
    QAction *act_objRemove = new QAction(menu_context);
    QAction *act_objDelete = new QAction(menu_context);

    act_objPlay->setText("Play");
    act_objEdit->setText("Edit");
    act_objDetails->setText("Details");
    menu_addto->setTitle("Add to Playlist...");
    act_editCut->setText("Cut");
    act_editCopy->setText("Copy");
    act_editPaste->setText("Paste");
    act_objOpenFileLocation->setText("Open File Location");
    act_treeSelectAll->setText("Select All");
    act_treeSelectNone->setText("Select None");
    act_libImportSongs->setText("Import Songs");
    act_libDownloadSongs->setText("Download Songs");
    act_objRemove->setText("Remove from Playlist");
    act_objDelete->setText("Delete from Library");

    menu_context->addAction(act_objPlay);
    menu_context->addAction(act_objEdit);
    menu_context->addAction(act_objDetails);
    menu_context->addMenu(menu_addto);
    menu_context->addSeparator();
    menu_context->addAction(act_editCut);
    menu_context->addAction(act_editCopy);
    menu_context->addAction(act_editPaste);
    menu_context->addAction(act_objOpenFileLocation);
    menu_context->addSeparator();
    menu_context->addAction(act_treeSelectAll);
    menu_context->addAction(act_treeSelectNone);
    menu_context->addSeparator();
    menu_context->addAction(act_libImportSongs);
    menu_context->addAction(act_libDownloadSongs);
    menu_context->addSeparator();
    menu_context->addAction(act_objRemove);
    menu_context->addAction(act_objDelete);

    QModelIndex index = m_treeSonglist->indexAt(point);
    if(!index.isValid()){
//        act_objPlay->setDisabled(true);
        act_objEdit->setDisabled(true);
        act_objDetails->setDisabled(true);
        menu_addto->setDisabled(true);
        act_editCopy->setDisabled(true);
        act_editCut->setDisabled(true);
        act_objOpenFileLocation->setDisabled(true);
        act_objRemove->setDisabled(true);
        act_objDelete->setDisabled(true);
    }

    if(m_treeSonglist->selectionModel()->selectedRows().size() > 1){
        act_objOpenFileLocation->setDisabled(true);
    }

    if(m_modelSonglist->currentDisplay() != MModelSonglist::DisplayPlaylist){
        act_objRemove->setDisabled(true);
    }

    connect(act_objPlay, &QAction::triggered, this, &MRootDesktop::objPlay);
    connect(act_objEdit, &QAction::triggered, this, &MRootDesktop::objEdit);
    connect(act_objDetails, &QAction::triggered, this, &MRootDesktop::objDetails);

    connect(act_editCut, &QAction::triggered, this, [this](){objCut(m_treeSonglist);});
    connect(act_editCopy, &QAction::triggered, this, [this](){objCopy(m_treeSonglist);});
    connect(act_editPaste, &QAction::triggered, this, [this](){objPaste(m_treeSonglist);});
    connect(act_objOpenFileLocation, &QAction::triggered, this, [this](){objOpenFileLocation(m_treeSonglist);});

    connect(act_treeSelectAll, &QAction::triggered, m_treeSonglist, [this](){m_treeSonglist->selectAll();});
    connect(act_treeSelectNone, &QAction::triggered, this, [this](){m_treeSonglist->selectionModel()->clear();});

    connect(act_libImportSongs, &QAction::triggered, this, [this](){libImportSongs(m_treeSonglist);});
    connect(act_libDownloadSongs, &QAction::triggered, this, [this](){libDownloadSongs(m_treeSonglist);});

//    connect(act_objRemove, &QAction::triggered, this, &Mpi3RootDesktop::objRemoveFrom);
    connect(act_objDelete, &QAction::triggered, this, [this](){objDelete(m_treeSonglist);});

    menu_context->exec(m_treeSonglist->mapToGlobal(point));
    delete menu_context;
}
void MRootDesktop::containersContextMenu(const QPoint &point){
    QMenu *menu_context = new QMenu(this);

    QAction *act_itemExpand = new QAction(menu_context);
    QAction *act_itemCollapse = new QAction(menu_context);
    QAction *act_itemExpandAll = new QAction(menu_context);
    QAction *act_itemCollapseAll = new QAction(menu_context);
    QAction *act_libNewFolder = new QAction(menu_context);
    QAction *act_libNewPlaylist = new QAction(menu_context);
    QAction *act_libImportPlaylists = new QAction(menu_context);
    QAction *act_libImportSongs = new QAction(menu_context);
    QAction *act_libDownloadSongs = new QAction(menu_context);
    QAction *act_objEdit = new QAction(menu_context);
    QAction *act_objDetails = new QAction(menu_context);
    QAction *act_objDuplicate = new QAction(menu_context);
    QAction *act_objDelete = new QAction(menu_context);

    act_itemExpand->setText("Expand");
    act_itemCollapse->setText("Collapse");
    act_itemExpandAll->setText("Expand All");
    act_itemCollapseAll->setText("Collapse All");
    act_libNewFolder->setText("New Folder");
    act_libNewPlaylist->setText("New Playlist");
    act_libImportPlaylists->setText("Import Playlists");
    act_libImportSongs->setText("Import Songs");
    act_libDownloadSongs->setText("Download Songs");
    act_objEdit->setText("Edit");
    act_objDetails->setText("Details...");
    act_objDuplicate->setText("Duplicate");
    act_objDelete->setText("Delete from Library");

    menu_context->addAction(act_itemExpand);
    menu_context->addAction(act_itemCollapse);
    menu_context->addSeparator();
    menu_context->addAction(act_itemExpandAll);
    menu_context->addAction(act_itemCollapseAll);
    menu_context->addSeparator();
    menu_context->addAction(act_libNewFolder);
    menu_context->addAction(act_libNewPlaylist);
    menu_context->addSeparator();
    menu_context->addAction(act_libImportPlaylists);
    menu_context->addAction(act_libImportSongs);
    menu_context->addAction(act_libDownloadSongs);
    menu_context->addSeparator();
    menu_context->addAction(act_objDuplicate);
    menu_context->addAction(act_objEdit);
    menu_context->addAction(act_objDetails);
    menu_context->addSeparator();
    menu_context->addAction(act_objDelete);

    QModelIndex index = m_treeContainers->indexAt(point);
    m_treeContainers->setCurrentIndex(index);

    MMediaElement *element = m_mediaLibrary->getElement(m_modelContainers->getPID(index));
    if(!element){
        act_itemExpand->setDisabled(true);
        act_itemCollapse->setDisabled(true);
        act_objDuplicate->setDisabled(true);
        act_objEdit->setDisabled(true);
        act_objDetails->setDisabled(true);
        act_objDelete->setDisabled(true);
    }
    else if(element->type() == Mpi3::PlaylistElement){
        act_itemExpand->setDisabled(true);
        act_itemCollapse->setDisabled(true);
    }
    else if(element->type() == Mpi3::FolderElement){
        act_libImportSongs->setDisabled(true);
        act_libDownloadSongs->setDisabled(true);
        act_objDuplicate->setDisabled(true);
    }

    connect(act_itemExpand, &QAction::triggered, this, [this](){m_treeContainers->expand(m_treeContainers->currentIndex());});
    connect(act_itemCollapse, &QAction::triggered, this, [this](){m_treeContainers->collapse(m_treeContainers->currentIndex());});
    connect(act_itemExpandAll, &QAction::triggered, this, [this](){m_treeContainers->expandAll();});
    connect(act_itemCollapseAll, &QAction::triggered, this, [this](){m_treeContainers->collapseAll();});

    connect(act_libNewPlaylist, &QAction::triggered, this, [this](){libNewPlaylist();});
    connect(act_libNewFolder, &QAction::triggered, this, [this](){libNewFolder();});

    connect(act_libImportPlaylists, &QAction::triggered, this, [this](){libImportPlaylists();});
    connect(act_libImportSongs, &QAction::triggered, this, [this](){libImportSongs(m_treeContainers);});
    connect(act_libDownloadSongs, &QAction::triggered, this, [this](){libDownloadSongs(m_treeContainers);});

//    connect(act_objDuplicate, &QAction::triggered, this, &Mpi3RootDesktop::objDuplicate);
//    connect(act_objEdit, &QAction::triggered, this, &Mpi3RootDesktop::objEdit);
//    connect(act_objDetails, &QAction::triggered, this, &Mpi3RootDesktop::objDetails);

    connect(act_objDelete, &QAction::triggered, this, [this](){objDelete(m_treeContainers);});

    menu_context->exec(m_treeContainers->mapToGlobal(point));
    delete menu_context;


//    else if (pid != m_mediaLibrary->pid()){
//        tree_containers->setCurrentIndex(m_modelContainers->getIndex(pid));
//        m_panelLibview->changeView(PanelLibrary::ViewContainer);
//    }
//    else {
//        tree_containers->selectionModel()->clear();
//    }
}

void MRootDesktop::mediaControlPlay(){

//    Mpi3Song *song = m_mediaLibrary->libSongs->at(0);

//    m_audioEngine->load(song->path().toStdString());
//    m_audioEngine->start();

//    qDebug() << "check";

//    if(!m_audioEngine->loaded()){
//        Mpi3Song *song = m_mediaLibrary->libSongs->at(0);

//        m_audioEngine->load(song->path().toStdString());

//        if(m_audioEngine->loaded()){
//            m_panelPlayback->setDisplay(song);
//            m_panelPlayback->setPlaying(true);
//            m_audioEngine->start();
////            m_audioEngine->play();
//        }
//    }

//    else if(m_audioEngine->paused()){
//        m_panelPlayback->setPlaying(true);
//        m_audioEngine->play();
//    }

}
void MRootDesktop::mediaControlPause(){
//    if(m_audioEngine->loaded() && !m_audioEngine->paused()){
//        m_panelPlayback->setPlaying(false);
//        m_audioEngine->pause();
//    }
}
void MRootDesktop::mediaControlNext(){

}
void MRootDesktop::mediaControlPrev(){

}
void MRootDesktop::mediaControlVolume(float vol){
    qDebug() << "VOLUME " <<  vol;
//    m_audioEngine->setVolume(vol/100);
}


void MRootDesktop::mediaControlPosition(double pos){
    Q_UNUSED(pos);
//    qDebug() << pos;
}

void MRootDesktop::setColumnVisibility(int column){
    bool hidden = m_modelSonglist->columnVisibility[column];
    m_treeSonglist->setColumnHidden(column, !hidden);
    m_modelSonglist->columnVisibility[column] = !hidden;
}
void MRootDesktop::openFileLocation(const QString &path){
    QStringList processArgs;
    processArgs << "/select," << QDir::toNativeSeparators(path);
    QProcess::startDetached("explorer", processArgs);
}

void MRootDesktop::libraryViewChanged(){
    switch(m_panelLibview->currentView()) {

        case MPanelLibrary::ViewAllSongs: {
            m_panelLibview->setDisplay("Library");
            m_modelSonglist->setDisplay(MModelSonglist::DisplayAllSongs);
            break;
        }
        case MPanelLibrary::ViewArtists: {
            m_panelLibview->setDisplay("Artists");
            m_modelSonglist->setDisplay(MModelSonglist::DisplayArtists);
            break;
        }
        case MPanelLibrary::ViewAlbums: {
            m_panelLibview->setDisplay("Albums");
            m_modelSonglist->setDisplay(MModelSonglist::DisplayArtists);
            break;
        }
        case MPanelLibrary::ViewContainer: {
            QModelIndex selectedIndex = m_treeContainers->selectionModel()->currentIndex();
            QString pidContainer = m_modelContainers->getPID(selectedIndex);
            MMediaElement *element = m_mediaLibrary->getElement(pidContainer);
            if(element){
                m_panelLibview->setDisplay(element->name());
                m_modelSonglist->setDisplay(element);
            }
            break;
        }
    }
}

void MRootDesktop::libImport(){
    QString libFile;
    QString pathDesktop(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    libFile = QFileDialog::getOpenFileName(this, "Open Mpi3Library File", pathDesktop, "Mpi3Lib Files (*.mpi3lib)");

    if(libFile != ""){
        m_mediaLibrary->load(libFile);
        m_modelContainers->setLibrary(m_mediaLibrary);
        m_modelSonglist->setLibrary(m_mediaLibrary);
        themeRefresh();
    }
}
void MRootDesktop::libExport(){
    QString libFile;
    QString pathDesktop(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    libFile = QFileDialog::getSaveFileName(this, "Export Mpi3Library File", pathDesktop, "Mpi3Lib Files (*.mpi3lib)");

    if(libFile != ""){
        m_mediaLibrary->save(libFile);
    }
}
void MRootDesktop::libReset(){
    m_mediaLibrary->libSongs->clear();
    m_mediaLibrary->libPlaylists->clear();
    m_mediaLibrary->libFolders->clear();
    m_mediaLibrary->save();

    m_modelContainers->setLibrary(m_mediaLibrary);
    m_modelSonglist->setLibrary(m_mediaLibrary);
    m_panelLibview->changeView(MPanelLibrary::ViewAllSongs);
}

void MRootDesktop::themeSet(){
    QString qssFile;
    QString pathDesktop(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    qssFile = QFileDialog::getOpenFileName(this, "Open QSS Theme File", pathDesktop, "QSS Files (*.qss)");

    if(qssFile != ""){
        m_qssStyleSheet->load(qssFile);
        themeRefresh();
    }
}
void MRootDesktop::themeRefresh(){
    if(m_qssStyleSheet){
        m_qssStyleSheet->load();
        setStyleSheet(m_qssStyleSheet->qssStyle());
    }
}

void MRootDesktop::libNewFolder(){
    QModelIndex currentIndex = m_treeContainers->currentIndex();
    MFolder *parentFolder = m_modelContainers->getParentFolderAt(currentIndex);
    MFolder *insertFolder = m_mediaLibrary->newFolder(true);
    m_mediaLibrary->insert(insertFolder, parentFolder);
    m_treeContainers->expand(currentIndex);
}
void MRootDesktop::libNewPlaylist(){
    QModelIndex currentIndex = m_treeContainers->currentIndex();
    MFolder *parentFolder = m_modelContainers->getParentFolderAt(currentIndex);
    MPlaylist *insertPlaylist = m_mediaLibrary->newPlaylist(true);
    m_mediaLibrary->insert(insertPlaylist, parentFolder);
    m_treeContainers->expand(currentIndex);
}
void MRootDesktop::libImportPlaylists(){
    QString plistFile;
    QString pathDesktop(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    plistFile = QFileDialog::getOpenFileName(this, "Open Itunes Plist File", pathDesktop, "XML Files (*.xml)");

    if(plistFile != ""){
        QModelIndex currentIndex = m_treeContainers->currentIndex();
        MFolder *parentFolder = m_modelContainers->getParentFolderAt(currentIndex);
        m_mediaLibrary->importItunesPlist(plistFile, parentFolder);
        m_treeContainers->expand(currentIndex);
    }
}
void MRootDesktop::libImportSongs(QTreeView *treeParent){
    QStringList songFiles;
    QString pathDesktop(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    songFiles = QFileDialog::getOpenFileNames(this, "Add Media Files", pathDesktop, "All Files (*.*)");

    if(songFiles.size() < 1){
        return;
    }

    MPlaylist *parentPlaylist = nullptr;
    if(treeParent == m_treeContainers){
        parentPlaylist = m_mediaLibrary->getPlaylist(m_modelContainers->getPID(m_treeContainers->currentIndex()));
    }
    else if(treeParent == m_treeSonglist && m_modelSonglist->currentDisplay() == MModelSonglist::DisplayPlaylist){
        parentPlaylist = m_mediaLibrary->getPlaylist(m_modelSonglist->currentContainer());
    }

    for(int i = 0; i < songFiles.size(); i++){
        MSong *addedSong = m_mediaLibrary->newSong(songFiles.at(i));
        m_mediaLibrary->insert(addedSong, parentPlaylist);
    }
}
void MRootDesktop::libDownloadSongs(QTreeView *treeParent){
    if(treeParent == m_treeContainers){
        libImportSongs(treeParent);
    }
}

void MRootDesktop::objPlay(){}
void MRootDesktop::objEdit(){}
void MRootDesktop::objDetails(){}

void MRootDesktop::objAddTo(){}
void MRootDesktop::objRemoveFrom(){}
void MRootDesktop::objDuplicate(){}
void MRootDesktop::objOpenFileLocation(QTreeView *treeParent){
    if(treeParent == m_treeSonglist && m_treeSonglist->selectionModel()->selectedRows().size() == 1){
        MSong *song = m_modelSonglist->getSongAt(treeParent->currentIndex());
        if(song){
            openFileLocation(song->path());
        }
    }
}

void MRootDesktop::editUndo(){}
void MRootDesktop::editRedo(){}

void MRootDesktop::objCut(QTreeView *treeParent){
    if(treeParent == m_treeSonglist){
//        QApplication::clipboard()->setMimeData()
    }
}
void MRootDesktop::objCopy(QTreeView *treeParent){
    if(treeParent == m_treeSonglist){
        QModelIndexList selectedIndexes = m_treeSonglist->selectionModel()->selectedIndexes();
        QApplication::clipboard()->setMimeData(m_modelSonglist->mimeData(selectedIndexes));
    }
}
void MRootDesktop::objPaste(QTreeView *treeParent){
    if(treeParent == m_treeSonglist){
        QModelIndex currentIndex = m_treeSonglist->currentIndex();
        const QMimeData *data = QApplication::clipboard()->mimeData(QClipboard::Clipboard);
        m_modelSonglist->dropMimeData(data, Qt::CopyAction, currentIndex.row(), currentIndex.column(), currentIndex.parent());
    }
}
void MRootDesktop::objDelete(QTreeView *treeParent){
    if(treeParent == m_treeContainers && m_treeContainers->selectionModel()->selectedRows().size() > 0){
        QModelIndex index = m_treeContainers->currentIndex();
        QString pid = m_modelContainers->getPID(index);

        MMediaElement *element = m_mediaLibrary->getElement(pid);
        if(!element){
            return;
        }
        else if(element->type() == Mpi3::FolderElement){
            m_mediaLibrary->discard(static_cast<MFolder*>(element));
        }
        else if(element->type() == Mpi3::PlaylistElement){
            m_mediaLibrary->discard(static_cast<MPlaylist*>(element));
        }

        QString pidCurrentContainer = m_modelSonglist->currentContainer();
        if(pidCurrentContainer.isNull()){
            m_panelLibview->changeView(MPanelLibrary::ViewAllSongs);
        }
        else if(pidCurrentContainer == m_mediaLibrary->pid()){
            m_treeContainers->selectionModel()->clear();
        }
    }
    else if(treeParent == m_treeSonglist && m_treeSonglist->selectionModel()->selectedRows().size() > 0){
        QModelIndexList currentIndexes = m_treeSonglist->selectionModel()->selectedIndexes();

        for(int i = 0; i < currentIndexes.size(); i++){
            QModelIndex index = currentIndexes.at(i);
            MSong *song = m_modelSonglist->getSongAt(index);
            m_mediaLibrary->discard(song);
        }
    }
}

void MRootDesktop::paintEvent(QPaintEvent *event){
    QStyleOption opt;
    opt.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QWidget::paintEvent(event);
}
void MRootDesktop::closeEvent(QCloseEvent *event){
    saveSettings();
    QMainWindow::closeEvent(event);
}
