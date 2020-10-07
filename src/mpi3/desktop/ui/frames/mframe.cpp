#include "mpi3/desktop/ui/frames/mframe.h"

#include <QPainter>
#include <QStyleOption>


MFrame::MFrame(QWidget *parent) : QWidget(parent)
{

}

void MFrame::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QWidget::paintEvent(event);
}
