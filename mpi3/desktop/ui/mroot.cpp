#include "mroot.h"
#include "mplayback.h"
#include "mlibrary.h"
#include "util/mtheme.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>
#include <QMenuBar>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>
#include <QMediaPlayer>


#include <QDebug>


// ----------------------------------------------------------------------------------------------------
// * Mpi3RootDesktop *
// ----------------------------------------------------------------------------------------------------
Mpi3RootDesktop::Mpi3RootDesktop()
{
    initializeMenubar();
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
    m_library = new PanelLibrary(this);

    layoutMain->addWidget(m_playback, 0, 0, 1, 1);
    layoutMain->addWidget(m_library, 1, 0, 1, 1);
    layoutMain->setColumnStretch(0, 1);
    layoutMain->setRowStretch(1, 1);
    layoutMain->setMargin(0);
    layoutMain->setHorizontalSpacing(0);
    layoutMain->setVerticalSpacing(0);

    windowMain->setLayout(layoutMain);
    windowMain->setGeometry(200, 200, 1000, 600);



    this->m_audio = new QMediaPlayer(this);
    this->m_audio->setAudioRole(QAudio::MusicRole);


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

    this->m_audio->setMedia(QUrl(p));
//    this->m_audio->setVolume(50);
//    this->m_audio->play();






    m_theme = new Mpi3Theme;
//    qDebug() << QDir::currentPath();
//    QString qssPath = QDir::currentPath() + "/qss/default.qss";
    m_theme->load(":/desktop/qss/default.qss");
    this->setStyleSheet(m_theme->qssStyle);

    windowMain->show();
}

void Mpi3RootDesktop::initializeMenubar()
{
    menuBar()->setObjectName("MainMenuBar");

    menu_theme = new QMenu("Themes", this);

    m_themeSet = new QAction("Set Theme", this);
    m_themeRefresh = new QAction("Refresh", this);

    connect(m_themeSet, &QAction::triggered, this, &Mpi3RootDesktop::themeSet);
    connect(m_themeRefresh, &QAction::triggered, this, &Mpi3RootDesktop::themeRefresh);

    menu_theme->addAction(m_themeSet);
    menu_theme->addAction(m_themeRefresh);

    menu_file = menuBar()->addMenu("File");
    menu_file->addMenu(menu_theme);
}

void Mpi3RootDesktop::themeSet()
{
    QString qssFile;
    QString pathDesktop(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));

    qssFile = QFileDialog::getOpenFileName(
                this, "Open QSS Theme File",
                pathDesktop, "QSS Files (*.qss)");

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

void Mpi3RootDesktop::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}
