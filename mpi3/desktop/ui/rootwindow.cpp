#include "rootwindow.h"
#include "librarydisplay.h"
#include "audiocontrol.h"

#include "mvc/libmodel.h"
#include "mvc/libview.h"

#include "util/uistyle.h"
#include "util/mpi3library.h"
#include "util/xmlsettings.h"

#include <QApplication>
#include <QMediaPlayer>
#include <QDesktopWidget>
#include <QHeaderView>
#include <QMenuBar>

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>

#include <QStandardPaths>
#include <QFileDialog>
#include <QDir>


#include <QDebug>


Mpi3RootDesktop::Mpi3RootDesktop(){}
Mpi3RootDesktop::~Mpi3RootDesktop(){}

void Mpi3RootDesktop::initialize(){
    initializeObjects();
    initializeActions();
    initializeMainMenu();
    initializeLayout();
    initializeState();
    centralWidget()->show();
}

void Mpi3RootDesktop::initializeObjects(){
    m_panelLibview = new PanelLibrary(this);
    m_panelPlayback = new PanelPlayback(this);

    tree_containers = findChild<Mpi3TreeView*>("PlaylistsTreeview");
    tree_songlist = findChild<Mpi3TreeView*>("LibraryTreeview");

    m_modelContainers = new LibraryModel();
    m_modelSonglist = new SonglistModel();
    m_audioOutput = new QMediaPlayer(this);

    m_mediaLibrary = new Mpi3Library();
    m_qssStyle = new Mpi3Style();

    m_audioOutput->setAudioRole(QAudio::MusicRole);
    tree_songlist->setModel(m_modelSonglist);
    tree_containers->setModel(m_modelContainers);

    connect(m_panelPlayback, &PanelPlayback::play, m_audioOutput, &QMediaPlayer::play);
    connect(m_panelPlayback, &PanelPlayback::pause, m_audioOutput, &QMediaPlayer::pause);
    connect(m_panelPlayback, &PanelPlayback::stop, m_audioOutput, &QMediaPlayer::stop);
//    connect(m_playback, &PanelPlayback::next, m_playlist, &QMediaPlaylist::next);
//    connect(m_playback, &PanelPlayback::previous, this, &PanelPlayback::previousClicked);
    connect(m_panelPlayback, &PanelPlayback::changeVolume, m_audioOutput, &QMediaPlayer::setVolume);

    connect(m_audioOutput, &QMediaPlayer::stateChanged, m_panelPlayback, &PanelPlayback::setState);
    connect(m_audioOutput, &QMediaPlayer::volumeChanged, m_panelPlayback, &PanelPlayback::setVolume);
    connect(m_audioOutput, &QMediaPlayer::mutedChanged, m_panelPlayback, &PanelPlayback::setMuted);

    connect(m_panelLibview, &PanelLibrary::viewChanged, this, &Mpi3RootDesktop::libraryViewChanged);

    connect(tree_containers, &QTreeView::customContextMenuRequested, this, &Mpi3RootDesktop::containersContextMenu);
    connect(tree_songlist, &QTreeView::customContextMenuRequested, this, &Mpi3RootDesktop::songlistContextMenu);
    connect(tree_songlist->header(), &QHeaderView::customContextMenuRequested, this, &Mpi3RootDesktop::headerContextMenu);

    connect(tree_containers->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this](){selectionChanged();});
    connect(tree_songlist->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this](){selectionChanged();});
}
void Mpi3RootDesktop::initializeActions(){
    act_editCut = new QAction(this);
    act_editCopy = new QAction(this);
    act_editPaste = new QAction(this);
    act_editDelete = new QAction(this);

    act_editCut->setText("Cut");
    act_editCopy->setText("Copy");
    act_editPaste->setText("Paste");
    act_editDelete->setText("Delete");

    act_editCut->setDisabled(true);
    act_editCopy->setDisabled(true);
    act_editPaste->setDisabled(true);
    act_editDelete->setDisabled(true);
}
void Mpi3RootDesktop::initializeMainMenu(){
    QMenuBar *menu_main = menuBar();

    menu_main->setObjectName("MainMenuBar");

    QAction *act_audioSettings = new QAction(menu_main);
    QAction *act_wndExit = new QAction(menu_main);

    act_wndExit->setText("Exit");
    act_audioSettings->setText("Audio Settings");

    // connect
    // connect

    QAction *act_libImport = new QAction(menu_main);
    QAction *act_libExport = new QAction(menu_main);
    QAction *act_libReset = new QAction(menu_main);

    act_libImport->setText("Import Library");
    act_libExport->setText("Export Library");
    act_libReset->setText("Reset Library");

    connect(act_libImport, &QAction::triggered, this, &Mpi3RootDesktop::libImport);
    connect(act_libExport, &QAction::triggered, this, &Mpi3RootDesktop::libExport);
    connect(act_libReset, &QAction::triggered, this, &Mpi3RootDesktop::libReset);

    QAction *act_libNewFolder = new QAction(menu_main);
    QAction *act_libNewPlaylist = new QAction(menu_main);

    act_libNewFolder->setText("New Folder");
    act_libNewPlaylist->setText("New Playlist");

    connect(act_libNewFolder, &QAction::triggered, this, &Mpi3RootDesktop::libNewFolder);
    connect(act_libNewPlaylist, &QAction::triggered, this, &Mpi3RootDesktop::libNewPlaylist);

    QAction *act_themeSet = new QAction(menu_main);
    QAction *act_themeRefresh = new QAction(menu_main);

    act_themeSet->setText("Set Theme");
    act_themeRefresh->setText("Refresh");

    connect(act_themeSet, &QAction::triggered, this, &Mpi3RootDesktop::themeSet);
    connect(act_themeRefresh, &QAction::triggered, this, &Mpi3RootDesktop::themeRefresh);

    QAction *act_editUndo = new QAction(menu_main);
    QAction *act_editRedo = new QAction(menu_main);

//    *act_editCut = new QAction(menu_main);
//    *act_editCopy = new QAction(menu_main);
//    *act_editPaste = new QAction(menu_main);
//    *act_editDelete = new QAction(menu_main);

    act_editUndo->setText("Undo");
    act_editRedo->setText("Redo");
//    act_editCut->setText("Cut");
//    act_editCopy->setText("Copy");
//    act_editPaste->setText("Paste");
//    act_editDelete->setText("Delete");


    // connect
    // connect
    // connect
    // connect
    // connect
    // connect

    QAction *act_viewMaximize = new QAction(menu_main);

    act_viewMaximize->setText("Maximize");

    // connect

    QAction *act_raspiConnect = new QAction(menu_main);

    act_raspiConnect->setText("Connect");

    // connect

    QAction *act_toolsOptions = new QAction(menu_main);

    act_toolsOptions->setText("Options");

    // connect

    QAction *act_helpAbout = new QAction(menu_main);

    act_helpAbout->setText("About");

    // connect


    QMenu *menu_file = new QMenu(menu_main);
    QMenu *menu_edit = new QMenu(menu_main);
    QMenu *menu_view = new QMenu(menu_main);
    QMenu *menu_device = new QMenu(menu_main);
    QMenu *menu_tools = new QMenu(menu_main);
    QMenu *menu_help = new QMenu(menu_main);

    menu_file->setTitle("File");
    menu_edit->setTitle("Edit");
    menu_view->setTitle("View");
    menu_device->setTitle("Device");
    menu_tools->setTitle("Tools");
    menu_help->setTitle("Help");

    menu_main->addMenu(menu_file);
    QMenu *menu_library = new QMenu(menu_main);
    QMenu *menu_theme = new QMenu(menu_main);

    menu_library->setTitle("Library");
    menu_theme->setTitle("Themes");

    menu_library->addAction(act_libImport);
    menu_library->addAction(act_libExport);
    menu_library->addAction(act_libReset);
    menu_library->addSeparator();
    menu_library->addAction(act_libNewFolder);
    menu_library->addAction(act_libNewPlaylist);

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

    menu_main->addMenu(menu_view);
    menu_view->addAction(act_viewMaximize);

    menu_main->addMenu(menu_device);
    menu_device->addAction(act_raspiConnect);

    menu_main->addMenu(menu_tools);
    menu_tools->addAction(act_toolsOptions);

    menu_main->addMenu(menu_help);
    menu_help->addAction(act_helpAbout);
}
void Mpi3RootDesktop::initializeLayout(){
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
    setMinimumHeight(100);
    setMinimumWidth(100);
}
void Mpi3RootDesktop::initializeState(){
    QString appDir = QApplication::applicationDirPath();
    QSettings settings(appDir + "/profile.xml", XmlSettingsFormat);

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

    m_qssStyle->load(qss_path);
    setStyleSheet(m_qssStyle->qssStyle());

    m_mediaLibrary->load(lib_path);
    m_modelContainers->setLibrary(m_mediaLibrary);
    m_modelSonglist->setLibrary(m_mediaLibrary);
    tree_containers->expandAll();

//    m_audioOutput->setMedia(QUrl("C:/Users/Matt/Desktop/Calm Down.wav"));
    m_audioOutput->setVolume(val_volume);


    m_panelLibview->changeView(PanelLibrary::ViewAllSongs);
}
void Mpi3RootDesktop::saveSettings(){
    QString appDir = QApplication::applicationDirPath();
    QDir().remove(appDir + "/profile.xml");
    QSettings *settings = new QSettings(appDir + "/profile.xml", XmlSettingsFormat);

    settings->beginGroup("RootWindow");
    settings->setValue("rootx", x());
    settings->setValue("rooty", y());
    settings->setValue("width", width());
    settings->setValue("height", height());
    settings->setValue("maximized", isMaximized());
    settings->endGroup();

    settings->beginGroup("UserApplicationPaths");
    settings->setValue("style", m_qssStyle->qssPath());
    settings->setValue("library", m_mediaLibrary->filepath());
    settings->setValue("media", "");
    settings->setValue("downloads", "");
    settings->endGroup();

    settings->beginGroup("UserApplicationValues");
    settings->setValue("volume", m_audioOutput->volume());
    settings->endGroup();

    m_mediaLibrary->save();
}

void Mpi3RootDesktop::headerContextMenu(const QPoint &point){
    QMenu *menu_context = new QMenu(this);
    for(int i = 0; i < m_modelSonglist->columnCount(); i++){
        QAction *act = new QAction(menu_context);
        act->setText(m_modelSonglist->headerData(i, Qt::Horizontal).toString());

        act->setCheckable(true);
        act->setChecked(!m_modelSonglist->columnVisibility[i]);

        connect(act, &QAction::triggered, this, [=](){setColumnVisibility(i);});
        menu_context->addAction(act);
    }

    menu_context->exec(tree_songlist->mapToGlobal(point));
    delete menu_context;
}
void Mpi3RootDesktop::songlistContextMenu(const QPoint &point){
    QMenu *menu_context = new QMenu(this);

    QAction *act_objPlay = new QAction(menu_context);
    QAction *act_objEdit = new QAction(menu_context);
    QAction *act_objDetails = new QAction(menu_context);
    QMenu *menu_addto = new QMenu(menu_context);

    QAction *act_objShowExplorer = new QAction(menu_context);
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

    act_objShowExplorer->setText("Show in Explorer");
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
    menu_context->addAction(act_objShowExplorer);
    menu_context->addSeparator();
    menu_context->addAction(act_treeSelectAll);
    menu_context->addAction(act_treeSelectNone);
    menu_context->addSeparator();
    menu_context->addAction(act_libImportSongs);
    menu_context->addAction(act_libDownloadSongs);
    menu_context->addSeparator();
    menu_context->addAction(act_objRemove);
    menu_context->addAction(act_objDelete);

//    qDebug() << tree_library->selectionModel()->selectedIndexes().size();
//    qDebug() << tree_library->selectionModel()->selectedRows().size();



//    Mpi3Song *song = nullptr;
//    QModelIndex index = tree_songlist->indexAt(point);
//    if (index.isValid()){
//        QString pid = m_modelSonglist->getPID(index);
//        song = m_library->getSong(pid);
//    }

//    if(song){
//        qDebug() << song->name;
//    }
//    else {
//        act_objPlay->setDisabled(true);
//        act_objEdit->setDisabled(true);
//        act_objDetails->setDisabled(true);
//        menu_addto->setDisabled(true);
//        act_editCopy->setDisabled(true);
//        act_editCut->setDisabled(true);
//        act_objShowExplorer->setDisabled(true);
//        act_objRemove->setDisabled(true);
//        act_objDelete->setDisabled(true);
//    }

    menu_context->exec(tree_songlist->mapToGlobal(point));
    delete menu_context;
}
void Mpi3RootDesktop::containersContextMenu(const QPoint &point){
    QMenu *menu_context = new QMenu(this);

    QAction *act_itemExpand = new QAction(menu_context);
    QAction *act_itemCollapse = new QAction(menu_context);
    QAction *act_itemExpandAll = new QAction(menu_context);
    QAction *act_itemCollapseAll = new QAction(menu_context);
    QMenu *menu_new = new QMenu(menu_context);
    QAction *act_newFolder = new QAction(menu_new);
    QAction *act_newPlaylist = new QAction(menu_new);
    QMenu *menu_import = new QMenu(menu_context);
    QAction *act_importPlaylists = new QAction(menu_import);
    QAction *act_importSongs = new QAction(menu_import);
    QAction *act_objEdit = new QAction(menu_context);
    QAction *act_objDetails = new QAction(menu_context);
    QAction *act_objDuplicate = new QAction(menu_context);
    QMenu *menu_moveto = new QMenu(menu_context);
    QAction *act_objDelete = new QAction(menu_context);

    act_itemExpand->setText("Expand");
    act_itemCollapse->setText("Collapse");
    act_itemExpandAll->setText("Expand All");
    act_itemCollapseAll->setText("Collapse All");
    menu_new->setTitle("New");
    act_newFolder->setText("New Folder");
    act_newPlaylist->setText("New Playlist");
    menu_import->setTitle("Import");
    act_importPlaylists->setText("Import Playlists");
    act_importSongs->setText("Import Songs");
    act_objEdit->setText("Edit");
    act_objDetails->setText("Details...");
    act_objDuplicate->setText("Duplicate");
    menu_moveto->setTitle("Move to...");
    act_objDelete->setText("Delete from Library");

    menu_context->addAction(act_itemExpand);
    menu_context->addAction(act_itemCollapse);
    menu_context->addSeparator();
    menu_context->addAction(act_itemExpandAll);
    menu_context->addAction(act_itemCollapseAll);
    menu_context->addSeparator();
    menu_new->addAction(act_newFolder);
    menu_new->addAction(act_newPlaylist);
    menu_context->addMenu(menu_new);
    menu_import->addAction(act_importPlaylists);
    menu_import->addAction(act_importSongs);
    menu_context->addMenu(menu_import);
    menu_context->addSeparator();
    menu_context->addAction(act_objDuplicate);
    menu_context->addAction(act_objEdit);
    menu_context->addAction(act_objDetails);
    menu_context->addMenu(menu_moveto);
    menu_context->addSeparator();
    menu_context->addAction(act_objDelete);

    QModelIndex index = tree_containers->indexAt(point);
    tree_containers->setCurrentIndex(index);

    Mpi3Folder *folder = nullptr;
    Mpi3Playlist *playlist = nullptr;
    if(index.isValid()){
        QString pid = m_modelContainers->getPID(index);
        folder = m_mediaLibrary->getFolder(pid);
        playlist = m_mediaLibrary->getPlaylist(pid);
    }
    else {
        tree_containers->clearSelection();
    }


    connect(act_itemExpand, &QAction::triggered, this, &Mpi3RootDesktop::itemExpand);
    connect(act_itemCollapse, &QAction::triggered, this, &Mpi3RootDesktop::itemCollapse);
    connect(act_itemExpandAll, &QAction::triggered, this, &Mpi3RootDesktop::itemExpandAll);
    connect(act_itemCollapseAll, &QAction::triggered, this, &Mpi3RootDesktop::itemCollapseAll);

    connect(act_newPlaylist, &QAction::triggered, this, &Mpi3RootDesktop::libNewPlaylist);
    connect(act_newFolder, &QAction::triggered, this, &Mpi3RootDesktop::libNewFolder);

    connect(act_importPlaylists, &QAction::triggered, this, &Mpi3RootDesktop::libImportPlaylists);
    connect(act_importSongs, &QAction::triggered, this, &Mpi3RootDesktop::libImportSongs);
    connect(act_objEdit, &QAction::triggered, this, &Mpi3RootDesktop::objEdit);
    connect(act_objDetails, &QAction::triggered, this, &Mpi3RootDesktop::objDetails);
    connect(act_objDuplicate, &QAction::triggered, this, &Mpi3RootDesktop::objDuplicate);

    connect(act_objDelete, &QAction::triggered, this, &Mpi3RootDesktop::objDelete);


    if(playlist){
//        qDebug() << "playlist:" << playlist->name;
        act_itemExpand->setDisabled(true);
        act_itemCollapse->setDisabled(true);

    }
    else if(folder) {
//        qDebug() << "folder:" << folder->name;

    }
    else {
        act_itemExpand->setDisabled(true);
        act_itemCollapse->setDisabled(true);
        act_objDuplicate->setDisabled(true);
        act_objEdit->setDisabled(true);
        act_objDetails->setDisabled(true);
        menu_moveto->setDisabled(true);
        act_objDelete->setDisabled(true);

    }

    menu_context->exec(tree_containers->mapToGlobal(point));
    delete menu_context;
}

void Mpi3RootDesktop::libraryViewChanged(){
    switch(m_panelLibview->currentView()) {

        case PanelLibrary::ViewAllSongs: {
            m_panelLibview->setDisplay("Library");
            m_modelSonglist->setDisplay(SonglistModel::DisplayAllSongs);
            break;
        }
        case PanelLibrary::ViewArtists: {
                m_panelLibview->setDisplay("Artists");
                m_modelSonglist->setDisplay(SonglistModel::DisplayArtists);
                break;
        }
        case PanelLibrary::ViewAlbums: {
                m_panelLibview->setDisplay("Albums");
                m_modelSonglist->setDisplay(SonglistModel::DisplayArtists);
                break;
        }
        case PanelLibrary::ViewContainer: {
                QModelIndex selectedIndex = tree_containers->selectionModel()->currentIndex();
                QString pidContainer = m_modelContainers->getPID(selectedIndex);

                Mpi3Playlist *playlist = m_mediaLibrary->getPlaylist(pidContainer);
                Mpi3Folder *folder = m_mediaLibrary->getFolder(pidContainer);

                if(playlist){
                    m_panelLibview->setDisplay(playlist->name());
                    m_modelSonglist->setContainer(playlist);
                }
                else if(folder){
                    m_panelLibview->setDisplay(folder->name());
                    m_modelSonglist->setContainer(folder);
                }

                break;
        }
    }
}
void Mpi3RootDesktop::selectionChanged(){
    int selectedContainers = tree_containers->selectionModel()->selectedIndexes().size();
    int selectedSongs = tree_songlist->selectionModel()->selectedIndexes().size();
    bool disable = selectedContainers + selectedSongs == 0;

    act_editCut->setDisabled(disable);
    act_editCopy->setDisabled(disable);
    act_editPaste->setDisabled(disable);
    act_editDelete->setDisabled(disable);
}
void Mpi3RootDesktop::libImport(){
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
void Mpi3RootDesktop::libExport(){
    QString libFile;
    QString pathDesktop(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    libFile = QFileDialog::getSaveFileName(this, "Export Mpi3Library File", pathDesktop, "Mpi3Lib Files (*.mpi3lib)");

    if(libFile != ""){
        m_mediaLibrary->save(libFile);
    }
}
void Mpi3RootDesktop::libReset(){
    m_mediaLibrary->libSongs->clear();
    m_mediaLibrary->libPlaylists->clear();
    m_mediaLibrary->libFolders->clear();
    m_mediaLibrary->save();

    m_modelContainers->setLibrary(m_mediaLibrary);
    m_modelSonglist->setLibrary(m_mediaLibrary);
    m_panelLibview->changeView(PanelLibrary::ViewAllSongs);
}

void Mpi3RootDesktop::themeSet(){
    QString qssFile;
    QString pathDesktop(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    qssFile = QFileDialog::getOpenFileName(this, "Open QSS Theme File", pathDesktop, "QSS Files (*.qss)");

    if(qssFile != ""){
        m_qssStyle->load(qssFile);
        themeRefresh();
    }
}
void Mpi3RootDesktop::themeRefresh(){
    if (m_qssStyle != nullptr){
        m_qssStyle->load();
        setStyleSheet(m_qssStyle->qssStyle());
    }
}

void Mpi3RootDesktop::setColumnVisibility(int column){
    bool hidden = m_modelSonglist->columnVisibility[column];
    tree_songlist->setColumnHidden(column, !hidden);
    m_modelSonglist->columnVisibility[column] = !hidden;
}

void Mpi3RootDesktop::itemExpand(){
    QModelIndex index = tree_containers->selectionModel()->currentIndex();
    tree_containers->expand(index);
}
void Mpi3RootDesktop::itemCollapse(){
    QModelIndex index = tree_containers->selectionModel()->currentIndex();
    tree_containers->collapse(index);
}
void Mpi3RootDesktop::itemExpandAll(){
    tree_containers->expandAll();
}
void Mpi3RootDesktop::itemCollapseAll(){
    tree_containers->collapseAll();
}

void Mpi3RootDesktop::libNewFolder(){
    QModelIndex currentIndex = tree_containers->currentIndex();
    if(currentIndex.isValid()){
        QString pid = m_modelContainers->getPID(currentIndex);
        Mpi3Playlist *currentPlaylist = m_mediaLibrary->getPlaylist(pid);

        if(currentPlaylist){
            currentIndex = currentIndex.parent();
        }
    }

    Mpi3Folder *parent = m_mediaLibrary->getFolder(m_modelContainers->getPID(currentIndex));
    Mpi3Folder *folder = m_mediaLibrary->newFolder(true);
    int position = parent ? parent->folders.size() : -1;

    m_mediaLibrary->insert(folder, parent, position);
    tree_containers->expand(currentIndex);
}
void Mpi3RootDesktop::libNewPlaylist(){
    QModelIndex currentIndex = tree_containers->currentIndex();
    if(currentIndex.isValid()){
        QString pid = m_modelContainers->getPID(currentIndex);
        Mpi3Playlist *currentPlaylist = m_mediaLibrary->getPlaylist(pid);

        if(currentPlaylist){
            currentIndex = currentIndex.parent();
        }
    }

    Mpi3Folder *parent = m_mediaLibrary->getFolder(m_modelContainers->getPID(currentIndex));
    Mpi3Playlist *playlist = m_mediaLibrary->newPlaylist(true);
    int position = parent ? parent->playlists.size() : -1;

    m_mediaLibrary->insert(playlist, parent, position);
    tree_containers->expand(currentIndex);
}
void Mpi3RootDesktop::libImportPlaylists(){
    QString plistFile;
    QString pathDesktop(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    plistFile = QFileDialog::getOpenFileName(this, "Open Itunes Plist File", pathDesktop, "XML Files (*.xml)");

    if(plistFile != ""){
        m_mediaLibrary->importItunesPlist(plistFile);
    }
}
void Mpi3RootDesktop::libImportSongs(){}
void Mpi3RootDesktop::libDownloadSongs(){}

void Mpi3RootDesktop::objPlay(){}
void Mpi3RootDesktop::objEdit(){}
void Mpi3RootDesktop::objDetails(){}
void Mpi3RootDesktop::objDelete(){
    if(tree_containers->selectionModel()->selectedRows().size() > 0){
        QModelIndex index = tree_containers->currentIndex();
        QString pid = m_modelContainers->getPID(index);

        Mpi3Folder *folder = m_mediaLibrary->getFolder(pid);
        Mpi3Playlist *playlist = m_mediaLibrary->getPlaylist(pid);
        if(folder){
            m_mediaLibrary->remove(folder);
        }
        else if(playlist){
            m_mediaLibrary->remove(playlist);
        }
    }
    else if(tree_songlist->selectionModel()->selectedRows().size() > 0){
        QModelIndexList currentIndexes = tree_songlist->selectionModel()->selectedIndexes();
        QStringList pids;

        for(int i = 0; i < currentIndexes.size(); i++){
            QModelIndex index = currentIndexes.at(i);
            QString pid = m_modelSonglist->getPID(index);
            if(!pids.contains(pid)){
                pids.append(pid);
            }
        }

        for(int i = 0; i < pids.size(); i++){
            QString pid = pids.at(i);
            Mpi3Song *song = m_mediaLibrary->getSong(pid);
            m_mediaLibrary->remove(song);
        }
    }
}

void Mpi3RootDesktop::objAddTo(){}
void Mpi3RootDesktop::objMoveTo(){}
void Mpi3RootDesktop::objRemoveFrom(){}
void Mpi3RootDesktop::objDuplicate(){}

void Mpi3RootDesktop::paintEvent(QPaintEvent *event){
    QStyleOption opt;
    opt.init(this);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}
void Mpi3RootDesktop::closeEvent(QCloseEvent *event){
    saveSettings();
    QMainWindow::closeEvent(event);
}
