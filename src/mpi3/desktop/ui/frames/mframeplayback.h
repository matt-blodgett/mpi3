#pragma once

#ifndef MFRAMEPLAYBACK_H
#define MFRAMEPLAYBACK_H


#include "mpi3/desktop/ui/frames/mframe.h"


#include <QMediaPlayer>


QT_BEGIN_NAMESPACE
class QPushButton;
class QLineEdit;
class QSlider;
class QLabel;
QT_END_NAMESPACE


#include "mpi3/core/mglobal.h"


class MFramePlayback : public MFrame
{
    Q_OBJECT
    Q_PROPERTY(double buttonOpacity READ buttonOpacity WRITE setButtonOpacity)
    Q_PROPERTY(QString iconPlay MEMBER m_icnPlay)
    Q_PROPERTY(QString iconPause MEMBER m_icnPause)

public:
    explicit MFramePlayback(QWidget *parent = nullptr);

private:
    double m_btnOpacity = 100.00;
    double buttonOpacity() const;
    void setButtonOpacity(double opacity);
    void animateFadeButton();

    QString m_icnPlay;
    QString m_icnPause;

public:
    int volume() const;
    qint64 position() const;

    void setSong(MSong *song);

private:
    QMediaPlayer::State m_currentState = QMediaPlayer::StoppedState;
    bool m_navigating = false;
    bool m_seeking = false;
    QString m_pidCurrentSong;

private:
    QLabel *m_lblTitle = nullptr;
    QLabel *m_lblArtist = nullptr;
    QLabel *m_lblPositionMin = nullptr;
    QLabel *m_lblPositionMax = nullptr;

    QTimer *m_fadeTimer = nullptr;
    QPushButton *m_btnNext = nullptr;
    QPushButton *m_btnPrev = nullptr;
    QPushButton *m_btnPlay = nullptr;
    QPushButton *m_btnFade = nullptr;
    QSlider *m_sldPosition = nullptr;

    QSlider *m_sldVolume = nullptr;
    QLineEdit *m_boxSearch = nullptr;
    QPushButton *m_btnSearch = nullptr;

private slots:
    void playClicked();
    void nextClicked();
    void prevClicked();
    void volumeChanged();
    void seekBegin();
    void seekEnd();
    void positionChanged(int position);

public slots:
    void setVolume(int volume);
    void setPosition(qint64 position);
    void setState(QMediaPlayer::State state);
    void songChanged(MSong *song);

signals:
    void playRequestedInitial();
    void playRequested();
    void pauseRequested();
    void volumeRequested(int volume);
    void seekRequested(qint64 position);
    void nextRequested();
    void prevRequested();
};


#endif
