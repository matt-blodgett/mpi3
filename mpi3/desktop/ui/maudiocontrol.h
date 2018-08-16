#pragma once

#ifndef MAUDIOCONTROL_H
#define MAUDIOCONTROL_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLineEdit;
class QSlider;
class QLabel;
class QTimer;
QT_END_NAMESPACE


#include "mglobal.h"


class MPanelPlayback : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double buttonOpacity READ buttonOpacity WRITE setButtonOpacity)

public:
    explicit MPanelPlayback(QWidget *parent = nullptr);
    ~MPanelPlayback();

private:
    void initializeLayout();

private:
    double m_btnOpacity = 100.00;
    double buttonOpacity() const;
    void setButtonOpacity(double opacity);
    void animateFadeButton();

public:
    int volume() const;
    double position() const;

    bool stopped() const;
    bool playing() const;
    bool paused() const;
    Mpi3::EngineState currentState() const;

    void setVolume(int volume);
    void setPosition(double position);
    void setState(Mpi3::EngineState state);
    void setDisplay(MSong *song = nullptr);

private:
    Mpi3::EngineState m_currentState;
    QString m_pidCurrentSong;
    bool m_navigating;

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

    QPixmap m_pixNext;
    QPixmap m_pixPrev;
    QPixmap m_pixPlay;
    QPixmap m_pixPaus;
    QPixmap m_pixSearch;

    QSlider *m_sldVolume = nullptr;
    QLineEdit *m_boxSearch = nullptr;
    QPushButton *m_btnSearch = nullptr;

private slots:
    void clickedPlay();
    void clickedNext();
    void clickedPrev();
    void volumeChanged();

signals:
    void audioPlay();
    void audioPause();
    void navigateNext();
    void navigatePrev();
    void changeVolume(int volume);
    void changePosition(double position);

public:
    void elementModified(MMediaElement *elemModified);

private:
    void paintEvent(QPaintEvent *event);

};


#endif
