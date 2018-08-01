#include "maudiocontrol.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>
#include <QSlider>
#include <QLineEdit>

#include <QDebug>


Mpi3PanelPlayback::Mpi3PanelPlayback(QWidget *parent) : QWidget(parent){
    m_frmVolume = new QWidget(this);
    m_frmControls = new QWidget(this);
    m_frmSearchbar = new QWidget(this);

    m_btnNext = new QPushButton(m_frmControls);
    m_btnPrev = new QPushButton(m_frmControls);
    m_btnPlay = new QPushButton(m_frmControls);
    m_sldPosition = new QSlider(m_frmControls);

    QPixmap pixmapNext(":/icons/playback/next.png");
    QPixmap pixmapPrev(":/icons/playback/prev.png");
    QPixmap pixmapPlay(":/icons/playback/play.png");
    QPixmap pixmapPaus(":/icons/playback/paus.png");

    m_icnNext.addPixmap(pixmapNext);
    m_icnPrev.addPixmap(pixmapPrev);
    m_icnPlay.addPixmap(pixmapPlay);
    m_icnPaus.addPixmap(pixmapPaus);

    m_btnNext->setIcon(m_icnNext);
    m_btnPrev->setIcon(m_icnPrev);
    m_btnPlay->setIcon(m_icnPlay);

    m_btnNext->setIconSize(pixmapNext.rect().size());
    m_btnPrev->setIconSize(pixmapPrev.rect().size());
    m_btnPlay->setIconSize(pixmapPlay.rect().size());

    m_sldPosition->setOrientation(Qt::Horizontal);

    m_btnNext->setFlat(true);
    m_btnPrev->setFlat(true);
    m_btnPlay->setFlat(true);

    int h = 60;
    int w = 350;
    int w_btn = 50;

    m_frmControls->setFixedHeight(h);
    m_frmControls->setFixedWidth(w);

    m_btnNext->setFixedHeight(h-1);
    m_btnPrev->setFixedHeight(h-1);
    m_btnPlay->setFixedHeight(h-20);

    m_btnNext->setFixedWidth(w_btn);
    m_btnPrev->setFixedWidth(w_btn);
    m_btnPlay->setFixedWidth(w-(w_btn*2));

    m_btnPrev->move(0, 0);
    m_btnNext->move(w-w_btn, 0);
    m_btnPlay->move(w_btn, 0);

    m_sldPosition->setFixedWidth(w-(w_btn*2));
    m_sldPosition->setFixedHeight(12);
    m_sldPosition->move(w_btn, 45);





    m_sldVolume = new QSlider(m_frmVolume);
    m_sldVolume->setOrientation(Qt::Horizontal);
    m_sldVolume->setFixedWidth(120);

    QGridLayout *layoutVolume = new QGridLayout(this);

    layoutVolume->addWidget(m_sldVolume, 0, 0, 1, 1);
    layoutVolume->setColumnStretch(1, 1);

    m_frmVolume->setLayout(layoutVolume);




    m_boxSearch = new QLineEdit(m_frmSearchbar);
    m_btnSearch = new QPushButton(m_frmSearchbar);

    m_boxSearch->setFixedWidth(100);
    m_btnSearch->setText("S");
    m_btnSearch->setFixedWidth(20);

    QGridLayout *layoutSearchbar = new QGridLayout(this);

    layoutSearchbar->addWidget(m_boxSearch, 0, 1, 1, 1);
    layoutSearchbar->addWidget(m_btnSearch, 0, 2, 1, 1);
    layoutSearchbar->setColumnStretch(0, 1);
    layoutSearchbar->setMargin(0);
    layoutSearchbar->setVerticalSpacing(0);
    layoutSearchbar->setHorizontalSpacing(0);

    m_frmSearchbar->setLayout(layoutSearchbar);




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

    this->setLayout(layoutMain);



    int x = m_frmVolume->width() + m_frmControls->width();
    m_frmControls->move(x, 0);



    connect(m_btnPlay, &QAbstractButton::clicked, this, &Mpi3PanelPlayback::clickPlay);
//    connect(m_stopButton, &QAbstractButton::clicked, this, &PlayerControls::stop);
    connect(m_btnNext, &QAbstractButton::clicked, this, &Mpi3PanelPlayback::next);
    connect(m_btnPrev, &QAbstractButton::clicked, this, &Mpi3PanelPlayback::previous);
//    connect(m_muteButton, &QAbstractButton::clicked, this, &PlayerControls::muteClicked);
    connect(m_sldVolume, &QSlider::valueChanged, this, &Mpi3PanelPlayback::onVolumeSliderValueChanged);

    m_btnNext->setObjectName("ButtonNext");
    m_btnPrev->setObjectName("ButtonPrev");
    m_btnPlay->setObjectName("ButtonPlay");
    m_sldPosition->setObjectName("SliderPosition");
    m_frmControls->setObjectName("PlaybackControl");
    m_sldVolume->setObjectName("SliderVolume");
    this->setObjectName("PanelPlayback");
}
Mpi3PanelPlayback::~Mpi3PanelPlayback(){}

void Mpi3PanelPlayback::clickPlay(){
    this->m_btnPlay->setIcon(this->m_icnPaus);

    switch (m_playerState) {
        case QMediaPlayer::StoppedState: {
            break;
        }
        case QMediaPlayer::PausedState: {
            emit play();
            break;
        }
        case QMediaPlayer::PlayingState: {
            emit pause();
            break;
        }
    }
}
void Mpi3PanelPlayback::clickMute(){
    emit changeMuting(!m_playerMuted);
}
void Mpi3PanelPlayback::onVolumeSliderValueChanged(){
    emit changeVolume(volume());
}

QMediaPlayer::State Mpi3PanelPlayback::state() const{
    return m_playerState;
}
int Mpi3PanelPlayback::volume() const{
    qreal linearVolume =  QAudio::convertVolume(
                m_sldVolume->value() / qreal(100),
                QAudio::LogarithmicVolumeScale,
                QAudio::LinearVolumeScale);

    return qRound(linearVolume * 100);
}
bool Mpi3PanelPlayback::isMuted() const{
    return m_playerMuted;
}

void Mpi3PanelPlayback::setState(QMediaPlayer::State state)
{
    if (state != m_playerState) {
        m_playerState = state;

        switch (state) {
            case QMediaPlayer::StoppedState: {
    //            btn_stop->setEnabled(false);
    //            btn_play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
                break;
            }
            case QMediaPlayer::PlayingState: {
    //            btn_stop->setEnabled(true);
    //            btn_play->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
                break;
            }
            case QMediaPlayer::PausedState: {
    //            btn_stop->setEnabled(true);
    //            btn_play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
                break;
            }
        }
    }
}
void Mpi3PanelPlayback::setVolume(int volume){
    qreal logarithmicVolume = QAudio::convertVolume(
                volume / qreal(100),
                QAudio::LinearVolumeScale,
                QAudio::LogarithmicVolumeScale);

    m_sldVolume->setValue(qRound(logarithmicVolume * 100));
}
void Mpi3PanelPlayback::setMuted(bool muted){
    if (muted != m_playerMuted) {
        m_playerMuted = muted;
    }
}

void Mpi3PanelPlayback::resizeEvent(QResizeEvent *event){
    int x = (this->width() / 2) - (m_frmControls->width() / 2);
    m_frmControls->move(x, 0);
    QWidget::resizeEvent(event);
}
void Mpi3PanelPlayback::paintEvent(QPaintEvent *event){
    QStyleOption opt;
    opt.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QWidget::paintEvent(event);
}
