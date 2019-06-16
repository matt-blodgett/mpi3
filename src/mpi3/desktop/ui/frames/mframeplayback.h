#pragma once

#ifndef MFRAMEPLAYBACK_H
#define MFRAMEPLAYBACK_H


#include "mframe.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QLineEdit;
class QSlider;
class QLabel;
QT_END_NAMESPACE


#include "mglobal.h"


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
    double position() const;

    void setVolume(int volume);
    void setPosition(double position);

    void setState(Mpi3::EngineState state);
    void setDisplay(MSong *song);

private:
    Mpi3::EngineState m_currentState;
    bool m_navigating;
    bool m_seeking;
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

signals:
    void playRequested();
    void pausRequested();
    void seekRequested(int position);
    void volumeRequested(int volume);
    void nextRequested();
    void prevRequested();

public slots:
    void elementModified(MMediaElement *elemModified);

};


#endif
