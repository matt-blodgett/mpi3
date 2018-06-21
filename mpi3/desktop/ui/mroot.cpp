#include "mroot.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>


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
    QGridLayout *layoutMain = new QGridLayout;

    frmPlayback = new PanelPlayback(this);
    frmViews = new PanelViews(this);
    frmTrees = new PanelTrees(this);

    layoutMain->addWidget(frmPlayback, 0, 0, 1, 2);
    layoutMain->addWidget(frmViews, 1, 0, 1, 1);
    layoutMain->addWidget(frmTrees, 1, 1, 1, 1);

    layoutMain->setColumnStretch(1, 1);
    layoutMain->setRowStretch(1, 1);

    layoutMain->setMargin(0);
//    connect(frmNav->btnSongs, &QPushButton::clicked, this, [this]{testButton("SONGS");});
//    connect(frmNav->btnArtists, &QPushButton::clicked, this, [this]{testButton("ARTISTS");});
//    connect(frmNav->btnPlaylists, &QPushButton::clicked, this, [this]{testButton("PLAYLISTS");});

    windowMain->setObjectName("window");
//    window->setStyleSheet("QWidget#window {background-color: #000000}");
    windowMain->setLayout(layoutMain);
    setCentralWidget(windowMain);

    windowMain->setGeometry(200, 200, 1000, 400);


    windowMain->show();
}

void Mpi3RootDesktop::testButton(QString const &str)
{
    Q_UNUSED(str);
//    frmTree->lblTreeview->setText(str);
}

void Mpi3RootDesktop::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}













