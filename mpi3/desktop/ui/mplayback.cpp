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

    volSlider->setObjectName("SliderVolume");
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

    QPixmap pixmapNext(":/desktop/icons/next.png");
    QPixmap pixmapPrev(":/desktop/icons/prev.png");
    QPixmap pixmapPlay(":/desktop/icons/play.png");
    QPixmap pixmapPaus(":/desktop/icons/paus.png");

    icnNext.addPixmap(pixmapNext);
    icnPrev.addPixmap(pixmapPrev);
    icnPlay.addPixmap(pixmapPlay);
    icnPaus.addPixmap(pixmapPaus);

    btnNext->setIcon(icnNext);
    btnPrev->setIcon(icnPrev);
    btnPlay->setIcon(icnPlay);

    btnNext->setIconSize(pixmapNext.rect().size());
    btnPrev->setIconSize(pixmapPrev.rect().size());
    btnPlay->setIconSize(pixmapPlay.rect().size());

    posSlider->setOrientation(Qt::Horizontal);
    btnNext->setFlat(true);
    btnPrev->setFlat(true);
    btnPlay->setFlat(true);

    int h = 60;
    int w = 300;
    int w_btn = 50;

    container->setFixedHeight(h);
    container->setFixedWidth(w);

    btnNext->setFixedHeight(h-1);
    btnPrev->setFixedHeight(h-1);
    btnPlay->setFixedHeight(h-20);

    btnNext->setFixedWidth(w_btn);
    btnPrev->setFixedWidth(w_btn);
    btnPlay->setFixedWidth(w-(w_btn*2));

    btnPrev->move(0, 0);
    btnNext->move(w-w_btn, 0);
    btnPlay->move(w_btn, 0);

    posSlider->setFixedWidth(w-(w_btn*2));
    posSlider->setFixedHeight(12);
    posSlider->move(w_btn, 45);

    connect(btnPlay, &QPushButton::clicked, this, [this]{clickPlay();});

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

void PlaybackControl::clickPlay()
{
    // https://evileg.com/en/post/155/
    this->btnPlay->setIcon(this->icnPaus);


    this->audio = new QMediaPlayer;

//    this->audio->setMedia(QUrl("C:/Users/mablodgett/Desktop/Calm Down.mp3"));
    this->audio->setMedia(QUrl::fromLocalFile("C:/Users/mablodgett/Desktop/Calm Down.mp3"));
//    this->audio->setVolume(50);
//    this->audio->play();
}

void PlaybackControl::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

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
