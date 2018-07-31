#include "uistyle.h"

#include <QStyleOption>
#include <QPainter>


Mpi3Style::Mpi3Style(QStyle *style) : QProxyStyle(style){}

Mpi3Style::TV_IndicatorStyle Mpi3Style::currentTVIndicatorStyle() const{
    return m_currentTVIndicatorStyle;
}
void Mpi3Style::setTVIndicatorStyle(Mpi3Style::TV_IndicatorStyle tvIndicatorStyle){
    m_currentTVIndicatorStyle = tvIndicatorStyle;
}

void Mpi3Style::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const{
    if(element == QStyle::PE_IndicatorRadioButton){
        return;
    }

    if(element == QStyle::PE_IndicatorItemViewItemDrop && !option->rect.isNull()){
        if(painter){
            QStyleOption opt(*option);
            opt.rect.setLeft(0);

            if(widget){
                opt.rect.setRight(widget->width());
            }

            switch(m_currentTVIndicatorStyle){

                case Mpi3Style::TV_IndicatorStyleMove: {
                    opt.rect.setHeight(0);
                    break;
                }

                case Mpi3Style::TV_IndicatorStyleDrop: {
                    if(option->rect.y() % 21 > 0){
                        opt.rect.setY(option->rect.y() - 21 + 1);
                    }
                    opt.rect.setHeight(21);
                    break;
                }
            }

            QProxyStyle::drawPrimitive(element, &opt, painter, widget);
        }

        return;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
