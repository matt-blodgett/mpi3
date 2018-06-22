#include "mpanels.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>


PanelViews::PanelViews(QWidget *parent) : QWidget(parent)
{
    this->setObjectName("PanelViews");

    QGridLayout *layout = new QGridLayout;

    btnSongs = new QPushButton(this);
    btnArtists = new QPushButton(this);
    btnPlaylists = new QPushButton(this);

    btnSongs->setText("Songs");
    btnArtists->setText("Artists");
    btnPlaylists->setText("Playlists");

    layout->addWidget(btnSongs, 0, 0, 1, 1);
    layout->addWidget(btnArtists, 1, 0, 1, 1);
    layout->addWidget(btnPlaylists, 2, 0, 1, 1);

    layout->setRowStretch(3, 1);

    layout->setMargin(0);

    this->setLayout(layout);
}

PanelViews::~PanelViews()
{

}


void PanelViews::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}
