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

    QWidget *window = new QWidget;
    QGridLayout *layout = new QGridLayout;

    frmPlayback = new PanelPlayback(this);
    frmViews = new PanelViews(this);
    frmTrees = new PanelTrees(this);

    layout->addWidget(frmPlayback, 0, 0, 1, 2);
    layout->addWidget(frmViews, 1, 0, 1, 1);
    layout->addWidget(frmTrees, 1, 1, 1, 1);

    layout->setColumnStretch(1, 1);
    layout->setRowStretch(0, 1);

    layout->setMargin(0);
//    connect(frmNav->btnSongs, &QPushButton::clicked, this, [this]{testButton("SONGS");});
//    connect(frmNav->btnArtists, &QPushButton::clicked, this, [this]{testButton("ARTISTS");});
//    connect(frmNav->btnPlaylists, &QPushButton::clicked, this, [this]{testButton("PLAYLISTS");});

    window->setObjectName("window");
//    window->setStyleSheet("QWidget#window {background-color: #000000}");
    window->setLayout(layout);
    setCentralWidget(window);
    window->show();
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













