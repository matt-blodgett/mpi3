#include "uistyle.h"

#include <QStyleOption>
#include <QPainter>


Mpi3Style::Mpi3Style(QStyle *style) : QProxyStyle(style){}

void Mpi3Style::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const{
    if(element == QStyle::PE_IndicatorRadioButton){
        return;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
