#include "mplayback.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>

#include <QLabel>

#include <QDebug>


PlaybackControl::PlaybackControl(QWidget *parent)
    : QWidget(parent)
{
    //    QGridLayout *layoutPlayback = new QGridLayout;
    //    layoutPlayback->addWidget(btnPrev, 0, 0, 1, 1);
    //    layoutPlayback->addWidget(btnPlay, 0, 1, 1, 1);
    //    layoutPlayback->addWidget(btnNext, 0, 2, 1, 1);


    //    frmPlayback->setLayout(layoutPlayback);


    btnNext = new QPushButton(this);
    btnPrev = new QPushButton(this);
    btnPlay = new QPushButton(this);

    btnNext->setText("Next");
    btnPrev->setText("Prev");
    btnPlay->setText("Play");


    this->update();

    btnPlay->move(btnPrev->width(), 0);
    btnNext->move(btnPrev->width() + btnPlay->width(), 0);
}


PlaybackControl::~PlaybackControl()
{

}

void PlaybackControl::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    qDebug() << this->size();

}


PanelPlayback::PanelPlayback(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *layoutMain = new QGridLayout;

    frmVolume = new QWidget(this);
    frmSearchbar = new QWidget(this);

    QGridLayout *layoutVolume = new QGridLayout;
    QGridLayout *layoutSearchbar = new QGridLayout;


    volSlider = new QSlider(frmVolume);
    volSlider->setOrientation(Qt::Horizontal);
    layoutVolume->addWidget(volSlider, 0, 0, 1, 1);
    frmVolume->setLayout(layoutVolume);




    frmPlayback = new PlaybackControl(this);




    boxSearch = new QLineEdit(frmSearchbar);
    btnSearch = new QPushButton(frmSearchbar);

    btnSearch->setText("S");
    layoutSearchbar->addWidget(boxSearch);
    layoutSearchbar->addWidget(btnSearch);
    frmSearchbar->setLayout(layoutSearchbar);



    layoutMain->addWidget(frmVolume, 0, 0, 1, 1);
    layoutMain->addWidget(frmPlayback, 0, 1, 1, 1);
    layoutMain->addWidget(frmSearchbar, 0, 2, 1, 1);

    layoutMain->setColumnStretch(1, 1);


    layoutMain->setMargin(0);
    this->setLayout(layoutMain);
}

PanelPlayback::~PanelPlayback()
{

}

void PanelPlayback::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}











