#include "mroot.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>
#include <QMenuBar>

#include <QStandardPaths>
#include <QFileDialog>
#include <QFile>

#include <QDebug>


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

    frmPlayback = new PanelPlayback(this);
    frmViews = new PanelViews(this);
    frmTrees = new PanelTrees(this);

    QGridLayout *layoutMain = new QGridLayout;

    frmLibrary = new QSplitter(this);
    frmLibrary->setOrientation(Qt::Horizontal);
    frmLibrary->addWidget(frmViews);
    frmLibrary->addWidget(frmTrees);
    frmLibrary->setHandleWidth(15);


    layoutMain->addWidget(frmPlayback, 0, 0, 1, 1);
    layoutMain->addWidget(frmLibrary, 1, 0, 1, 1);

    layoutMain->setColumnStretch(0, 1);
    layoutMain->setRowStretch(1, 1);

    layoutMain->setMargin(0);
    windowMain->setLayout(layoutMain);

//    connect(frmNav->btnSongs, &QPushButton::clicked, this, [this]{testButton("SONGS");});
//    connect(frmNav->btnArtists, &QPushButton::clicked, this, [this]{testButton("ARTISTS");});
//    connect(frmNav->btnPlaylists, &QPushButton::clicked, this, [this]{testButton("PLAYLISTS");});
//    frmTree->lblTreeview->setText(str);


    windowMain->setGeometry(200, 200, 1000, 400);

    themeCurrent = QString(":/desktop/qss/default.qss");
    themeRefresh();

    windowMain->show();
}

void Mpi3RootDesktop::initializeMenubar()
{
    menuBar()->setObjectName("MainMenuBar");

    menuTheme = new QMenu("Themes", this);

    m_themeSet = new QAction("Set Theme", this);
    m_themeRefresh = new QAction("Refresh", this);

    connect(m_themeSet, &QAction::triggered, this, &Mpi3RootDesktop::themeSet);
    connect(m_themeRefresh, &QAction::triggered, this, &Mpi3RootDesktop::themeRefresh);

    menuTheme->addAction(m_themeSet);
    menuTheme->addAction(m_themeRefresh);

    menuFile = menuBar()->addMenu("File");
    menuFile->addMenu(menuTheme);
}

void Mpi3RootDesktop::themeSet()
{
    QString qssFile;
    QString pathDesktop(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));

    qssFile = QFileDialog::getOpenFileName(
                this, "Open QSS Theme File",
                pathDesktop, "QSS Files (*.qss)");

    if(qssFile != ""){
        themeCurrent = qssFile;
        themeRefresh();
    }
}

void Mpi3RootDesktop::themeRefresh()
{
    if (themeCurrent != ""){
        qDebug() << themeCurrent;

        QFile qssFile(themeCurrent);
        qssFile.open(QFile::ReadOnly);

        QString qssStyle(qssFile.readAll());
        this->setStyleSheet(qssStyle);
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
