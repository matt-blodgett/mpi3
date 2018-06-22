#include "mroot.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>
#include <QMenuBar>
#include <QAction>

#include <QDebug>


Mpi3RootDesktop::Mpi3RootDesktop()
{
    initialize();
}

Mpi3RootDesktop::~Mpi3RootDesktop()
{

}

void Mpi3RootDesktop::initialize()
{

    QWidget *windowMain = new QWidget;
    setCentralWidget(windowMain);


    frmPlayback = new PanelPlayback(this);
    frmViews = new PanelViews(this);
    frmTrees = new PanelTrees(this);


    QGridLayout *layoutMain = new QGridLayout;

    layoutMain->addWidget(frmPlayback, 0, 0, 1, 2);
    layoutMain->addWidget(frmViews, 1, 0, 1, 1);
    layoutMain->addWidget(frmTrees, 1, 1, 1, 1);

    layoutMain->setColumnStretch(1, 1);
    layoutMain->setRowStretch(1, 1);

    layoutMain->setMargin(0);
    windowMain->setLayout(layoutMain);


//    connect(frmNav->btnSongs, &QPushButton::clicked, this, [this]{testButton("SONGS");});
//    connect(frmNav->btnArtists, &QPushButton::clicked, this, [this]{testButton("ARTISTS");});
//    connect(frmNav->btnPlaylists, &QPushButton::clicked, this, [this]{testButton("PLAYLISTS");});
//    frmTree->lblTreeview->setText(str);


    m_setTheme = new QAction("Set Theme", this);

    connect(m_setTheme, &QAction::triggered, this, &Mpi3RootDesktop::setTheme);

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_setTheme);


    windowMain->setObjectName("Mpi3RootDesktop");

    windowMain->setGeometry(200, 200, 1000, 400);

    windowMain->show();
}


void Mpi3RootDesktop::setTheme()
{
    qDebug() << "Hey";

}

void Mpi3RootDesktop::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}













