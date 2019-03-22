#include "mroot.h"
#include "mstyle.h"
#include "mactions.h"
#include "msettings.h"

#include "mstylesheet.h"
#include "mmedialibrary.h"
#include "maudioengine.h"

#include "mframecontextbar.h"
#include "mframeplayback.h"
#include "mframetreeview.h"
#include "mtreeview.h"

#include "mpanellibrary.h"
#include "mpaneldevice.h"
#include "mpanelmedia.h"
#include "mmenubar.h"

#include <QStyleOption>
#include <QPainter>

#include <QGridLayout>
#include <QFileDialog>
#include <QMenuBar>


#include <QEvent>
#include <QMouseEvent>


#include <QDebug>


MRootDesktop::MRootDesktop()
{
    Q_INIT_RESOURCE(desktop);
    Q_INIT_RESOURCE(fonts);
}
MRootDesktop::~MRootDesktop()
{
    m_audioEngine->stop();
    delete m_audioEngine;
    delete m_mediaLibrary;
    delete m_styleSheet;
    MAudioEngine::deinitialize();
}

void MRootDesktop::initialize()
{
    Mpi3::initialize();
    MStyle::initialize();
    MAudioEngine::initialize();

    initializeObjects();
    initializeMainMenu();
    initializeLayout();
    initializeState();
    centralWidget()->show();
}

void MRootDesktop::initializeObjects()
{
    m_styleSheet = new MStyleSheet();
    m_mediaLibrary = new MMediaLibrary(this);
    m_audioEngine = new MAudioEngine(this);

//    connect(m_audioEngine, &MAudioEngine::notifyMediaStatus, this, &MRootDesktop::processAudioMediaStatus);
//    connect(m_audioEngine, &MAudioEngine::notifyEngineStatus, this, &MRootDesktop::processAudioEngineStatus);
//    connect(m_audioEngine, &MAudioEngine::notifyErrorStatus, this, &MRootDesktop::processAudioErrorStatus);
//    connect(m_audioEngine, &MAudioEngine::notifyRequestStatus, this, &MRootDesktop::processAudioRequestStatus);

    m_panelPlayback = new MFramePlayback(this);

    connect(m_panelPlayback, &MFramePlayback::playRequested, m_audioEngine, &MAudioEngine::play);
    connect(m_panelPlayback, &MFramePlayback::pausRequested, m_audioEngine, &MAudioEngine::pause);
    connect(m_panelPlayback, &MFramePlayback::volumeRequested, m_audioEngine, &MAudioEngine::gain);
    connect(m_panelPlayback, &MFramePlayback::seekRequested, m_audioEngine, &MAudioEngine::seek);

    connect(m_audioEngine, &MAudioEngine::notifyVolume, m_panelPlayback, &MFramePlayback::setVolume);
    connect(m_audioEngine, &MAudioEngine::notifyPosition, m_panelPlayback, &MFramePlayback::setPosition);
    connect(m_audioEngine, &MAudioEngine::notifyEngineStatus, m_panelPlayback, &MFramePlayback::setState);
//    connect(m_mediaLibrary, &MMediaLibrary::elementModified, m_panelPlayback, &MFramePlayback::elementModified);

    m_panelMedia = new MPanelMedia(this);
//    connect(m_panelPlayback, &MPanelPlayback::nextRequested, m_treeSonglist, &MTreeSonglist::playNextItem);
//    connect(m_panelPlayback, &MPanelPlayback::prevRequested, m_treeSonglist, &MTreeSonglist::playPrevItem);

//    connect(m_treeSonglist, &MTreeSonglist::playbackChanged, m_panelPlayback, &MPanelPlayback::setDisplay);
//    connect(m_treeSonglist, &MTreeSonglist::playbackChanged, this, &MRootDesktop::setPlaybackSong);

    m_panelLibrary = new MPanelLibrary(this);
    m_panelDevice = new MPanelDevice(this);
    m_contextBar = new MFrameContextBar(this);

    connect(m_contextBar, &MFrameContextBar::viewChanged, this, &MRootDesktop::setContextPanel);
}
void MRootDesktop::initializeMainMenu()
{
    QMenuBar *menu_main = menuBar();

    QAction *act_audioSettings = new QAction(menu_main);
    QAction *act_wndExit = new QAction(menu_main);

    QAction *act_libImport = new QAction(menu_main);
    QAction *act_libExport = new QAction(menu_main);
    QAction *act_libBackup = new QAction(menu_main);
    QAction *act_libReset = new QAction(menu_main);
    QAction *act_libOpen = new QAction(menu_main);
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
    act_libBackup->setText("Backup Library");
    act_libReset->setText("Reset Library");
    act_libOpen->setText("Open File Location");
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
    menu_library->addAction(act_libBackup);
    menu_library->addSeparator();
    menu_library->addAction(act_libReset);
    menu_library->addAction(act_libOpen);
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
    connect(act_wndExit, &QAction::triggered, this, [this]() {window()->close();});

    connect(act_libImport, &QAction::triggered, m_panelLibrary, &MPanelLibrary::askLibraryImport);
    connect(act_libExport, &QAction::triggered, m_panelLibrary, &MPanelLibrary::askLibraryExport);
    connect(act_libBackup, &QAction::triggered, m_panelLibrary, &MPanelLibrary::backupLibrary);
    connect(act_libReset, &QAction::triggered, m_panelLibrary, &MPanelLibrary::resetLibrary);

    connect(act_libOpen, &QAction::triggered, [=]() {MActions::openFileLocation(m_mediaLibrary->savePath());});
    connect(act_libNewPlaylist, &QAction::triggered, m_panelMedia->frameContainers(), &MFrameContainers::newPlaylist);
    connect(act_libNewFolder, &QAction::triggered, m_panelMedia->frameContainers(), &MFrameContainers::newFolder);
    connect(act_libImportPlaylists, &QAction::triggered, m_panelMedia->frameContainers(), &MFrameContainers::importPlaylists);
    connect(act_libImportSongs, &QAction::triggered, m_panelMedia->frameContainers(), &MFrameContainers::importSongs);
    connect(act_libDownloadSongs, &QAction::triggered, m_panelMedia->frameContainers(), &MFrameContainers::downloadSongs);

    connect(act_themeSet, &QAction::triggered, this, [this]() {setTheme();});
    connect(act_themeRefresh, &QAction::triggered, this, [this]() {refreshTheme();});

//    connect(act_editUndo, &QAction::triggered, this, [this]() {editUndo();});
//    connect(act_editRedo, &QAction::triggered, this, [this]() {editRedo();});

    connect(act_editCut, &QAction::triggered, m_panelMedia->frameSonglist(), &MFrameSonglist::cutItems);
    connect(act_editCopy, &QAction::triggered, m_panelMedia->frameSonglist(), &MFrameSonglist::copyItems);
    connect(act_editPaste, &QAction::triggered, m_panelMedia->frameSonglist(), &MFrameSonglist::pasteItems);

    MTreeSonglist *treeSonglist = m_panelMedia->frameSonglist()->tree();
    QItemSelectionModel *treeSonglistItems = m_panelMedia->frameSonglist()->tree()->selectionModel();
    connect(act_treeSelectAll, &QAction::triggered, this, [=](){treeSonglist->selectAll();});
    connect(act_treeSelectNone, &QAction::triggered, this, [=](){treeSonglistItems->clear();});

//    connect(act_raspiConnect

//    connect(act_toolsOptions

//    connect(act_helpAbout

    connect(treeSonglistItems, &QItemSelectionModel::selectionChanged, act_editCut,
            [=](){act_editCut->setDisabled(!treeSonglistItems->selectedRows().size());});
    connect(treeSonglistItems, &QItemSelectionModel::selectionChanged, act_editCopy,
            [=](){act_editCopy->setDisabled(!treeSonglistItems->selectedRows().size());});
    connect(treeSonglistItems, &QItemSelectionModel::selectionChanged, act_editDelete,
            [=](){act_editDelete->setDisabled(!treeSonglistItems->selectedRows().size());});

    act_editCut->setDisabled(true);
    act_editCopy->setDisabled(true);
    act_editDelete->setDisabled(true);

    QAction *act_RefreshThemeDirect = new QAction(this);
    act_RefreshThemeDirect->setText("Refresh Theme");
    connect(act_RefreshThemeDirect, &QAction::triggered, this, &MRootDesktop::refreshTheme);
    menu_main->addAction(act_RefreshThemeDirect);
}
void MRootDesktop::initializeLayout()
{
    m_menuBar = new MMenuBar(this);

    QWidget *windowMain = new QWidget(this);
    QGridLayout *gridMain = new QGridLayout(windowMain);
    gridMain->addWidget(m_panelPlayback, 0, 0, 1, 1);
    gridMain->addWidget(m_contextBar, 1, 0, 1, 1);
    gridMain->addWidget(m_panelMedia, 2, 0, 1, 1);
    gridMain->addWidget(m_panelLibrary, 2, 0, 1, 1);
    gridMain->addWidget(m_panelDevice, 2, 0, 1, 1);
    gridMain->setColumnStretch(0, 1);
    gridMain->setRowStretch(2, 1);
    gridMain->setHorizontalSpacing(0);
    gridMain->setVerticalSpacing(0);
    gridMain->setMargin(0);
    windowMain->setLayout(gridMain);

    setMinimumHeight(300);
    setMinimumWidth(700);
    setContentsMargins(1, 1, 1, 1);
    setCentralWidget(windowMain);
    setMenuWidget(m_menuBar);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setWindowIcon(QIcon(":/icons/window/mpi3.ico"));

    m_panelLibrary->hide();
    m_panelDevice->hide();
}
void MRootDesktop::initializeState()
{
    if(!QFile::exists(MActions::pathProfile())) {
        QFile(":/profiles/default.xml").copy(MActions::pathProfile());
        QFile(MActions::pathProfile()).setPermissions(QFile::WriteOwner);
    }

    MSettingsXml settings(MActions::pathProfile());

    settings.beginGroup("RootWindow");
    QString stylePath = settings.value("style", ":/styles/default.qss").toString();
    QString libraryPath = settings.value("library").toString();
    int context = settings.value("context").toInt();
    int volume = settings.value("volume", 50).toInt();
    m_menuBar->load(&settings);
    settings.endGroup();

    m_styleSheet->load(stylePath);
    setStyleSheet(m_styleSheet->qssStyle());

    m_panelPlayback->setVolume(volume);
    m_audioEngine->gain(m_panelPlayback->volume());
    m_contextBar->changeView(static_cast<MFrameContextBar::View>(context));

    if(!QFile::exists(libraryPath)){
        libraryPath = MActions::pathLibraryDefault();
        m_mediaLibrary->save(libraryPath);
    }

    m_mediaLibrary->load(libraryPath);
    m_panelMedia->setLibrary(m_mediaLibrary);
    m_panelLibrary->setLibrary(m_mediaLibrary);
    m_panelDevice->setLibrary(m_mediaLibrary);

    m_panelMedia->load(&settings);
    m_panelLibrary->load(&settings);
    m_panelDevice->load(&settings);
}
void MRootDesktop::saveSettings()
{
    QDir().remove(MActions::pathProfile());
    MSettingsXml *settings = new MSettingsXml(MActions::pathProfile());

    settings->beginGroup("RootWindow");
    settings->setValue("style", m_styleSheet->qssPath());
    settings->setValue("library", m_mediaLibrary->savePath());
    settings->setValue("context", m_contextBar->currentView());
    settings->setValue("volume", m_panelPlayback->volume());
    m_menuBar->save(settings);
    settings->endGroup();

    m_panelMedia->save(settings);
    m_panelLibrary->save(settings);
    m_panelDevice->save(settings);

    m_mediaLibrary->save();
}

void MRootDesktop::setContextPanel()
{
    switch(m_contextBar->currentView()) {

        case MFrameContextBar::ViewMedia: {
            m_panelMedia->show();
            m_panelLibrary->hide();
            m_panelDevice->hide();
            break;
        }
        case MFrameContextBar::ViewLibrary: {
            m_panelMedia->hide();
            m_panelLibrary->show();
            m_panelDevice->hide();
            break;
        }
        case MFrameContextBar::ViewDevice: {
            m_panelMedia->hide();
            m_panelLibrary->hide();
            m_panelDevice->show();
            break;
        }
    }
}
void MRootDesktop::setPlaybackSong(MSong *song)
{
    if(song) {
        m_audioEngine->stop();
        m_audioEngine->open(song->path());
        m_audioEngine->start();
    }
}

void MRootDesktop::setTheme()
{
    QString title = "Open QSS Theme File";
    QString files = "QSS Files (*.qss)";
    QString path = QFileDialog::getOpenFileName(
        this, title, MActions::pathDesktop(), files);

    if(path != "") {
        m_styleSheet->load(path);
        refreshTheme();
    }
}
void MRootDesktop::refreshTheme()
{
    m_styleSheet->load();
    setStyleSheet(m_styleSheet->qssStyle());
}

void MRootDesktop::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QWidget::paintEvent(event);
}
void MRootDesktop::closeEvent(QCloseEvent *event)
{
    saveSettings();
    QMainWindow::closeEvent(event);
}
