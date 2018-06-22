#include "mplayback.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>
#include <QLabel>

#include <QDebug>


// ----------------------------------------------------------------------------------------------------
// * VolumeControl *
// ----------------------------------------------------------------------------------------------------
VolumeControl::VolumeControl(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layoutVolume = new QGridLayout;

    volSlider = new QSlider(this);
    volSlider->setOrientation(Qt::Horizontal);
    layoutVolume->addWidget(volSlider, 0, 0, 1, 1);

    this->setLayout(layoutVolume);
}

VolumeControl::~VolumeControl()
{

}


// ----------------------------------------------------------------------------------------------------
// * PlaybackControl *
// ----------------------------------------------------------------------------------------------------
PlaybackControl::PlaybackControl(QWidget *parent) : QWidget(parent)
{
    container = new QWidget(this);

    btnNext = new QPushButton(container);
    btnPrev = new QPushButton(container);
    btnPlay = new QPushButton(container);
    posSlider = new QSlider(container);

    posSlider->setOrientation(Qt::Horizontal);

    btnNext->setText("Next");
    btnPrev->setText("Prev");
    btnPlay->setText("Play");

    btnPlay->move(btnPrev->width(), 0);
    btnNext->move(btnPrev->width() + btnPlay->width(), 0);

    posSlider->setFixedHeight(20);
    posSlider->setGeometry(0, btnPrev->height(), 300, posSlider->height());
    posSlider->move(0, btnPrev->height());
}

PlaybackControl::~PlaybackControl()
{

}

void PlaybackControl::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

//    qDebug() << container->width();

//    int x = (this->width() / 2) - (container->width() / 2);
    int x = (this->width() / 2) - (300 / 2);
    container->move(x, 0);

}


// ----------------------------------------------------------------------------------------------------
// * LibrarySearchbar *
// ----------------------------------------------------------------------------------------------------
LibrarySearchbar::LibrarySearchbar(QWidget *parent) : QWidget(parent)
{
    boxSearch = new QLineEdit(this);
    btnSearch = new QPushButton(this);

    boxSearch->setFixedWidth(120);

    btnSearch->setText("S");
    btnSearch->setFixedWidth(20);

    QGridLayout *layoutSearchbar = new QGridLayout;

    layoutSearchbar->addWidget(boxSearch, 0, 0, 1, 1);
    layoutSearchbar->addWidget(btnSearch, 0, 1, 1, 1);
    this->setLayout(layoutSearchbar);
}

LibrarySearchbar::~LibrarySearchbar()
{

}


// ----------------------------------------------------------------------------------------------------
// * PanelPlayback *
// ----------------------------------------------------------------------------------------------------
PanelPlayback::PanelPlayback(QWidget *parent) : QWidget(parent)
{
    frmVolume = new VolumeControl(this);
    frmPlayback = new PlaybackControl(this);
    frmSearchbar = new LibrarySearchbar(this);

    QGridLayout *layoutMain = new QGridLayout;

    layoutMain->addWidget(frmVolume, 0, 0, 1, 1);
    layoutMain->addWidget(frmPlayback, 0, 1, 1, 1);
    layoutMain->addWidget(frmSearchbar, 0, 2, 1, 1);

    layoutMain->setColumnStretch(1, 1);

    layoutMain->setColumnMinimumWidth(0, 150);
    layoutMain->setColumnMinimumWidth(1, 400);
    layoutMain->setColumnMinimumWidth(2, 150);

    layoutMain->setRowMinimumHeight(0, 60);

    layoutMain->setMargin(0);


    this->setObjectName("PanelPlayback");
//    this->setStyleSheet("QWidget#PanelPlayback {"
//                        "background-color: #BABABA;"
//                        "border-bottom: 1px solid #000000}");

    this->setLayout(layoutMain);
}

PanelPlayback::~PanelPlayback()
{

}

void PanelPlayback::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}
