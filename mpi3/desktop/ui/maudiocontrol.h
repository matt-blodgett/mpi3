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

#include "core/mglobal.h"

class Mpi3Element;
class Mpi3Song;


class Mpi3PanelPlayback : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double buttonOpacity READ getButtonOpacity WRITE setButtonOpacity)

public:
    explicit Mpi3PanelPlayback(QWidget *parent = nullptr);
    ~Mpi3PanelPlayback();

private:
    void initializeLayout();

private:
    double m_btnOpacity = 100.00;
    double getButtonOpacity() const;
    void setButtonOpacity(double opacity);
    void beginFadeButton();

public:
    int volume() const;
    bool stopped() const;
    bool playing() const;
    bool paused() const;
    Mpi3::EngineState currentState() const;

    void setVolume(int volume);
    void setState(Mpi3::EngineState state);
    void setDisplay(Mpi3Song *song = nullptr);

private:
    Mpi3::EngineState m_currentState;
    QString m_pidCurrentSong;

private:
    QWidget *m_frmVolume = nullptr;
    QWidget *m_frmControls = nullptr;
    QWidget *m_frmSearchbar = nullptr;

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

public:
    void elementModified(Mpi3Element *elemModified);

private:
    void paintEvent(QPaintEvent *event);

};


#endif
