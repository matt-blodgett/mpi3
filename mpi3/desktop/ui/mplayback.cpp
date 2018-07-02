#include "mplayback.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>
#include <QSlider>
#include <QLineEdit>

#include <QDebug>


PanelPlayback::PanelPlayback(QWidget *parent) : QWidget(parent)
{
    frm_controls = new QWidget(this);

    btn_next = new QPushButton(frm_controls);
    btn_prev = new QPushButton(frm_controls);
    btn_play = new QPushButton(frm_controls);
    sld_position = new QSlider(frm_controls);

    QPixmap pixmapNext(":/desktop/icons/next.png");
    QPixmap pixmapPrev(":/desktop/icons/prev.png");
    QPixmap pixmapPlay(":/desktop/icons/play.png");
    QPixmap pixmapPaus(":/desktop/icons/paus.png");

    icn_next.addPixmap(pixmapNext);
    icn_prev.addPixmap(pixmapPrev);
    icn_play.addPixmap(pixmapPlay);
    icn_paus.addPixmap(pixmapPaus);

    btn_next->setIcon(icn_next);
    btn_prev->setIcon(icn_prev);
    btn_play->setIcon(icn_play);

    btn_next->setIconSize(pixmapNext.rect().size());
    btn_prev->setIconSize(pixmapPrev.rect().size());
    btn_play->setIconSize(pixmapPlay.rect().size());

    sld_position->setOrientation(Qt::Horizontal);

    btn_next->setFlat(true);
    btn_prev->setFlat(true);
    btn_play->setFlat(true);

    int h = 60;
    int w = 300;
    int w_btn = 50;

    frm_controls->setFixedHeight(h);
    frm_controls->setFixedWidth(w);

    btn_next->setFixedHeight(h-1);
    btn_prev->setFixedHeight(h-1);
    btn_play->setFixedHeight(h-20);

    btn_next->setFixedWidth(w_btn);
    btn_prev->setFixedWidth(w_btn);
    btn_play->setFixedWidth(w-(w_btn*2));

    btn_prev->move(0, 0);
    btn_next->move(w-w_btn, 0);
    btn_play->move(w_btn, 0);

    sld_position->setFixedWidth(w-(w_btn*2));
    sld_position->setFixedHeight(12);
    sld_position->move(w_btn, 45);


    sld_volume = new QSlider(this);
    sld_volume->setOrientation(Qt::Horizontal);


    box_search = new QLineEdit(this);
    btn_search = new QPushButton(this);

    box_search->setFixedWidth(120);
    btn_search->setText("S");
    btn_search->setFixedWidth(20);

    QGridLayout *layoutSearchbar = new QGridLayout;

    layoutSearchbar->addWidget(box_search, 0, 0, 1, 1);
    layoutSearchbar->addWidget(btn_search, 0, 1, 1, 1);

    QGridLayout *layoutMain = new QGridLayout;

    layoutMain->addWidget(sld_volume, 0, 0, 1, 1);
    layoutMain->addWidget(frm_controls, 0, 1, 1, 1);
    layoutMain->addLayout(layoutSearchbar, 0, 2, 1, 1);

    layoutMain->setColumnStretch(1, 1);
    layoutMain->setColumnMinimumWidth(0, 150);
    layoutMain->setColumnMinimumWidth(1, 400);
    layoutMain->setColumnMinimumWidth(2, 150);
    layoutMain->setRowMinimumHeight(0, 60);
    layoutMain->setMargin(0);

    this->setLayout(layoutMain);

    connect(btn_play, &QAbstractButton::clicked, this, &PanelPlayback::clickPlay);
//    connect(m_stopButton, &QAbstractButton::clicked, this, &PlayerControls::stop);
    connect(btn_next, &QAbstractButton::clicked, this, &PanelPlayback::next);
    connect(btn_prev, &QAbstractButton::clicked, this, &PanelPlayback::previous);
//    connect(m_muteButton, &QAbstractButton::clicked, this, &PlayerControls::muteClicked);
    connect(sld_volume, &QSlider::valueChanged, this, &PanelPlayback::onVolumeSliderValueChanged);

    btn_next->setObjectName("ButtonNext");
    btn_prev->setObjectName("ButtonPrev");
    btn_play->setObjectName("ButtonPlay");
    sld_position->setObjectName("SliderPosition");
    frm_controls->setObjectName("PlaybackControl");
    sld_volume->setObjectName("SliderVolume");
    this->setObjectName("PanelPlayback");
}

PanelPlayback::~PanelPlayback()
{

}

void PanelPlayback::clickPlay()
{
    this->btn_play->setIcon(this->icn_paus);

    switch (m_playerState) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        emit play();
        break;
    case QMediaPlayer::PlayingState:
        emit pause();
        break;
    }
}

void PanelPlayback::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    int x = (this->frm_controls->width() / 2) - (300 / 2);
    this->frm_controls->move(x, 0);
}

void PanelPlayback::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}






QMediaPlayer::State PanelPlayback::state() const
{
    return m_playerState;
}

void PanelPlayback::setState(QMediaPlayer::State state)
{
    if (state != m_playerState) {
        m_playerState = state;

        switch (state) {
        case QMediaPlayer::StoppedState:
//            btn_stop->setEnabled(false);
//            btn_play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        case QMediaPlayer::PlayingState:
//            btn_stop->setEnabled(true);
//            btn_play->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        case QMediaPlayer::PausedState:
//            btn_stop->setEnabled(true);
//            btn_play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        }
    }
}

int PanelPlayback::volume() const
{
    qreal linearVolume =  QAudio::convertVolume(sld_volume->value() / qreal(100),
                                                QAudio::LogarithmicVolumeScale,
                                                QAudio::LinearVolumeScale);

    return qRound(linearVolume * 100);
}

void PanelPlayback::setVolume(int volume)
{
    qreal logarithmicVolume = QAudio::convertVolume(volume / qreal(100),
                                                    QAudio::LinearVolumeScale,
                                                    QAudio::LogarithmicVolumeScale);

    sld_volume->setValue(qRound(logarithmicVolume * 100));
}


void PanelPlayback::onVolumeSliderValueChanged()
{
    emit changeVolume(volume());
}


bool PanelPlayback::isMuted() const
{
    return m_playerMuted;
}

void PanelPlayback::setMuted(bool muted)
{
    if (muted != m_playerMuted) {
        m_playerMuted = muted;
    }
}

void PanelPlayback::clickMute()
{
    emit changeMuting(!m_playerMuted);
}








