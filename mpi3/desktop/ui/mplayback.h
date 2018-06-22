#ifndef MPLAYBACK_H
#define MPLAYBACK_H

#include <QPushButton>
#include <QLineEdit>
#include <QSlider>


// ----------------------------------------------------------------------------------------------------
// * VolumeControl *
// ----------------------------------------------------------------------------------------------------
class VolumeControl : public QWidget
{
    Q_OBJECT

public:
    explicit VolumeControl(QWidget *parent=nullptr);
    ~VolumeControl();

private:
    QSlider *volSlider = nullptr;

};


// ----------------------------------------------------------------------------------------------------
// * PlaybackControl *
// ----------------------------------------------------------------------------------------------------
class PlaybackControl : public QWidget
{
    Q_OBJECT

public:
    explicit PlaybackControl(QWidget *parent=nullptr);
    ~PlaybackControl();

private:
    QWidget *container = nullptr;

    QPushButton *btnNext = nullptr;
    QPushButton *btnPrev = nullptr;
    QPushButton *btnPlay = nullptr;
    QSlider *posSlider = nullptr;

private:
    void resizeEvent(QResizeEvent *event);

};


// ----------------------------------------------------------------------------------------------------
// * LibrarySearchbar *
// ----------------------------------------------------------------------------------------------------
class LibrarySearchbar : public QWidget
{
    Q_OBJECT

public:
    explicit LibrarySearchbar(QWidget *parent=nullptr);
    ~LibrarySearchbar();

private:
    QLineEdit *boxSearch = nullptr;
    QPushButton *btnSearch = nullptr;

};


// ----------------------------------------------------------------------------------------------------
// * PanelPlayback *
// ----------------------------------------------------------------------------------------------------
class PanelPlayback : public QWidget
{
    Q_OBJECT

public:
    explicit PanelPlayback(QWidget *parent=nullptr);
    ~PanelPlayback();

private:
    VolumeControl *frmVolume = nullptr;
    PlaybackControl *frmPlayback = nullptr;
    LibrarySearchbar *frmSearchbar = nullptr;

private:
    void paintEvent(QPaintEvent *event);

};


#endif // MPLAYBACK_H
