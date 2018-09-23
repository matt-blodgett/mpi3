#include "maudiocontrol.h"
#include "core/mmedialibrary.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>

#include <QPropertyAnimation>
#include <QPushButton>
#include <QLineEdit>
#include <QSlider>
#include <QLabel>

#include <QEvent>
#include <QTimer>


#include <QDebug>


MPanelPlayback::MPanelPlayback(QWidget *parent) : QWidget(parent){
    initializeLayout();

    m_currentState = Mpi3::EngineStopped;

    m_sldVolume->setRange(0, 100);

    m_fadeTimer = new QTimer(this);
    m_fadeTimer->setSingleShot(true);
    connect(m_fadeTimer, &QTimer::timeout, this, [this](){animateFadeButton();});

    connect(m_btnPlay, &QPushButton::released, this, &MPanelPlayback::clickedPlay);
    connect(m_btnNext, &QPushButton::released, this, &MPanelPlayback::clickedNext);
    connect(m_btnPrev, &QPushButton::released, this, &MPanelPlayback::clickedPrev);
    connect(m_sldVolume, &QSlider::valueChanged, this, &MPanelPlayback::volumeChanged);
}
MPanelPlayback::~MPanelPlayback(){}

void MPanelPlayback::initializeLayout(){

    // -------------------------------------------------- PIXMAPS

    m_pixNext = QPixmap(":/icons/playback/next-light.png");
    m_pixPrev = QPixmap(":/icons/playback/prev-light.png");
    m_pixPlay = QPixmap(":/icons/playback/play-light.png");
    m_pixPaus = QPixmap(":/icons/playback/paus-light.png");
    m_pixSearch = QPixmap(":/icons/playback/search-light.png");

    // -------------------------------------------------- VOLUME

    QWidget *frmVolume = new QWidget(this);

    m_sldVolume = new QSlider(this);

    QGridLayout *gridVolume = new QGridLayout(this);
    gridVolume->addWidget(m_sldVolume, 1, 0, 1, 1);
    gridVolume->setColumnStretch(0, 1);
    gridVolume->setRowMinimumHeight(1, 20);
    gridVolume->setHorizontalSpacing(0);
    gridVolume->setVerticalSpacing(0);
    gridVolume->setMargin(0);
    frmVolume->setLayout(gridVolume);

    m_sldVolume->setOrientation(Qt::Horizontal);

    // -------------------------------------------------- PLAYBACK

    int uHeight = 52;
    int uBtnWidth = 48;

    QWidget *frmPlayback = new QWidget(this);

    m_lblTitle = new QLabel(this);
    m_lblArtist = new QLabel(this);
    m_lblPositionMin = new QLabel(this);
    m_lblPositionMax = new QLabel(this);

    m_btnPlay = new QPushButton(this);
    m_btnNext = new QPushButton(this);
    m_btnPrev = new QPushButton(this);
    m_btnFade = new QPushButton(this);
    m_sldPosition = new QSlider(this);

    QGridLayout *gridControl = new QGridLayout(this);
    gridControl->addWidget(m_lblTitle, 0, 2, 1, 1);
    gridControl->addWidget(m_lblArtist, 1, 2, 3, 1);
    gridControl->addWidget(m_lblPositionMin,  3, 1, 1, 1);
    gridControl->addWidget(m_lblPositionMax, 3, 3, 1, 1);
    gridControl->addWidget(m_sldPosition, 4, 1, 1, 3);
    gridControl->addWidget(m_btnPrev, 0, 0, 5, 1);
    gridControl->addWidget(m_btnFade, 0, 1, 5, 3);
    gridControl->addWidget(m_btnNext, 0, 4, 5, 1);
    gridControl->setColumnMinimumWidth(1, 22);
    gridControl->setColumnMinimumWidth(3, 22);
    gridControl->setColumnStretch(0, 0);
    gridControl->setColumnStretch(1, 0);
    gridControl->setColumnStretch(2, 1);
    gridControl->setColumnStretch(3, 0);
    gridControl->setColumnStretch(4, 0);
    gridControl->setHorizontalSpacing(0);
    gridControl->setVerticalSpacing(0);
    gridControl->setMargin(0);
    frmPlayback->setLayout(gridControl);

    frmPlayback->setFixedHeight(uHeight-1);

    m_btnPrev->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_btnNext->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_btnFade->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    m_sldPosition->setFixedHeight(8);
    m_sldPosition->setOrientation(Qt::Horizontal);

    m_lblTitle->setAlignment(Qt::AlignCenter);
    m_lblArtist->setAlignment(Qt::AlignCenter);

    m_btnNext->setFixedWidth(uBtnWidth);
    m_btnPrev->setFixedWidth(uBtnWidth);

    m_btnNext->setIcon(QIcon(m_pixNext));
    m_btnPrev->setIcon(QIcon(m_pixPrev));
    m_btnFade->setIcon(QIcon(m_pixPlay));

    m_btnNext->setIconSize(QSize((uBtnWidth/12)*8, (uHeight/12)*8));
    m_btnPrev->setIconSize(QSize((uBtnWidth/12)*8, (uHeight/12)*8));
    m_btnFade->setIconSize(QSize((uBtnWidth/12)*8, (uHeight/12)*8));

    m_btnNext->setFlat(true);
    m_btnPrev->setFlat(true);
    m_btnFade->setFlat(true);

    m_btnPlay->setFixedWidth(32);
    m_btnPlay->setFixedHeight(32);
    m_btnPlay->setIconSize(QSize(22, 22));
    m_btnPlay->setIcon(QIcon(m_pixPlay));
    m_btnPlay->setFlat(true);

    // -------------------------------------------------- SEARCHBAR
    QWidget *frmSearchbar = new QWidget(this);

    m_boxSearch = new QLineEdit(this);
    m_btnSearch = new QPushButton(this);

    QGridLayout *gridSearchbar = new QGridLayout(this);
    gridSearchbar->addWidget(m_btnSearch, 1, 0, 1, 1);
    gridSearchbar->addWidget(m_boxSearch, 1, 1, 1, 1);
    gridSearchbar->setColumnStretch(1, 1);
    gridSearchbar->setRowStretch(0, 1);
    gridSearchbar->setRowStretch(2, 1);
    gridSearchbar->setRowMinimumHeight(1, 20);
    gridSearchbar->setHorizontalSpacing(0);
    gridSearchbar->setVerticalSpacing(0);
    gridSearchbar->setMargin(0);
    frmSearchbar->setLayout(gridSearchbar);

    m_btnSearch->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_boxSearch->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_boxSearch->setTextMargins(0, 0, 0, 0);

    m_btnSearch->setIcon(QIcon(m_pixSearch));
    m_btnSearch->setIconSize(QSize(20, 20));
    m_btnSearch->setFlat(true);

    // -------------------------------------------------- PLAYBACK PANEl

    QGridLayout *gridMain = new QGridLayout(this);
    gridMain->addWidget(frmVolume, 0, 1, 1, 1);
    gridMain->addWidget(m_btnPlay, 0, 3, 1, 1);
    gridMain->addWidget(frmPlayback, 0, 5, 1, 1);
    gridMain->addWidget(frmSearchbar, 0, 9, 1, 1);
    gridMain->setRowMinimumHeight(0, uHeight);
    gridMain->setColumnMinimumWidth(0, 20);
    gridMain->setColumnMinimumWidth(1, 150);
    gridMain->setColumnMinimumWidth(2, 10);
    gridMain->setColumnMinimumWidth(3, 40);
    gridMain->setColumnMinimumWidth(4, 10);
    gridMain->setColumnMinimumWidth(5, 400);
    gridMain->setColumnMinimumWidth(6, 10);
    gridMain->setColumnMinimumWidth(7, 40);
    gridMain->setColumnMinimumWidth(8, 10);
    gridMain->setColumnMinimumWidth(9, 150);
    gridMain->setColumnMinimumWidth(10, 20);
    gridMain->setColumnStretch(0, 0);
    gridMain->setColumnStretch(1, 0);
    gridMain->setColumnStretch(2, 1);
    gridMain->setColumnStretch(3, 0);
    gridMain->setColumnStretch(4, 1);
    gridMain->setColumnStretch(5, 6);
    gridMain->setColumnStretch(6, 1);
    gridMain->setColumnStretch(7, 0);
    gridMain->setColumnStretch(8, 1);
    gridMain->setColumnStretch(9, 0);
    gridMain->setColumnStretch(10, 0);
    gridMain->setHorizontalSpacing(0);
    gridMain->setVerticalSpacing(0);
    gridMain->setMargin(0);
    setLayout(gridMain);

    // -------------------------------------------------- OBJECT NAMES

    m_lblTitle->setObjectName("DisplayLabelSongTitle");
    m_lblArtist->setObjectName("DisplayLabelSongArtist");
    m_lblPositionMax->setObjectName("DisplayLabelSongPosition");
    m_lblPositionMin->setObjectName("DisplayLabelSongPosition");
    m_btnNext->setObjectName("ButtonNext");
    m_btnPrev->setObjectName("ButtonPrev");
    m_btnPlay->setObjectName("ButtonPlay");
    m_btnFade->setObjectName("ButtonFade");
    m_sldVolume->setObjectName("SliderVolume");
    m_sldPosition->setObjectName("SliderPosition");
    frmPlayback->setObjectName("PlaybackControl");
    m_btnSearch->setObjectName("ButtonSearch");
    m_boxSearch->setObjectName("BoxSearch");
    setObjectName("PanelPlayback");
}

double MPanelPlayback::buttonOpacity() const{
    return m_btnOpacity;
}
void MPanelPlayback::setButtonOpacity(double opacity){
    m_btnOpacity = opacity;

    QPixmap pixCurrent = playing() ? m_pixPlay : m_pixPaus;

    if(m_btnOpacity > 0.00){
        QPixmap pixMask = QPixmap(pixCurrent);

        QImage imgMask(pixMask.size(), QImage::Format_ARGB32_Premultiplied);
        imgMask.fill(Qt::transparent);

        QPainter painter(&imgMask);
        painter.setOpacity(m_btnOpacity / 100.00);
        painter.drawPixmap(0, 0, pixMask);
        painter.end();

        QIcon icnMask(QPixmap::fromImage(imgMask));
        m_btnFade->setIcon(icnMask);
    }
    else {
        m_btnFade->setVisible(false);
        m_btnFade->setIcon(QIcon(pixCurrent));
    }
}
void MPanelPlayback::animateFadeButton(){
    if(!stopped()){
        QPropertyAnimation *animation = new QPropertyAnimation(this);
        animation->setPropertyName("buttonOpacity");
        animation->setTargetObject(this);
        animation->setStartValue(100.00);
        animation->setEndValue(0.00);
        animation->setDuration(700);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

int MPanelPlayback::volume() const{
    return m_sldVolume->value();
}
double MPanelPlayback::position() const {
    return m_sldPosition->value();
}
bool MPanelPlayback::stopped() const {
    return m_currentState == Mpi3::EngineStopped;
}
bool MPanelPlayback::playing() const {
    return m_currentState == Mpi3::EngineActive;
}
bool MPanelPlayback::paused() const {
    return m_currentState == Mpi3::EngineIdle;
}

void MPanelPlayback::setVolume(int volume){
    m_sldVolume->blockSignals(true);
    m_sldVolume->setValue(volume);
    m_sldVolume->blockSignals(false);
}
void MPanelPlayback::setPosition(double position){
    m_sldPosition->blockSignals(true);
    m_sldPosition->setValue(static_cast<int>(position));
    m_lblPositionMin->setText(MMediaLibrary::timeToString(position));
    m_sldPosition->blockSignals(false);
}
void MPanelPlayback::setState(Mpi3::EngineState state){

    if(state == Mpi3::EngineActive){
        m_btnPlay->setIcon(QIcon(m_pixPaus));
        m_btnFade->setIcon(QIcon(m_pixPlay));
    }
    else if(state == Mpi3::EngineIdle) {
        m_btnPlay->setIcon(QIcon(m_pixPlay));
        m_btnFade->setIcon(QIcon(m_pixPaus));
    }

    if(!m_navigating && state != m_currentState){
        m_btnFade->setVisible(true);
        m_fadeTimer->start(300);
    }

    if(state != Mpi3::EngineStopped){
        m_navigating = false;
    }

    m_currentState = state;
}
void MPanelPlayback::setDisplay(MSong *song){
    if(song){
        m_sldPosition->setMinimum(0);
        m_sldPosition->setMaximum(static_cast<int>(song->time()));
        m_lblTitle->setText(song->name());
        m_lblArtist->setText(song->artist());
        m_lblPositionMax->setText(song->time_str());
        m_lblPositionMin->setText("0:00");
    }
}

void MPanelPlayback::clickedPlay(){
    emit paused() || stopped() ? audioPlay() : audioPause();
}
void MPanelPlayback::clickedNext(){
    m_navigating = true;
    emit navigateNext();
}
void MPanelPlayback::clickedPrev(){
    m_navigating = true;
    emit navigatePrev();
}
void MPanelPlayback::volumeChanged(){
    emit changeVolume(volume());
}

void MPanelPlayback::elementModified(MMediaElement *elemModified){
    if(elemModified->type() == Mpi3::SongElement){
        MSong *sc_song = static_cast<MSong*>(elemModified);
        if(sc_song->pid() == m_pidCurrentSong){
            setDisplay(sc_song);
        }
    }
}

void MPanelPlayback::paintEvent(QPaintEvent *event){
    QStyleOption opt;
    opt.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QWidget::paintEvent(event);
}





