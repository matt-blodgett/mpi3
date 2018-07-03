#include "mroot.h"
#include "mplayback.h"
#include "mlibrary.h"
#include "mlibmodel.h"

#include "util/mtheme.h"
#include "util/medialib.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>

#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>

#include <QMediaPlayer>
#include <QTreeView>
#include <QMenuBar>

#include <QDebug>


Mpi3RootDesktop::Mpi3RootDesktop()
{
    initializeActions();
    initializeMainMenu();
}

Mpi3RootDesktop::~Mpi3RootDesktop()
{

}

void Mpi3RootDesktop::initialize()
{
    this->setObjectName("Mpi3RootDesktop");

    QWidget *windowMain = new QWidget;
    setCentralWidget(windowMain);

    QGridLayout *layoutMain = new QGridLayout;

    m_playback = new PanelPlayback(this);
    m_libview = new PanelLibrary(this);

    layoutMain->addWidget(m_playback, 0, 0, 1, 1);
    layoutMain->addWidget(m_libview, 1, 0, 1, 1);
    layoutMain->setColumnStretch(0, 1);
    layoutMain->setRowStretch(1, 1);
    layoutMain->setMargin(0);
    layoutMain->setHorizontalSpacing(0);
    layoutMain->setVerticalSpacing(0);

    windowMain->setLayout(layoutMain);
    windowMain->setGeometry(200, 200, 1000, 600);



    m_audio = new QMediaPlayer(this);
    m_audio->setAudioRole(QAudio::MusicRole);


    m_playback->setState(m_audio->state());
    m_playback->setVolume(m_audio->volume());


    connect(m_playback, &PanelPlayback::play, m_audio, &QMediaPlayer::play);
    connect(m_playback, &PanelPlayback::pause, m_audio, &QMediaPlayer::pause);
    connect(m_playback, &PanelPlayback::stop, m_audio, &QMediaPlayer::stop);
//    connect(m_playback, &PanelPlayback::next, m_playlist, &QMediaPlaylist::next);
//    connect(m_playback, &PanelPlayback::previous, this, &PanelPlayback::previousClicked);
    connect(m_playback, &PanelPlayback::changeVolume, m_audio, &QMediaPlayer::setVolume);

    connect(m_audio, &QMediaPlayer::stateChanged, m_playback, &PanelPlayback::setState);
    connect(m_audio, &QMediaPlayer::volumeChanged, m_playback, &PanelPlayback::setVolume);
    connect(m_audio, &QMediaPlayer::mutedChanged, m_playback, &PanelPlayback::setMuted);



//  QString p("file:///C:/Users/Matt/Desktop/Prayer in C.mp3");
//  QString p("C:/Users/Matt/Desktop/Prayer in C.mp3");
    QString p("C:/Users/Matt/Desktop/Calm Down.wav");

    m_audio->setMedia(QUrl(p));
//    this->m_audio->setVolume(50);
//    this->m_audio->play();



    tree_library = this->findChild<QTreeView*>("LibraryTreeview");



    QStringList headers;
    headers << "Name" << "Artist" << "Path";

    m_model = new LibraryModel(this, headers);
    tree_library->setModel(m_model);


//    QString p("C:/Users/Matt/Desktop/Calm Down.wav");

//    m_playlist->addMedia(QUrl(p));


    Mpi3Library *mpi3Lib = new Mpi3Library(true);

    mpi3Lib->name = "Main Library";
    mpi3Lib->added = "03/07/2017";


    //    mpi3Lib->save("C:\\Users\\Matt\\Desktop\\lib.txt");


    m_model->setLibrary(mpi3Lib);

    Mpi3Song *song_1 = mpi3Lib->addSong();
    song_1->name = "Me, Myself and I";
    song_1->artist = "G-Eazy";
    song_1->path = "F:\\iTunes\\Music\\G-Eazy\\Unknown Album\\Me, Myself  I (Ft. Bebe Rexha).mp3";

    mpi3Lib->update(song_1);

    Mpi3Song *song_2 = mpi3Lib->addSong();
    song_2->name = "Been On";
    song_2->artist = "G-Eazy";
    song_2->path = "F:\\iTunes\\Music\\G-Eazy\\Unknown Album\\Been On.mp3";

    mpi3Lib->update(song_2);


    Mpi3Song *song_3 = mpi3Lib->addSong();
    song_3->name = "Change My Heart";
    song_3->artist = "Ozcan x Laurell";
    song_3->path = "F:\\iTunes\\Music\\Ozcan x Laurell\\Unknown Album\\Change My Heart.mp3";

    mpi3Lib->update(song_3);


    Mpi3Song *song_4 = mpi3Lib->addSong();
    song_4->name = "Little Moment";
    song_4->artist = "Omar LinX";
    song_4->path = "F:\\iTunes\\Music\\Omar LinX\\Unknown Album\\Little Moment.mp3";

    mpi3Lib->update(song_4);



    //    Mpi3Folder *fldr_1 = mpi3Lib->addFolder();
    //    Mpi3Playlist *plist_1 = mpi3Lib->addPlaylist();

    //    fldr_1->name = "electric beat";
    //    plist_1->name = "upbeat";

    //    Mpi3Playlist *plist_2 = mpi3Lib->addPlaylist(fldr_1);
    //    plist_2->name = "dance";




    tree_library->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tree_library, &QTreeView::customContextMenuRequested, this, &Mpi3RootDesktop::treeviewContextMenu);




    m_theme = new Mpi3Theme;
//    qDebug() << QDir::currentPath();
//    QString qssPath = QDir::currentPath() + "/qss/default.qss";
    m_theme->load(":/desktop/qss/default.qss");
    this->setStyleSheet(m_theme->qssStyle);

    windowMain->show();
}

void Mpi3RootDesktop::initializeActions()
{
    act_audioSettings = new QAction(this);
    act_wndExit = new QAction(this);

    act_wndExit->setText("Exit");
    act_audioSettings->setText("Audio Settings");

    // connect
    // connect

    act_libImport = new QAction(this);
    act_libExport = new QAction(this);

    act_libImport->setText("Import Library");
    act_libExport->setText("Export Library");

    connect(act_libImport, &QAction::triggered, this, &Mpi3RootDesktop::libImport);
    connect(act_libExport, &QAction::triggered, this, &Mpi3RootDesktop::libExport);

    act_themeSet = new QAction(this);
    act_themeRefresh = new QAction(this);

    act_themeSet->setText("Set Theme");
    act_themeRefresh->setText("Refresh");

    connect(act_themeSet, &QAction::triggered, this, &Mpi3RootDesktop::themeSet);
    connect(act_themeRefresh, &QAction::triggered, this, &Mpi3RootDesktop::themeRefresh);

    act_itemExpand = new QAction(this);
    act_itemCollapse = new QAction(this);
    act_itemExpandAll = new QAction(this);
    act_itemCollapseAll = new QAction(this);

    act_libDelete = new QAction(this);
    act_libNewFolder = new QAction(this);
    act_libNewPlaylist = new QAction(this);
    act_libImportPlaylists = new QAction(this);
    act_libImportSongs = new QAction(this);
    act_libDownloadSongs = new QAction(this);

    act_objPlay = new QAction(this);
    act_objEdit = new QAction(this);
    act_objDetails = new QAction(this);

    act_objAddTo = new QAction(this);
    act_objMoveTo = new QAction(this);
    act_objRemoveFrom = new QAction(this);
    act_objDuplicate = new QAction(this);

    act_itemExpand->setText("Expand");
    act_itemCollapse->setText("Collapse");
    act_itemExpandAll->setText("Expand All");
    act_itemCollapseAll->setText("Collapse All");

    act_libDelete->setText("Delete from Library");
    act_libNewFolder->setText("New Folder");
    act_libNewPlaylist->setText("New Playlist");
    act_libImportPlaylists->setText("Import Playlist");

    act_objPlay->setText("Play");
    act_objEdit->setText("Edit");
    act_objDetails->setText("Details");

    act_objAddTo->setText("Add to...");
    act_objMoveTo->setText("Move to...");
    act_objRemoveFrom->setText("Remove from Playlist");
    act_objDuplicate->setText("Duplicate");

    connect(act_itemExpand, &QAction::triggered, this, &Mpi3RootDesktop::itemExpand);
    connect(act_itemCollapse, &QAction::triggered, this, &Mpi3RootDesktop::itemCollapse);
    connect(act_itemExpandAll, &QAction::triggered, this, &Mpi3RootDesktop::itemExpandAll);
    connect(act_itemCollapseAll, &QAction::triggered, this, &Mpi3RootDesktop::itemCollapseAll);

    connect(act_libDelete, &QAction::triggered, this, &Mpi3RootDesktop::libDelete);
    connect(act_libNewFolder, &QAction::triggered, this, &Mpi3RootDesktop::libNewFolder);
    connect(act_libNewPlaylist, &QAction::triggered, this, &Mpi3RootDesktop::libNewPlaylist);
    connect(act_libImportPlaylists, &QAction::triggered, this, &Mpi3RootDesktop::libImportPlaylists);

    connect(act_objPlay, &QAction::triggered, this, &Mpi3RootDesktop::objPlay);
    connect(act_objEdit, &QAction::triggered, this, &Mpi3RootDesktop::objEdit);
    connect(act_objDetails, &QAction::triggered, this, &Mpi3RootDesktop::objDetails);

    connect(act_objAddTo, &QAction::triggered, this, &Mpi3RootDesktop::objAddTo);
    connect(act_objMoveTo, &QAction::triggered, this, &Mpi3RootDesktop::objMoveTo);
    connect(act_objRemoveFrom, &QAction::triggered, this, &Mpi3RootDesktop::objRemoveFrom);
    connect(act_objDuplicate, &QAction::triggered, this, &Mpi3RootDesktop::objDuplicate);
}


void Mpi3RootDesktop::initializeMainMenu()
{
    menuBar()->setObjectName("MainMenuBar");

    QMenu *menu_file = new QMenu(this);
    QMenu *menu_edit = new QMenu(this);
    QMenu *menu_view = new QMenu(this);
    QMenu *menu_device = new QMenu(this);
    QMenu *menu_options = new QMenu(this);
    QMenu *menu_help = new QMenu(this);

    menu_file->setTitle("File");
    menu_edit->setTitle("Edit");
    menu_view->setTitle("View");
    menu_device->setTitle("Device");
    menu_options->setTitle("Options");
    menu_help->setTitle("Help");

    menuBar()->addMenu(menu_file);
    menuBar()->addMenu(menu_edit);
    menuBar()->addMenu(menu_view);
    menuBar()->addMenu(menu_device);
    menuBar()->addMenu(menu_options);
    menuBar()->addMenu(menu_help);

    QMenu *menu_library = new QMenu(this);
    QMenu *menu_theme = new QMenu(this);

    menu_library->setTitle("Library");
    menu_theme->setTitle("Themes");

    menu_library->addAction(act_libImport);
    menu_library->addAction(act_libExport);
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

}

void Mpi3RootDesktop::treeviewContextMenu(const QPoint &point)
{
    QModelIndex index = tree_library->indexAt(point);
    if (index.isValid()){
        QMenu contextMenu(this);

        qDebug() <<  this->children().size();

        contextMenu.addAction(act_itemExpand);
        contextMenu.addAction(act_itemCollapse);
        contextMenu.addAction(act_itemExpandAll);
        contextMenu.addAction(act_itemCollapseAll);

        contextMenu.addSeparator();

        contextMenu.addAction(act_libDelete);
        contextMenu.addAction(act_libNewFolder);
        contextMenu.addAction(act_libNewPlaylist);
        contextMenu.addAction(act_libImportPlaylists);

        contextMenu.addSeparator();

        contextMenu.addAction(act_objPlay);
        contextMenu.addAction(act_objEdit);
        contextMenu.addAction(act_objDetails);

        contextMenu.addSeparator();

        contextMenu.addAction(act_objAddTo);
        contextMenu.addAction(act_objMoveTo);
        contextMenu.addAction(act_objRemoveFrom);
        contextMenu.addAction(act_objDuplicate);



        contextMenu.exec(tree_library->mapToGlobal(point));
    }
}








void Mpi3RootDesktop::libImport()
{

}

void Mpi3RootDesktop::libExport()
{

}


void Mpi3RootDesktop::themeSet()
{
    QString qssFile;
    QString pathDesktop(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    qssFile = QFileDialog::getOpenFileName(this, "Open QSS Theme File", pathDesktop, "QSS Files (*.qss)");

    if(qssFile != ""){
        m_theme->load(qssFile);
        themeRefresh();
    }
}

void Mpi3RootDesktop::themeRefresh()
{
    if (m_theme != nullptr){
        m_theme->load();
        this->setStyleSheet(m_theme->qssStyle);
    }
}


void Mpi3RootDesktop::itemExpand()
{

}
void Mpi3RootDesktop::itemCollapse()
{

}
void Mpi3RootDesktop::itemExpandAll()
{

}
void Mpi3RootDesktop::itemCollapseAll()
{

}

void Mpi3RootDesktop::libDelete()
{

}
void Mpi3RootDesktop::libNewFolder()
{

}

void Mpi3RootDesktop::libNewPlaylist()
{

}
void Mpi3RootDesktop::libImportPlaylists()
{

}

void Mpi3RootDesktop::objPlay()
{

}
void Mpi3RootDesktop::objEdit()
{

}
void Mpi3RootDesktop::objDetails()
{

}

void Mpi3RootDesktop::objAddTo()
{

}
void Mpi3RootDesktop::objMoveTo()
{

}
void Mpi3RootDesktop::objRemoveFrom()
{

}
void Mpi3RootDesktop::objDuplicate()
{

}





















void Mpi3RootDesktop::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}













