#include "mplayback.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>

#include <QLabel>


PanelPlayback::PanelPlayback(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *layoutMain = new QGridLayout;


    frmVolume = new QWidget;
    frmPlayback = new QWidget;
    frmSearchbar = new QWidget;

    QGridLayout *layoutVolume = new QGridLayout;
    QGridLayout *layoutPlayback = new QGridLayout;
    QGridLayout *layoutSearchbar = new QGridLayout;


    volSlider = new QSlider;
    volSlider->setOrientation(Qt::Horizontal);

    layoutVolume->addWidget(volSlider, 0, 0, 1, 1);

    frmVolume->setLayout(layoutVolume);


    btnNext = new QPushButton;
    btnPrev = new QPushButton;
    btnPlay = new QPushButton;

    btnNext->setText("Next");
    btnPrev->setText("Prev");
    btnPlay->setText("Play");


    layoutPlayback->addWidget(btnPrev, 0, 0, 1, 1);
    layoutPlayback->addWidget(btnPlay, 0, 1, 1, 1);
    layoutPlayback->addWidget(btnNext, 0, 2, 1, 1);


    frmPlayback->setLayout(layoutPlayback);



    boxSearch = new QLineEdit;
    btnSearch = new QPushButton;

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











