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

    m_currentState = Mpi3::EngineState::EngineStopped;

    m_sldVolume->setRange(0, 100);

    m_fadeTimer = new QTimer(this);
    m_fadeTimer->setSingleShot(true);

    m_btnFade->installEventFilter(this);
    m_frmControls->installEventFilter(this);

    connect(m_fadeTimer, &QTimer::timeout, this, [this](){animateFadeButton();});

    connect(m_btnPlay, &QPushButton::released, this, &MPanelPlayback::clickedPlay);
    connect(m_btnNext, &QPushButton::released, this, &MPanelPlayback::clickedNext);
    connect(m_btnPrev, &QPushButton::released, this, &MPanelPlayback::clickedPrev);
    connect(m_sldVolume, &QSlider::valueChanged, this, &MPanelPlayback::volumeChanged);

    m_lblTitle->setObjectName("SongDisplayLabelTitle");
    m_lblArtist->setObjectName("SongDisplayLabelArtist");
    m_lblPositionMax->setObjectName("SongDisplayLabelPosition");
    m_lblPositionMin->setObjectName("SongDisplayLabelPosition");
    m_btnNext->setObjectName("ButtonNext");
    m_btnPrev->setObjectName("ButtonPrev");
    m_btnPlay->setObjectName("ButtonPlay");
    m_btnFade->setObjectName("ButtonFade");
    m_sldVolume->setObjectName("SliderVolume");
    m_sldPosition->setObjectName("SliderPosition");
    m_frmControls->setObjectName("PlaybackControl");
    m_btnSearch->setObjectName("ButtonSearch");
    m_boxSearch->setObjectName("BoxSearch");
    setObjectName("PanelPlayback");
}
MPanelPlayback::~MPanelPlayback(){}

void MPanelPlayback::initializeLayout(){
    m_frmVolume = new QWidget(this);
    m_frmControls = new QWidget(this);
    m_frmSearchbar = new QWidget(this);

    m_lblTitle = new QLabel(this);
    m_lblArtist = new QLabel(this);
    m_lblPositionMin = new QLabel(this);
    m_lblPositionMax = new QLabel(this);

    m_btnPlay = new QPushButton(this);
    m_btnNext = new QPushButton(this);
    m_btnPrev = new QPushButton(this);
    m_btnFade = new QPushButton(this);
    m_sldPosition = new QSlider(this);

    m_sldVolume = new QSlider(this);
    m_boxSearch = new QLineEdit(this);
    m_btnSearch = new QPushButton(this);

    m_pixNext = QPixmap(":/icons/playback/next.png");
    m_pixPrev = QPixmap(":/icons/playback/prev.png");
    m_pixPlay = QPixmap(":/icons/playback/play.png");
    m_pixPaus = QPixmap(":/icons/playback/paus.png");
    m_pixSearch = QPixmap(":/icons/playback/search.png");

    QGridLayout *layoutVolume = new QGridLayout(this);
    layoutVolume->addWidget(m_sldVolume, 1, 0, 1, 1);
    layoutVolume->addWidget(m_btnPlay, 1, 1, 1, 1);
    layoutVolume->setColumnStretch(0, 1);
    layoutVolume->setRowStretch(0, 1);
    layoutVolume->setRowStretch(2, 1);
    layoutVolume->setRowMinimumHeight(1, 20);
    layoutVolume->setHorizontalSpacing(0);
    layoutVolume->setVerticalSpacing(0);
    layoutVolume->setMargin(0);
    m_frmVolume->setLayout(layoutVolume);

    m_btnPlay->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_btnPlay->setIcon(QIcon(m_pixPlay));
    m_btnPlay->setFlat(true);

    m_sldVolume->setOrientation(Qt::Horizontal);

    int uHeight = 52;
    int uBtnWidth = 48;

    QGridLayout *layoutControl = new QGridLayout(this);
    layoutControl->addWidget(m_lblTitle, 0, 2, 1, 1);
    layoutControl->addWidget(m_lblArtist, 1, 2, 3, 1);
    layoutControl->addWidget(m_lblPositionMin,  3, 1, 1, 1);
    layoutControl->addWidget(m_lblPositionMax, 3, 3, 1, 1);
    layoutControl->addWidget(m_sldPosition, 4, 1, 1, 3);
    layoutControl->addWidget(m_btnPrev, 0, 0, 5, 1);
    layoutControl->addWidget(m_btnFade, 0, 1, 4, 3);
    layoutControl->addWidget(m_btnNext, 0, 4, 5, 1);
    layoutControl->setColumnMinimumWidth(1, 22);
    layoutControl->setColumnMinimumWidth(3, 22);
    layoutControl->setColumnStretch(0, 0);
    layoutControl->setColumnStretch(1, 0);
    layoutControl->setColumnStretch(2, 1);
    layoutControl->setColumnStretch(3, 0);
    layoutControl->setColumnStretch(4, 0);
    layoutControl->setHorizontalSpacing(0);
    layoutControl->setVerticalSpacing(0);
    layoutControl->setMargin(0);
    m_frmControls->setLayout(layoutControl);

    m_frmControls->setFixedHeight(uHeight-1);

    m_btnPrev->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_btnNext->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_btnFade->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    m_btnNext->setFixedWidth(uBtnWidth);
    m_btnPrev->setFixedWidth(uBtnWidth);

    m_sldPosition->setFixedHeight(8);
    m_sldPosition->setOrientation(Qt::Horizontal);

    m_lblTitle->setAlignment(Qt::AlignCenter);
    m_lblArtist->setAlignment(Qt::AlignCenter);
    m_sldPosition->setOrientation(Qt::Horizontal);




    m_btnFade->setVisible(false);



    m_btnNext->setIcon(QIcon(m_pixNext));
    m_btnPrev->setIcon(QIcon(m_pixPrev));
    m_btnFade->setIcon(QIcon(m_pixPlay));

    m_btnNext->setIconSize(QSize((uBtnWidth/12)*8, (uHeight/12)*8));
    m_btnPrev->setIconSize(QSize((uBtnWidth/12)*8, (uHeight/12)*8));
    m_btnFade->setIconSize(QSize((uBtnWidth/12)*8, (uHeight/12)*8));

    m_btnNext->setFlat(true);
    m_btnPrev->setFlat(true);
    m_btnFade->setFlat(true);

    m_btnSearch->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_boxSearch->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_boxSearch->setTextMargins(0, 0, 0, 0);

    m_btnSearch->setIcon(QIcon(m_pixSearch));
    m_btnSearch->setIconSize(QSize(20, 20));
    m_btnSearch->setFlat(true);

    QGridLayout *layoutSearchbar = new QGridLayout(this);
    layoutSearchbar->addWidget(m_btnSearch, 1, 0, 1, 1);
    layoutSearchbar->addWidget(m_boxSearch, 1, 1, 1, 1);
    layoutSearchbar->setColumnStretch(1, 1);
    layoutSearchbar->setRowStretch(0, 1);
    layoutSearchbar->setRowStretch(2, 1);
    layoutSearchbar->setRowMinimumHeight(1, 20);
    layoutSearchbar->setHorizontalSpacing(0);
    layoutSearchbar->setVerticalSpacing(0);
    layoutSearchbar->setMargin(0);
    m_frmSearchbar->setLayout(layoutSearchbar);

    QGridLayout *layoutMain = new QGridLayout(this);
    layoutMain->addWidget(m_frmVolume, 0, 1, 1, 1);
    layoutMain->addWidget(m_frmControls, 0, 3, 1, 1);
    layoutMain->addWidget(m_frmSearchbar, 0, 5, 1, 1);
    layoutMain->setRowMinimumHeight(0, uHeight);
    layoutMain->setColumnMinimumWidth(0, 20);
    layoutMain->setColumnMinimumWidth(1, 150);
    layoutMain->setColumnMinimumWidth(3, 360);
    layoutMain->setColumnMinimumWidth(5, 150);
    layoutMain->setColumnMinimumWidth(6, 20);
    layoutMain->setColumnStretch(2, 1);
    layoutMain->setColumnStretch(3, 6);
    layoutMain->setColumnStretch(4, 1);
    layoutMain->setHorizontalSpacing(0);
    layoutMain->setVerticalSpacing(0);
    layoutMain->setMargin(0);
    setLayout(layoutMain);
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
bool MPanelPlayback::stopped() const {
    return m_currentState == Mpi3::EngineState::EngineStopped;
}
bool MPanelPlayback::playing() const {
    return m_currentState == Mpi3::EngineState::EngineActive;
}
bool MPanelPlayback::paused() const {
    return m_currentState == Mpi3::EngineState::EngineIdle;
}

void MPanelPlayback::setVolume(int volume){
    m_sldVolume->blockSignals(true);
    m_sldVolume->setValue(volume);
    m_sldVolume->blockSignals(false);
}
void MPanelPlayback::setState(Mpi3::EngineState state){
    m_currentState = state;

    if(m_currentState == Mpi3::EngineState::EngineActive){
        m_btnPlay->setIcon(QIcon(m_pixPaus));
        m_btnFade->setIcon(QIcon(m_pixPlay));
    }
    else {
        m_btnPlay->setIcon(QIcon(m_pixPlay));
        m_btnFade->setIcon(QIcon(m_pixPaus));
    }

    m_btnFade->setVisible(true);
    m_fadeTimer->start(300);
}
void MPanelPlayback::setDisplay(MSong *song){
    if(song){
        m_lblTitle->setText(song->name());
        m_lblArtist->setText(song->artist());
        m_lblPositionMax->setText(QString::number(song->time()));
        m_lblPositionMin->setText("0.00");
    }
}

void MPanelPlayback::clickedPlay(){
    emit playing() ? audioPause() : audioPlay();
}
void MPanelPlayback::clickedNext(){
    emit navigateNext();
}
void MPanelPlayback::clickedPrev(){
    emit navigatePrev();
}
void MPanelPlayback::volumeChanged(){
    emit changeVolume(volume());
}

void MPanelPlayback::elementModified(MMediaElement *elemModified){
    if(elemModified->type() == Mpi3::SongElement){
        MSong *sc_songModified = static_cast<MSong*>(elemModified);
        if(sc_songModified->pid() == m_pidCurrentSong){
            setDisplay(sc_songModified);
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





