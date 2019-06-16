#include "mframeplayback.h"
#include "mmedialibrary.h"
#include "mformat.h"
#include "mstyle.h"

#include <QGridLayout>
#include <QPainter>

#include <QPropertyAnimation>
#include <QPushButton>
#include <QLineEdit>
#include <QSlider>
#include <QLabel>

#include <QEvent>
#include <QTimer>


#include <QDebug>


MFramePlayback::MFramePlayback(QWidget *parent) : MFrame(parent)
{

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
    MStyle::setStyle(m_sldVolume, MStyle::SLD_Volume);

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

    MStyle::setStyle(m_lblTitle, MStyle::LBL_SongTitle);
    MStyle::setStyle(m_lblArtist, MStyle::LBL_SongArtist);
    MStyle::setStyle(m_lblPositionMin, MStyle::LBL_SongPosition);
    MStyle::setStyle(m_lblPositionMax, MStyle::LBL_SongPosition);

    MStyle::setStyle(m_btnPlay, MStyle::PB_PlaySong);
    MStyle::setStyle(m_btnNext, MStyle::PB_NextSong);
    MStyle::setStyle(m_btnPrev, MStyle::PB_PreviousSong);
    MStyle::setStyle(m_btnFade, MStyle::PB_Fade);
    MStyle::setStyle(m_sldPosition, MStyle::SLD_Postion);

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


    m_sldPosition->setOrientation(Qt::Horizontal);

    m_lblTitle->setAlignment(Qt::AlignCenter);
    m_lblArtist->setAlignment(Qt::AlignCenter);

    m_btnNext->setFixedWidth(uBtnWidth);
    m_btnPrev->setFixedWidth(uBtnWidth);

    m_btnNext->setIconSize(QSize((uBtnWidth/12)*8, (uHeight/12)*8));
    m_btnPrev->setIconSize(QSize((uBtnWidth/12)*8, (uHeight/12)*8));
    m_btnFade->setIconSize(QSize((uBtnWidth/12)*8, (uHeight/12)*8));

    m_btnPlay->setFixedWidth(32);
    m_btnPlay->setFixedHeight(32);
    m_btnPlay->setIconSize(QSize(22, 22));


    // -------------------------------------------------- SEARCHBAR

    QWidget *frmSearchbar = new QWidget(this);

    m_boxSearch = new QLineEdit(this);
    m_btnSearch = new QPushButton(this);

    MStyle::setStyle(m_boxSearch, MStyle::LE_Search);
    MStyle::setStyle(m_btnSearch, MStyle::PB_Search);

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

    m_btnSearch->setIconSize(QSize(20, 20));

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



    frmPlayback->setObjectName("PlaybackControl");


    m_currentState = Mpi3::EngineStopped;

    m_sldVolume->setRange(0, 100);

    m_fadeTimer = new QTimer(this);
    m_fadeTimer->setSingleShot(true);
    connect(m_fadeTimer, &QTimer::timeout, this, [this](){animateFadeButton();});

    connect(m_btnPlay, &QPushButton::released, this, &MFramePlayback::playClicked);
    connect(m_btnNext, &QPushButton::released, this, &MFramePlayback::nextClicked);
    connect(m_btnPrev, &QPushButton::released, this, &MFramePlayback::prevClicked);

    connect(m_sldPosition, &QSlider::sliderPressed, this, &MFramePlayback::seekBegin);
    connect(m_sldPosition, &QSlider::sliderReleased, this, &MFramePlayback::seekEnd);
    connect(m_sldPosition, &QSlider::valueChanged, this, &MFramePlayback::positionChanged);

    connect(m_sldVolume, &QSlider::valueChanged, this, &MFramePlayback::volumeChanged);
}

double MFramePlayback::buttonOpacity() const
{
    return m_btnOpacity;
}
void MFramePlayback::setButtonOpacity(double opacity)
{
    m_btnOpacity = opacity;

    bool playing = m_currentState == Mpi3::EngineActive;
    QPixmap pixCurrent = playing ? QPixmap(m_icnPlay) : QPixmap(m_icnPause);

    if(m_btnOpacity > 0.00) {
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
void MFramePlayback::animateFadeButton()
{
    if(m_currentState != Mpi3::EngineStopped) {
        QPropertyAnimation *animation = new QPropertyAnimation(this);
        animation->setPropertyName("buttonOpacity");
        animation->setTargetObject(this);
        animation->setStartValue(100.00);
        animation->setEndValue(0.00);
        animation->setDuration(700);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

int MFramePlayback::volume() const
{
    return m_sldVolume->value();
}
double MFramePlayback::position() const
{
    return m_sldPosition->value();
}

void MFramePlayback::setVolume(int volume)
{
    m_sldVolume->blockSignals(true);
    m_sldVolume->setValue(volume);
    m_sldVolume->blockSignals(false);
}
void MFramePlayback::setPosition(double position)
{
    m_sldPosition->blockSignals(true);
    m_sldPosition->setValue(static_cast<int>(position));
    m_lblPositionMin->setText(Mpi3::Util::timeToString(position));
//    m_lblPositionMin->setText(MMediaLibrary::timeToString(position));
    m_sldPosition->blockSignals(false);
}
void MFramePlayback::setState(Mpi3::EngineState state)
{
    if(m_seeking) {
        m_seeking = state == Mpi3::EngineIdle;
    }
    else {
        if(state == Mpi3::EngineActive) {
            m_btnPlay->setIcon(QIcon(QPixmap(m_icnPause)));
            m_btnFade->setIcon(QIcon(QPixmap(m_icnPlay)));
        }
        else if(state == Mpi3::EngineIdle) {
            m_btnPlay->setIcon(QIcon(QPixmap(m_icnPlay)));
            m_btnFade->setIcon(QIcon(QPixmap(m_icnPause)));
        }

        if(!m_navigating && state != m_currentState) {
            m_btnFade->setVisible(true);
            m_fadeTimer->start(300);
        }

        if(state != Mpi3::EngineStopped) {
            m_navigating = false;
        }
    }

    m_currentState = state;
}
void MFramePlayback::setDisplay(MSong *song)
{
    m_sldPosition->setMinimum(0);
    m_sldPosition->setMaximum(static_cast<int>(song->time()));
    m_lblTitle->setText(song->name());
    m_lblArtist->setText(song->artist());
    m_lblPositionMax->setText(Mpi3::Util::timeToString(song->time()));
    m_lblPositionMin->setText("0:00");
    m_pidCurrentSong = song->pid();
}

void MFramePlayback::playClicked()
{
    bool paused = m_currentState == Mpi3::EngineIdle;
    bool stopped = m_currentState == Mpi3::EngineStopped;
    emit paused || stopped ? playRequested() : pausRequested();
}
void MFramePlayback::nextClicked()
{
    m_navigating = true;
    emit nextRequested();
}
void MFramePlayback::prevClicked()
{
    m_navigating = true;
    emit prevRequested();
}
void MFramePlayback::volumeChanged()
{
    emit volumeRequested(volume());
}

void MFramePlayback::seekBegin()
{
    m_seeking = true;
    emit pausRequested();
}
void MFramePlayback::seekEnd()
{
    emit seekRequested(m_sldPosition->value());
}
void MFramePlayback::positionChanged(int position)
{
    setPosition(static_cast<double>(position));
}

void MFramePlayback::elementModified(MMediaElement *elemModified)
{
    if(elemModified->pid() == m_pidCurrentSong) {
        setDisplay(static_cast<MSong*>(elemModified));
    }
}
