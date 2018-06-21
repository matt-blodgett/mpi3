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




    frmPlayback->setLayout(layoutPlayback);
    frmSearchbar->setLayout(layoutSearchbar);



    layoutMain->addWidget(frmVolume, 0, 0, 1, 1);
    layoutMain->addWidget(frmPlayback, 0, 1, 1, 1);
    layoutMain->addWidget(frmSearchbar, 0, 2, 1, 1);


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











