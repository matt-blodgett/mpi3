#ifndef MPLAYBACK_H
#define MPLAYBACK_H

#include <QPushButton>
#include <QLineEdit>
#include <QSlider>



class PlaybackControl : public QWidget
{
    Q_OBJECT

public:
    explicit PlaybackControl(QWidget *parent=nullptr);
    ~PlaybackControl();

private:
    QPushButton *btnNext = nullptr;
    QPushButton *btnPrev = nullptr;
    QPushButton *btnPlay = nullptr;
    //    QSlider *posSlider = nullptr;

private:
    void resizeEvent(QResizeEvent *event);

};


class PanelPlayback : public QWidget
{
    Q_OBJECT

public:
    explicit PanelPlayback(QWidget *parent=nullptr);
    ~PanelPlayback();

private:
    QWidget *frmVolume = nullptr;
//    QWidget *frmPlayback = nullptr;
    QWidget *frmSearchbar = nullptr;

    QSlider *volSlider = nullptr;


    PlaybackControl *frmPlayback = nullptr;

    QLineEdit *boxSearch = nullptr;
    QPushButton *btnSearch = nullptr;

private:
    void paintEvent(QPaintEvent *event);

};





#endif // MPLAYBACK_H
