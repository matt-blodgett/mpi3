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

    volSlider->setObjectName("SliderVolume");
    this->setLayout(layoutVolume);

    this->setObjectName("VolumeControl");
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

    QPixmap pixmapNext(":/desktop/icons/next.png");
    QIcon icnNext(pixmapNext);
    btnNext->setIcon(icnNext);
    btnNext->setIconSize(pixmapNext.rect().size());

    QPixmap pixmapPrev(":/desktop/icons/prev.png");
    QIcon icnPrev(pixmapPrev);
    btnPrev->setIcon(icnPrev);
    btnPrev->setIconSize(pixmapPrev.rect().size());

    QPixmap pixmapPlay(":/desktop/icons/play.png");
    QIcon icnPlay(pixmapPlay);
    btnPlay->setIcon(icnPlay);
    btnPlay->setIconSize(pixmapPlay.rect().size());


    int h = 60;
    int w = 300;

    container->setFixedHeight(h);
    container->setFixedWidth(w);

    btnNext->setFixedHeight(h);
    btnPrev->setFixedHeight(h);
    btnPlay->setFixedHeight(h);

    btnNext->setFixedWidth(50);
    btnPrev->setFixedWidth(50);
    btnPlay->setFixedWidth(50);

    btnPrev->move(0, 0);
    btnNext->move(w-50, 0);
    btnPlay->move((w/2)-(50/2), 0);


    posSlider->setGeometry(0, btnPrev->height(), 300, posSlider->height());
    posSlider->move(0, btnPrev->height());

    posSlider->setFixedWidth(300);
    posSlider->setFixedHeight(12);


    btnNext->setObjectName("ButtonNext");
    btnPrev->setObjectName("ButtonPrev");
    btnPlay->setObjectName("ButtonPlay");
    posSlider->setObjectName("SliderPosition");

    container->setObjectName("PlaybackControl");
    this->setObjectName("PlaybackControl");
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

    this->setObjectName("LibrarySearchbar");
}

LibrarySearchbar::~LibrarySearchbar()
{

}


// ----------------------------------------------------------------------------------------------------
// * PanelPlayback *
// ----------------------------------------------------------------------------------------------------
PanelPlayback::PanelPlayback(QWidget *parent) : QWidget(parent)
{
    this->setObjectName("PanelPlayback");

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
