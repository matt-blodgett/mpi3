#ifndef MAUDIOCONTROL_H
#define MAUDIOCONTROL_H

#include <QWidget>
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLineEdit;
class QSlider;
class QTimer;
QT_END_NAMESPACE


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
    int volume() const;
    void setVolume(int volume);

    QMediaPlayer::State state() const;
    void setState(QMediaPlayer::State state);

private:
    QMediaPlayer::State m_playerState = QMediaPlayer::StoppedState;

private:
    QWidget *m_frmVolume = nullptr;
    QWidget *m_frmControls = nullptr;
    QWidget *m_frmSearchbar = nullptr;

    QPushButton *m_btnNext = nullptr;
    QPushButton *m_btnPrev = nullptr;
    QPushButton *m_btnPlay = nullptr;
    QSlider *m_sldPosition = nullptr;

    QPixmap m_pixNext;
    QPixmap m_pixPrev;
    QPixmap m_pixPlay;
    QPixmap m_pixPaus;

    QTimer *m_fadeTimer = nullptr;

    QSlider *m_sldVolume = nullptr;
    QLineEdit *m_boxSearch = nullptr;
    QPushButton *m_btnSearch = nullptr;

private:
    void clickedPlay();
    void clickedNext();
    void clickedPrev();
    void volumeSliderChanged();

signals:
    void audioPlay();
    void audioPause();
    void navigateNext();
    void navigatePrev();
    void changeVolume(int volume);

private:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *object, QEvent *event);
};


#endif
