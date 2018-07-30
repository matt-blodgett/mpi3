#ifndef AUDIOCONTROL_H
#define AUDIOCONTROL_H

#include <QMediaPlayer>
#include <QWidget>
#include <QIcon>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QAbstractSlider;
class QPushButton;
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
    QWidget *frm_volume = nullptr;
    QWidget *frm_controls = nullptr;
    QWidget *frm_searchbar = nullptr;

    QPushButton *btn_next = nullptr;
    QPushButton *btn_prev = nullptr;
    QPushButton *btn_play = nullptr;
    QAbstractSlider *sld_position = nullptr;

    QIcon icn_next;
    QIcon icn_prev;
    QIcon icn_play;
    QIcon icn_paus;

    QLineEdit *box_search = nullptr;
    QPushButton *btn_search = nullptr;
    QAbstractSlider *sld_volume = nullptr;

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
