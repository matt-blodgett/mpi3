#include "maudiocontrol.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>

#include <QPushButton>
#include <QSlider>
#include <QLineEdit>

#include <QEvent>
#include <QTimer>


#include <QDebug>


Mpi3PanelPlayback::Mpi3PanelPlayback(QWidget *parent) : QWidget(parent){
    initializeLayout();



//    QPixmap pixmapPlay(":/icons/playback/play.png");
//    m_icnPlay.addPixmap(pixmapPlay);

    QImage img(":/icons/playback/play.png", "png");


    int w = img.width();
    int h = img.height();

//    QImage alphaMask = QImage()


    QColor colour(255, 255, 255, 255);

    QImage alpha(w, h, QImage::Format_ARGB32);

    img.fill(colour);


//    QImage::Format_ARGB32
//http://doc.qt.io/qt-5/qimage.html#Format-enum
//http://doc.qt.io/qt-5/qpixmap.html

    QPixmap pxm = QPixmap::fromImage(img);

    pxm.fill(colour);


    m_icnPlay.addPixmap(pxm);

    m_btnPlay->setIcon(m_icnPlay);
    m_btnPlay->setIconSize(pxm.rect().size());








    m_btnPlayTimer = new QTimer(this);
    m_btnPlay->installEventFilter(this);


    m_frmControls->installEventFilter(this);

    connect(m_btnPlayTimer, &QTimer::timeout, this, [this](){m_btnPlay->setVisible(false);});




    connect(m_btnPlay, &QPushButton::clicked, this, &Mpi3PanelPlayback::clickPlay);
//    connect(m_btnNext, &QPushButton::clicked, this, &Mpi3PanelPlayback::next);
//    connect(m_btnPrev, &QPushButton::clicked, this, &Mpi3PanelPlayback::prev);
    connect(m_sldVolume, &QSlider::valueChanged, this, &Mpi3PanelPlayback::onVolumeSliderValueChanged);

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

    QPixmap pixmapNext(":/icons/playback/next.png");
    QPixmap pixmapPrev(":/icons/playback/prev.png");
//    QPixmap pixmapPlay(":/icons/playback/play.png");
    QPixmap pixmapPaus(":/icons/playback/paus.png");

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

    m_sldPosition->setOrientation(Qt::Horizontal);
    m_sldPosition->setFixedWidth(w-(w_btn*2));
    m_sldPosition->setFixedHeight(12);
    m_sldPosition->move(w_btn, 45);

    m_icnNext.addPixmap(pixmapNext);
    m_icnPrev.addPixmap(pixmapPrev);
//    m_icnPlay.addPixmap(pixmapPlay);
    m_icnPaus.addPixmap(pixmapPaus);

    m_btnNext->setIcon(m_icnNext);
    m_btnPrev->setIcon(m_icnPrev);
//    m_btnPlay->setIcon(m_icnPlay);

    m_btnNext->setIconSize(pixmapNext.rect().size());
    m_btnPrev->setIconSize(pixmapPrev.rect().size());
//    m_btnPlay->setIconSize(pixmapPlay.rect().size());

    m_btnNext->setFlat(true);
    m_btnPrev->setFlat(true);
    m_btnPlay->setFlat(true);

    QGridLayout *layoutSearchbar = new QGridLayout(this);
    layoutSearchbar->addWidget(m_boxSearch, 0, 1, 1, 1);
    layoutSearchbar->addWidget(m_btnSearch, 0, 2, 1, 1);
    layoutSearchbar->setColumnStretch(0, 1);
    layoutSearchbar->setMargin(0);
    layoutSearchbar->setVerticalSpacing(0);
    layoutSearchbar->setHorizontalSpacing(0);
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
    if (state != m_playerState) {
        m_playerState = state;

        qDebug() << m_playerState;

        switch(m_playerState) {
            case QMediaPlayer::StoppedState: {
                m_btnPlay->setIcon(m_icnPlay);
                break;
            }
            case QMediaPlayer::PausedState: {
                m_btnPlay->setIcon(m_icnPlay);
                break;
            }
            case QMediaPlayer::PlayingState: {
                m_btnPlay->setIcon(m_icnPaus);
                break;
            }
        }
    }
}

void Mpi3PanelPlayback::clickPlay(){
    switch(m_playerState) {
        case QMediaPlayer::StoppedState: {
            setState(QMediaPlayer::PlayingState);
//            emit stop();
            break;
        }
        case QMediaPlayer::PausedState: {
            setState(QMediaPlayer::PlayingState);
//            emit play();
            break;
        }
        case QMediaPlayer::PlayingState: {
            setState(QMediaPlayer::PausedState);
//            emit pause();
            break;
        }
    }
}
void Mpi3PanelPlayback::onVolumeSliderValueChanged(){
    emit changeVolume(volume());
}

void Mpi3PanelPlayback::resizeEvent(QResizeEvent *event){
    int x = (width() / 2) - (m_frmControls->width() / 2);
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

bool Mpi3PanelPlayback::eventFilter(QObject *object, QEvent *event){
    if(object == m_frmControls){
        if(event->type() == QEvent::Enter){
            m_btnPlayTimer->stop();
            m_btnPlay->setVisible(true);
        }
    }
    else if(object == m_btnPlay){
        if(event->type() == QEvent::Enter){
            m_btnPlayTimer->stop();
        }
        else if(event->type() == QEvent::Leave){
            m_btnPlayTimer->start(1000);
        }
    }

    return QWidget::eventFilter(object, event);
}

























