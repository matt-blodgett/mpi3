#ifndef MPLAYBACK_H
#define MPLAYBACK_H

#include <QPushButton>
#include <QLineEdit>
#include <QSlider>


class PanelPlayback : public QWidget
{
    Q_OBJECT

public:
    explicit PanelPlayback(QWidget *parent=nullptr);
    ~PanelPlayback();

private:
    QWidget *frmVolume = nullptr;
    QWidget *frmPlayback = nullptr;
    QWidget *frmSearchbar = nullptr;

    QSlider *volSlider = nullptr;

    QPushButton *btnNext = nullptr;
    QPushButton *btnPrev = nullptr;
    QPushButton *btnPlay = nullptr;
//    QSlider *posSlider = nullptr;


    QLineEdit *boxSearch = nullptr;
    QPushButton *btnSearch = nullptr;




private:
    void paintEvent(QPaintEvent *event);

};

#endif // MPLAYBACK_H
