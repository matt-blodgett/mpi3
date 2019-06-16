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


//    m_mediaLibrary->newPlaylist(m_mediaLibrary->folders().at(1));

//    m_mediaLibrary->reset();

//    MFolder *folder1 = m_mediaLibrary->newFolder();
//    MFolder *folder2 = m_mediaLibrary->newFolder();

//    folder2->setParentFolder(folder1);

//    m_mediaLibrary->newPlaylist();
//    m_mediaLibrary->newPlaylist(folder1);


//    MFolder *f = m_mediaLibrary->newFolder();
//    m_mediaLibrary->newPlaylist(f);


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
    QMenuBar *menuMain = menuBar();

    QAction *actAudioSettings = new QAction(menuMain);
    QAction *actWndExit = new QAction(menuMain);

    QAction *actLibImport = new QAction(menuMain);
    QAction *actLibExport = new QAction(menuMain);
    QAction *actLibBackup = new QAction(menuMain);
    QAction *actLibReset = new QAction(menuMain);
    QAction *actLibOpen = new QAction(menuMain);
    QAction *actLibNewFolder = new QAction(menuMain);
    QAction *actLibNewPlaylist = new QAction(menuMain);
    QAction *actLibImportPlaylists = new QAction(menuMain);

    QAction *actThemeSet = new QAction(menuMain);
    QAction *actThemeRefresh = new QAction(menuMain);

    QAction *actEditUndo = new QAction(menuMain);
    QAction *actEditRedo = new QAction(menuMain);

    QAction *actEditCut = new QAction(this);
    QAction *actEditCopy = new QAction(this);
    QAction *actEditPaste = new QAction(this);
    QAction *actEditDelete = new QAction(this);
    QAction *actTreeSelectAll = new QAction(this);
    QAction *actTreeSelectNone = new QAction(this);

    QAction *actRaspiConnect = new QAction(menuMain);

    QAction *actToolsOptions = new QAction(menuMain);

    QAction *actHelpAbout = new QAction(menuMain);

    actAudioSettings->setText("Audio Settings");
    actWndExit->setText("Exit");

    actLibImport->setText("Import Library");
    actLibExport->setText("Export Library");
    actLibBackup->setText("Backup Library");
    actLibReset->setText("Reset Library");
    actLibOpen->setText("Open File Location");
    actLibNewFolder->setText("New Folder");
    actLibNewPlaylist->setText("New Playlist");
    actLibImportPlaylists->setText("Import Playlists");

    actThemeSet->setText("Set Theme");
    actThemeRefresh->setText("Refresh");

    actEditUndo->setText("Undo");
    actEditRedo->setText("Redo");
    actEditCut->setText("Cut");
    actEditCopy->setText("Copy");
    actEditPaste->setText("Paste");
    actEditDelete->setText("Delete");
    actTreeSelectAll->setText("Select All");
    actTreeSelectNone->setText("Select None");

    actRaspiConnect->setText("Connect");

    actToolsOptions->setText("Options");

    actHelpAbout->setText("About");

    QMenu *menuFile = new QMenu(menuMain);
    QMenu *menuLibrary = new QMenu(menuMain);
    QMenu *menuTheme = new QMenu(menuMain);

    QMenu *menuEdit = new QMenu(menuMain);
    QMenu *menuDevice = new QMenu(menuMain);
    QMenu *menuTools = new QMenu(menuMain);
    QMenu *menuHelp = new QMenu(menuMain);

    menuFile->setTitle("File");
    menuLibrary->setTitle("Library");
    menuTheme->setTitle("Themes");
    menuEdit->setTitle("Edit");
    menuDevice->setTitle("Device");
    menuTools->setTitle("Tools");
    menuHelp->setTitle("Help");

    menuMain->addMenu(menuFile);
    menuLibrary->addAction(actLibImport);
    menuLibrary->addAction(actLibExport);
    menuLibrary->addAction(actLibBackup);
    menuLibrary->addSeparator();
    menuLibrary->addAction(actLibReset);
    menuLibrary->addAction(actLibOpen);
    menuLibrary->addSeparator();
    menuLibrary->addAction(actLibNewFolder);
    menuLibrary->addAction(actLibNewPlaylist);
    menuLibrary->addSeparator();
    menuLibrary->addAction(actLibImportPlaylists);

    menuTheme->addAction(actThemeSet);
    menuTheme->addAction(actThemeRefresh);

    menuFile->addAction(actAudioSettings);
    menuFile->addMenu(menuLibrary);
    menuFile->addMenu(menuTheme);
    menuFile->addSeparator();
    menuFile->addAction(actWndExit);

    menuMain->addMenu(menuEdit);
    menuEdit->addAction(actEditUndo);
    menuEdit->addAction(actEditRedo);
    menuEdit->addSeparator();
    menuEdit->addAction(actEditCut);
    menuEdit->addAction(actEditCopy);
    menuEdit->addAction(actEditPaste);
    menuEdit->addAction(actEditDelete);
    menuEdit->addSeparator();
    menuEdit->addAction(actTreeSelectAll);
    menuEdit->addAction(actTreeSelectNone);

    menuMain->addMenu(menuDevice);
    menuDevice->addAction(actRaspiConnect);

    menuMain->addMenu(menuTools);
    menuTools->addAction(actToolsOptions);

    menuMain->addMenu(menuHelp);
    menuHelp->addAction(actHelpAbout);

//    connect(act_audioSettings
    connect(actWndExit, &QAction::triggered, this, [this]() {window()->close();});

    connect(actLibImport, &QAction::triggered, m_panelLibrary, &MPanelLibrary::askLibraryImport);
    connect(actLibExport, &QAction::triggered, m_panelLibrary, &MPanelLibrary::askLibraryExport);
    connect(actLibBackup, &QAction::triggered, m_panelLibrary, &MPanelLibrary::backupLibrary);
    connect(actLibReset, &QAction::triggered, m_panelLibrary, &MPanelLibrary::resetLibrary);

    connect(actLibOpen, &QAction::triggered, [=]() {MActions::openFileLocation(m_mediaLibrary->savePath());});
    connect(actLibNewPlaylist, &QAction::triggered, m_panelMedia->frameContainers(), &MFrameContainers::newPlaylist);
    connect(actLibNewFolder, &QAction::triggered, m_panelMedia->frameContainers(), &MFrameContainers::newFolder);
    connect(actLibImportPlaylists, &QAction::triggered, m_panelMedia->frameContainers(), &MFrameContainers::importPlaylists);

    connect(actThemeSet, &QAction::triggered, this, [this]() {setTheme();});
    connect(actThemeRefresh, &QAction::triggered, this, [this]() {refreshTheme();});

//    connect(act_editUndo, &QAction::triggered, this, [this]() {editUndo();});
//    connect(act_editRedo, &QAction::triggered, this, [this]() {editRedo();});

    connect(actEditCut, &QAction::triggered, m_panelMedia->frameSonglist(), &MFrameSonglist::cutItems);
    connect(actEditCopy, &QAction::triggered, m_panelMedia->frameSonglist(), &MFrameSonglist::copyItems);
    connect(actEditPaste, &QAction::triggered, m_panelMedia->frameSonglist(), &MFrameSonglist::pasteItems);

//    MTreeSonglist *treeSonglist = m_panelMedia->frameSonglist()->tree();
//    QItemSelectionModel *treeSonglistItems = m_panelMedia->frameSonglist()->tree()->selectionModel();
//    connect(act_treeSelectAll, &QAction::triggered, this, [=](){treeSonglist->selectAll();});
//    connect(act_treeSelectNone, &QAction::triggered, this, [=](){treeSonglistItems->clear();});

//    connect(act_raspiConnect

//    connect(act_toolsOptions

//    connect(act_helpAbout

//    connect(treeSonglistItems, &QItemSelectionModel::selectionChanged, act_editCut,
//            [=](){act_editCut->setDisabled(!treeSonglistItems->selectedRows().size());});
//    connect(treeSonglistItems, &QItemSelectionModel::selectionChanged, act_editCopy,
//            [=](){act_editCopy->setDisabled(!treeSonglistItems->selectedRows().size());});
//    connect(treeSonglistItems, &QItemSelectionModel::selectionChanged, act_editDelete,
//            [=](){act_editDelete->setDisabled(!treeSonglistItems->selectedRows().size());});

    actEditCut->setDisabled(true);
    actEditCopy->setDisabled(true);
    actEditDelete->setDisabled(true);

    QAction *act_RefreshThemeDirect = new QAction(this);
    act_RefreshThemeDirect->setText("Refresh Theme");
    connect(act_RefreshThemeDirect, &QAction::triggered, this, &MRootDesktop::refreshTheme);
    menuMain->addAction(act_RefreshThemeDirect);
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

    // Need to create the temp file if it doesn't exist
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
