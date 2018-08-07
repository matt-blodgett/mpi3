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
    double getButtonOpacity() const;
    void setButtonOpacity(double opacity);
    void beginFadeButton();

    double m_btnOpacity = 100.00;

public:
    void setDisplay(Mpi3Song *song = nullptr);

    int volume() const;
    void setVolume(int volume);

    bool playing() const;
    void setPlaying(bool playing);

private:
    QString m_pidCurrentSong;
    bool m_playing = false;

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

private:
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
