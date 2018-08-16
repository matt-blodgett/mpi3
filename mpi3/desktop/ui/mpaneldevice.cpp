#include "mpaneldevice.h"

#include <QStyleOption>
#include <QPainter>


MPanelDevice::MPanelDevice(QWidget *parent) : QWidget(parent){
    initializeLayout();
}
MPanelDevice::~MPanelDevice(){}

void MPanelDevice::initializeLayout(){

}

void MPanelDevice::paintEvent(QPaintEvent *event){
    QStyleOption opt;
    opt.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QWidget::paintEvent(event);
}
void MPanelDevice::showEvent(QShowEvent *event){
//    m_frmLibrary->setSizes({180, width()-180});
    QWidget::showEvent(event);
}
