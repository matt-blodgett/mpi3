#include "mpanel.h"

#include <QStyleOption>
#include <QPainter>


MPanel::MPanel(QWidget *parent) : QWidget(parent){}

void MPanel::paintEvent(QPaintEvent *event){
    QStyleOption opt;
    opt.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QWidget::paintEvent(event);
}






















