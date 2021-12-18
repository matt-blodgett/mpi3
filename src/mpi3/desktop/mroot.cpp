#include "mpi3/desktop/mroot.h"
#include "mpi3/desktop/ui/frames/mframecontextbar.h"
#include "mpi3/desktop/ui/frames/mframeplayback.h"
#include "mpi3/desktop/ui/frames/mframetreeview.h"
#include "mpi3/desktop/ui/panels/mpanellibrary.h"
#include "mpi3/desktop/ui/panels/mpaneldevice.h"
#include "mpi3/desktop/ui/panels/mpanelmedia.h"
#include "mpi3/desktop/ui/widgets/mtreeview.h"
#include "mpi3/desktop/ui/models/mmodelsonglist.h"
#include "mpi3/desktop/ui/mstyle.h"
#include "mpi3/desktop/ui/mactions.h"
#include "mpi3/core/mmedialibrary.h"
#include "mpi3/util/msettings.h"
#include "mpi3/util/mstylesheet.h"

#include <QStyleOption>
#include <QPainter>

#include <QGridLayout>
#include <QFileDialog>
#include <QMenuBar>

#include <QGuiApplication>
#include <QApplication>
#include <QScreen>

#include <QMediaPlayer>
#include <QAudioOutput>


#include <QDebug>


MRootDesktop::MRootDesktop()
{
    Q_INIT_RESOURCE(desktop);
    Q_INIT_RESOURCE(fonts);
}
MRootDesktop::~MRootDesktop()
{
    m_mediaPlayer->stop();
    delete m_mediaPlayer;
    delete m_styleSheet;
    delete m_mediaLibrary;
    delete m_settingsProfile;
}

void MRootDesktop::initialize()
{
    Mpi3::initialize();
    MStyle::initialize();
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
    m_mediaPlayer = new QMediaPlayer(this);

    m_framePlayback = new MFramePlayback(this);
    m_frameContextBar = new MFrameContextBar(this);

    m_panelMedia = new MPanelMedia(this);
    m_panelLibrary = new MPanelLibrary(this);
    m_panelDevice = new MPanelDevice(this);

    QAudioOutput *audioOutput = new QAudioOutput(this);
    m_mediaPlayer->setAudioOutput(audioOutput);

    connect(m_framePlayback, &MFramePlayback::playRequested, m_mediaPlayer, &QMediaPlayer::play);
    connect(m_framePlayback, &MFramePlayback::pauseRequested, m_mediaPlayer, &QMediaPlayer::pause);
    connect(m_framePlayback, &MFramePlayback::volumeRequested, audioOutput, &QAudioOutput::setVolume);
    connect(m_framePlayback, &MFramePlayback::seekRequested, m_mediaPlayer, &QMediaPlayer::setPosition);

    connect(audioOutput, &QAudioOutput::volumeChanged, m_framePlayback, &MFramePlayback::setVolume);
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged, m_framePlayback, &MFramePlayback::setPosition);
    connect(m_mediaPlayer, &QMediaPlayer::playbackStateChanged, m_framePlayback, &MFramePlayback::setState);
    connect(m_mediaLibrary, &MMediaLibrary::songChanged, m_framePlayback, &MFramePlayback::songChanged);

    connect(m_panelMedia->frameSonglist(), &MFrameSonglist::currentPlayingSongChanged, this, &MRootDesktop::setPlaybackSong);

    // This causes an issue where if it hasn't been pushed yet it will select the first row and play it.
    // Should only do this when app opens and no songs have playedy yet.
    connect(m_framePlayback, &MFramePlayback::playRequestedInitial, this, &MRootDesktop::setPlaybackSongInitial);
    connect(m_framePlayback, &MFramePlayback::nextRequested, m_panelMedia->frameSonglist(), &MFrameSonglist::playItemNext);
    connect(m_framePlayback, &MFramePlayback::prevRequested, m_panelMedia->frameSonglist(), &MFrameSonglist::playItemPrev);

    connect(m_frameContextBar, &MFrameContextBar::contextPanelChanged, this, &MRootDesktop::setContextPanel);
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
    QWidget *windowMain = new QWidget(this);
    QGridLayout *gridMain = new QGridLayout(windowMain);
    gridMain->addWidget(m_framePlayback, 0, 0, 1, 1);
    gridMain->addWidget(m_frameContextBar, 1, 0, 1, 1);
    gridMain->addWidget(m_panelMedia, 2, 0, 1, 1);
    gridMain->addWidget(m_panelLibrary, 2, 0, 1, 1);
    gridMain->addWidget(m_panelDevice, 2, 0, 1, 1);
    gridMain->setColumnStretch(0, 1);
    gridMain->setRowStretch(2, 1);
    gridMain->setContentsMargins(0, 0, 0, 0);
    gridMain->setHorizontalSpacing(0);
    gridMain->setVerticalSpacing(0);
    windowMain->setLayout(gridMain);

    setMinimumHeight(300);
    setMinimumWidth(700);
    setCentralWidget(windowMain);
//    setWindowIcon(QIcon(":/icons/window/mpi3.ico"));

    m_panelLibrary->hide();
    m_panelDevice->hide();
}
void MRootDesktop::initializeState()
{
    // TODO: Add debugging
    if (!QDir(MActions::pathAppData()).exists()) {
        bool created = QDir().mkpath(MActions::pathAppData());
        qDebug() << created << MActions::pathAppData();
    }
    if(!QFile::exists(MActions::pathProfile())) {
        QFile(":/profiles/default.xml").copy(MActions::pathProfile());
        QFile(MActions::pathProfile()).setPermissions(QFile::WriteOwner | QFile::ReadOwner);
    }

    m_settingsProfile = new MSettingsXml(MActions::pathProfile());

    m_settingsProfile->beginGroup("RootWindow");
    QString stylePath = m_settingsProfile->value("style", ":/styles/default.qss").toString();
    QString libraryPath = m_settingsProfile->value("library").toString();
    int context = m_settingsProfile->value("context").toInt();
    float volume = m_settingsProfile->value("volume", 0.5).toFloat();
    m_settingsProfile->beginGroup("WindowGeometry");
    QScreen *screen = QGuiApplication::screenAt(pos());
    int d_rootx = (screen->availableGeometry().width() / 2) - 400;
    int d_rooty = (screen->availableGeometry().height() / 2) - 300;
    int wnd_rootx = m_settingsProfile->value("rootx", d_rootx).toInt();
    int wnd_rooty = m_settingsProfile->value("rooty", d_rooty).toInt();
    int wnd_width = m_settingsProfile->value("width", 800).toInt();
    int wnd_height = m_settingsProfile->value("height", 600).toInt();
    bool wnd_maximized = m_settingsProfile->value("maximized", false).toBool();
    m_settingsProfile->endGroup();
    m_settingsProfile->endGroup();

    if(wnd_maximized) {
        showMaximized();
    }
    else {
        move(wnd_rootx, wnd_rooty);
        resize(wnd_width, wnd_height);
    }

    m_styleSheet->load(stylePath);
    setStyleSheet(m_styleSheet->qssStyle());

    m_framePlayback->setVolume(volume);
    m_mediaPlayer->audioOutput()->setVolume(volume);
    m_frameContextBar->changeView(static_cast<MFrameContextBar::View>(context));

    // Need to create the temp file if it doesn't exist
    if(!QFile::exists(libraryPath)){
        libraryPath = MActions::pathLibraryDefault();
        m_mediaLibrary->save(libraryPath);
    }

    m_mediaLibrary->load(libraryPath);
    m_panelMedia->setLibrary(m_mediaLibrary);
    m_panelLibrary->setLibrary(m_mediaLibrary);
    m_panelDevice->setLibrary(m_mediaLibrary);

    m_panelMedia->load(m_settingsProfile);
    m_panelLibrary->load(m_settingsProfile);
    m_panelDevice->load(m_settingsProfile);

    m_settingsProfile->sync();
}
void MRootDesktop::saveSettings()
{
    m_settingsProfile->beginGroup("RootWindow");
    m_settingsProfile->setValue("style", m_styleSheet->qssPath());
    m_settingsProfile->setValue("library", m_mediaLibrary->savePath());
    m_settingsProfile->setValue("context", m_frameContextBar->currentView());
    m_settingsProfile->setValue("volume", m_framePlayback->volume());
    m_settingsProfile->beginGroup("WindowGeometry");
    m_settingsProfile->setValue("rootx", x());
    m_settingsProfile->setValue("rooty", y());
    m_settingsProfile->setValue("width", width());
    m_settingsProfile->setValue("height", height());
    m_settingsProfile->setValue("maximized", isMaximized());
    m_settingsProfile->endGroup();
    m_settingsProfile->endGroup();

    m_panelMedia->save(m_settingsProfile);
    m_panelLibrary->save(m_settingsProfile);
    m_panelDevice->save(m_settingsProfile);

    m_mediaLibrary->save();

    m_settingsProfile->sync();
}

void MRootDesktop::setContextPanel()
{
    switch(m_frameContextBar->currentView()) {

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
void MRootDesktop::setPlaybackSongInitial()
{
    if (m_panelMedia->frameSonglist()->model()->rowCount() > 0) {
        QModelIndex idxRowZero = m_panelMedia->frameSonglist()->model()->index(0, 0);
        m_panelMedia->frameSonglist()->tree()->selectionModel()->setCurrentIndex(idxRowZero, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        m_panelMedia->frameSonglist()->playItemSelected();
    }
}
void MRootDesktop::setPlaybackSong(const QString &pid)
{
    MSong *song = m_mediaLibrary->getSong(pid);
    if(song) {
        m_framePlayback->setSong(song);
        m_mediaPlayer->stop();
        m_mediaPlayer->setSource(QUrl::fromLocalFile(song->path()));
        m_mediaPlayer->play();
    }
}
void MRootDesktop::setTheme()
{
    QString title = "Open QSS Theme File";
    QString files = "QSS Files (*.qss)";
    QString path = QFileDialog::getOpenFileName(this, title, MActions::pathDesktop(), files);

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
