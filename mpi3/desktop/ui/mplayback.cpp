#include "mplayback.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>

#include <QLabel>

#include <QDebug>


// https://doc.qt.io/qt-5/qtwidgets-widgets-icons-example.html#



// ----------------------------------------------------------------------------------------------------
// * PlaybackControl *
// ----------------------------------------------------------------------------------------------------
PlaybackControl::PlaybackControl(QWidget *parent)
    : QWidget(parent)
{
    container = new QWidget(this);

    btnNext = new QPushButton(container);
    btnPrev = new QPushButton(container);
    btnPlay = new QPushButton(container);

    btnNext->setText("Next");
    btnPrev->setText("Prev");
    btnPlay->setText("Play");


    btnPlay->move(btnPrev->width(), 0);
    btnNext->move(btnPrev->width() + btnPlay->width(), 0);
}

PlaybackControl::~PlaybackControl()
{

}

void PlaybackControl::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    int x = (this->width() / 2) - (container->width() / 2);
    container->move(x, 0);

    btnPrev->set
}


// ----------------------------------------------------------------------------------------------------
// * PanelPlayback *
// ----------------------------------------------------------------------------------------------------
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

    layoutMain->setColumnMinimumWidth(0, 150);
    layoutMain->setColumnMinimumWidth(1, 400);
    layoutMain->setColumnMinimumWidth(2, 150);

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











