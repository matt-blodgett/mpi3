#include "maudiocontrol.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>

#include <QPropertyAnimation>
#include <QPushButton>
#include <QLineEdit>
#include <QSlider>

#include <QEvent>
#include <QTimer>


#include <QDebug>


Mpi3PanelPlayback::Mpi3PanelPlayback(QWidget *parent) : QWidget(parent){
    initializeLayout();


    m_fadeTimer = new QTimer(this);
    m_fadeTimer->setSingleShot(true);

    m_btnPlay->installEventFilter(this);
    m_frmControls->installEventFilter(this);

    connect(m_fadeTimer, &QTimer::timeout, this, [this](){beginFadeButton();});

    connect(m_btnPlay, &QPushButton::released, this, &Mpi3PanelPlayback::clickedPlay);
    connect(m_btnNext, &QPushButton::released, this, &Mpi3PanelPlayback::clickedNext);
    connect(m_btnPrev, &QPushButton::released, this, &Mpi3PanelPlayback::clickedPrev);
    connect(m_sldVolume, &QSlider::valueChanged, this, &Mpi3PanelPlayback::volumeSliderChanged);

    m_btnNext->setObjectName("ButtonNext");
    m_btnPrev->setObjectName("ButtonPrev");
    m_btnPlay->setObjectName("ButtonPlay");
    m_sldPosition->setObjectName("SliderPosition");
    m_frmControls->setObjectName("PlaybackControl");
    m_sldVolume->setObjectName("SliderVolume");
    setObjectName("PanelPlayback");
}
Mpi3PanelPlayback::~Mpi3PanelPlayback(){}

void Mpi3PanelPlayback::initializeLayout(){
    m_frmVolume = new QWidget(this);
    m_frmControls = new QWidget(this);
    m_frmSearchbar = new QWidget(this);

    m_sldVolume = new QSlider(m_frmVolume);

    m_btnNext = new QPushButton(m_frmControls);
    m_btnPrev = new QPushButton(m_frmControls);
    m_btnPlay = new QPushButton(m_frmControls);
    m_sldPosition = new QSlider(m_frmControls);

    m_boxSearch = new QLineEdit(m_frmSearchbar);
    m_btnSearch = new QPushButton(m_frmSearchbar);

    QGridLayout *layoutVolume = new QGridLayout(this);
    layoutVolume->addWidget(m_sldVolume, 0, 0, 1, 1);
    layoutVolume->setColumnStretch(1, 1);
    m_frmVolume->setLayout(layoutVolume);

    m_sldVolume->setOrientation(Qt::Horizontal);
    m_sldVolume->setFixedWidth(120);

    int h = 60;
    int w = 350;
    int w_btn = 50;

    m_frmControls->setFixedHeight(h);
    m_frmControls->setFixedWidth(w);

    m_sldPosition->setOrientation(Qt::Horizontal);
    m_sldPosition->setFixedWidth(w-(w_btn*2));
    m_sldPosition->setFixedHeight(12);
    m_sldPosition->move(w_btn, 45);

    m_btnNext->setFixedHeight(h-1);
    m_btnPrev->setFixedHeight(h-1);
    m_btnPlay->setFixedHeight(h-20);

    m_btnNext->setFixedWidth(w_btn);
    m_btnPrev->setFixedWidth(w_btn);
    m_btnPlay->setFixedWidth(w-(w_btn*2));

    m_btnPrev->move(0, 0);
    m_btnNext->move(w-w_btn, 0);
    m_btnPlay->move(w_btn, 0);

    m_btnNext->setFlat(true);
    m_btnPrev->setFlat(true);
    m_btnPlay->setFlat(true);

    m_pixNext = QPixmap(":/icons/playback/next.png");
    m_pixPrev = QPixmap(":/icons/playback/prev.png");
    m_pixPlay = QPixmap(":/icons/playback/play.png");
    m_pixPaus = QPixmap(":/icons/playback/paus.png");

    m_btnNext->setIcon(QIcon(m_pixNext));
    m_btnPrev->setIcon(QIcon(m_pixPrev));
    m_btnPlay->setIcon(QIcon(m_pixPlay));

    m_btnNext->setIconSize(m_pixNext.rect().size());
    m_btnPrev->setIconSize(m_pixPrev.rect().size());
    m_btnPlay->setIconSize(m_pixPlay.rect().size());


    QGridLayout *layoutSearchbar = new QGridLayout(this);
    layoutSearchbar->addWidget(m_boxSearch, 0, 1, 1, 1);
    layoutSearchbar->addWidget(m_btnSearch, 0, 2, 1, 1);
    layoutSearchbar->setColumnStretch(0, 1);
    layoutSearchbar->setVerticalSpacing(0);
    layoutSearchbar->setHorizontalSpacing(0);
    layoutSearchbar->setMargin(0);
    m_frmSearchbar->setLayout(layoutSearchbar);

    m_boxSearch->setFixedWidth(100);
    m_btnSearch->setText("S");
    m_btnSearch->setFixedWidth(20);

    QGridLayout *layoutMain = new QGridLayout(this);
    layoutMain->addWidget(m_frmVolume, 0, 0, 1, 1);
    layoutMain->addWidget(m_frmControls, 0, 1, 1, 1);
    layoutMain->addWidget(m_frmSearchbar, 0, 2, 1, 1);
    layoutMain->setColumnStretch(1, 1);
    layoutMain->setColumnMinimumWidth(0, 150);
    layoutMain->setColumnMinimumWidth(1, 350);
    layoutMain->setColumnMinimumWidth(2, 150);
    layoutMain->setRowMinimumHeight(0, 60);
    layoutMain->setMargin(0);
    setLayout(layoutMain);

    int x = m_frmVolume->width() + m_frmControls->width();
    m_frmControls->move(x, 0);
}

double Mpi3PanelPlayback::getButtonOpacity() const{
    return m_btnOpacity;
}
void Mpi3PanelPlayback::setButtonOpacity(double opacity){
    m_btnOpacity = opacity;

    QPixmap pixCurrent = m_playerState == QMediaPlayer::PlayingState ? m_pixPaus : m_pixPlay;

    if(m_btnOpacity > 0.00){
        QPixmap pixMask = QPixmap(pixCurrent);

        QImage imgMask(pixMask.size(), QImage::Format_ARGB32_Premultiplied);
        imgMask.fill(Qt::transparent);

        QPainter painter(&imgMask);
        painter.setOpacity(m_btnOpacity / 100.00);
        painter.drawPixmap(0, 0, pixMask);
        painter.end();

        QIcon icnMask(QPixmap::fromImage(imgMask));
        m_btnPlay->setIcon(icnMask);
    }
    else {
        m_btnPlay->setVisible(false);
        m_btnPlay->setIcon(QIcon(pixCurrent));
    }
}
void Mpi3PanelPlayback::beginFadeButton(){
    if(m_playerState != QMediaPlayer::StoppedState){
        QPropertyAnimation *animation = new QPropertyAnimation(this);
        animation->setPropertyName("buttonOpacity");
        animation->setTargetObject(this);
        animation->setDuration(1000);
        animation->setStartValue(100.00);
        animation->setEndValue(0.00);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

int Mpi3PanelPlayback::volume() const{
    double linearVolume =  QAudio::convertVolume(
                m_sldVolume->value() / double(100),
                QAudio::LogarithmicVolumeScale,
                QAudio::LinearVolumeScale);

    return qRound(linearVolume * 100);
}
void Mpi3PanelPlayback::setVolume(int volume){
    double logarithmicVolume = QAudio::convertVolume(
                volume / double(100),
                QAudio::LinearVolumeScale,
                QAudio::LogarithmicVolumeScale);

    m_sldVolume->setValue(qRound(logarithmicVolume * 100));
}

QMediaPlayer::State Mpi3PanelPlayback::state() const{
    return m_playerState;
}
void Mpi3PanelPlayback::setState(QMediaPlayer::State state) {
    if(state != m_playerState) {
        m_playerState = state;

        switch(m_playerState) {
            case QMediaPlayer::StoppedState: {
                m_btnPlay->setIcon(QIcon(m_pixPlay));
                m_btnPlay->setVisible(true);
                break;
            }
            case QMediaPlayer::PausedState: {
                m_btnPlay->setIcon(QIcon(m_pixPlay));
                break;
            }
            case QMediaPlayer::PlayingState: {
                m_btnPlay->setIcon(QIcon(m_pixPaus));
                break;
            }
        }
    }
}

void Mpi3PanelPlayback::clickedPlay(){
    switch(m_playerState) {
        case QMediaPlayer::StoppedState: {
//            setState(QMediaPlayer::PlayingState);
            emit audioPlay();
            break;
        }
        case QMediaPlayer::PausedState: {
//            setState(QMediaPlayer::PlayingState);
            emit audioPlay();
            break;
        }
        case QMediaPlayer::PlayingState: {
//            setState(QMediaPlayer::PausedState);
            emit audioPause();
            break;
        }
    }
}

void Mpi3PanelPlayback::clickedNext(){
    emit navigateNext();
}

void Mpi3PanelPlayback::clickedPrev(){
    emit navigatePrev();
}

void Mpi3PanelPlayback::volumeSliderChanged(){
    emit changeVolume(volume());
}

void Mpi3PanelPlayback::paintEvent(QPaintEvent *event){
    QStyleOption opt;
    opt.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QWidget::paintEvent(event);
}
void Mpi3PanelPlayback::resizeEvent(QResizeEvent *event){
    int x = (width() / 2) - (m_frmControls->width() / 2);
    m_frmControls->move(x, 0);
    QWidget::resizeEvent(event);
}
bool Mpi3PanelPlayback::eventFilter(QObject *object, QEvent *event){
    if(object == m_frmControls){
        if(event->type() == QEvent::Enter){
            m_fadeTimer->stop();
            m_btnPlay->setVisible(true);
        }
    }
    else if(object == m_btnPlay){
        if(event->type() == QEvent::Enter){
            m_fadeTimer->stop();
        }
        else if(event->type() == QEvent::Leave){
            m_fadeTimer->stop();
            m_fadeTimer->start(1000);
        }
    }

    return QWidget::eventFilter(object, event);
}












