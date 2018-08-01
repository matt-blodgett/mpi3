#ifndef MAUDIOCONTROL_H
#define MAUDIOCONTROL_H

#include <QMediaPlayer>
#include <QWidget>
#include <QIcon>

QT_BEGIN_NAMESPACE
class QAbstractSlider;
class QPushButton;
class QLineEdit;
QT_END_NAMESPACE


class Mpi3PanelPlayback : public QWidget
{
    Q_OBJECT

public:
    explicit Mpi3PanelPlayback(QWidget *parent=nullptr);
    ~Mpi3PanelPlayback();

public:
    QMediaPlayer *m_audio = nullptr;
    QMediaPlayer::State state() const;
    int volume() const;
    bool isMuted() const;

private:
    QMediaPlayer::State m_playerState = QMediaPlayer::StoppedState;
    bool m_playerMuted = false;

private:
    QWidget *m_frmVolume = nullptr;
    QWidget *m_frmControls = nullptr;
    QWidget *m_frmSearchbar = nullptr;

    QPushButton *m_btnNext = nullptr;
    QPushButton *m_btnPrev = nullptr;
    QPushButton *m_btnPlay = nullptr;
    QAbstractSlider *m_sldPosition = nullptr;

    QIcon m_icnNext;
    QIcon m_icnPrev;
    QIcon m_icnPlay;
    QIcon m_icnPaus;

    QLineEdit *m_boxSearch = nullptr;
    QPushButton *m_btnSearch = nullptr;
    QAbstractSlider *m_sldVolume = nullptr;

signals:
    void play();
    void pause();
    void stop();
    void next();
    void previous();
    void changeVolume(int volume);
    void changeMuting(bool muting);

public:
    void setState(QMediaPlayer::State state);
    void setVolume(int volume);
    void setMuted(bool muted);

private:
    void clickPlay();
    void clickMute();
    void onVolumeSliderValueChanged();

private:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

};


#endif
