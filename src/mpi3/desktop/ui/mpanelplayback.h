#pragma once

#ifndef MPANELPLAYBACK_H
#define MPANELPLAYBACK_H


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

    void setVolume(int volume);
    void setPosition(double position);

    void setState(Mpi3::EngineState state);
    void setDisplay(MSong *song);

private:
    Mpi3::EngineState m_currentState;
    QString m_pidCurrentSong;
    bool m_navigating;
    bool m_seeking;

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
    void seekBegin();
    void seekEnd();
    void positionChanged(int position);

signals:
    void audioPlay();
    void audioPause();
    void audioSeek(int position);
    void changeVolume(int volume);
    void navigateNext();
    void navigatePrev();

public slots:
    void elementModified(MMediaElement *elemModified);

protected:
    void paintEvent(QPaintEvent *event);

};


#endif
