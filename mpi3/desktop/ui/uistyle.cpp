#include "uistyle.h"

#include <QStyleOption>
#include <QPainter>


#include <QPalette>


#include <QDebug>


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


//    if(widget->objectName() == "ContainersTreeview"){
//        qDebug() << element;
//    }

    if(element == QStyle::PE_Widget){
        if(noDraw){
            return;
        }
    }

    if(element == QStyle::PE_PanelItemViewRow || element == QStyle::PE_PanelItemViewItem){
        if(noDraw){
            if(painter){

                qDebug() << "style row";

                QStyleOption opt(*option);
                opt.rect.setLeft(0);

                QColor red(250, 42, 63, 255);
                opt.palette.setColor(QPalette::Active, QPalette::WindowText, red);
                opt.palette.setColor(QPalette::Active, QPalette::Button, red);
                opt.palette.setColor(QPalette::Active, QPalette::Text, red);
                opt.palette.setColor(QPalette::Active, QPalette::ButtonText, red);
                opt.palette.setColor(QPalette::Active, QPalette::Base, red);
                opt.palette.setColor(QPalette::Active, QPalette::Window, red);
                opt.palette.setColor(QPalette::Active, QPalette::Highlight, red);
                opt.palette.setColor(QPalette::Active, QPalette::HighlightedText, red);

                opt.palette.setColor(QPalette::Disabled, QPalette::WindowText, red);
                opt.palette.setColor(QPalette::Disabled, QPalette::Button, red);
                opt.palette.setColor(QPalette::Disabled, QPalette::Text, red);
                opt.palette.setColor(QPalette::Disabled, QPalette::ButtonText, red);
                opt.palette.setColor(QPalette::Disabled, QPalette::Base, red);
                opt.palette.setColor(QPalette::Disabled, QPalette::Window, red);
                opt.palette.setColor(QPalette::Disabled, QPalette::Highlight, red);
                opt.palette.setColor(QPalette::Disabled, QPalette::HighlightedText, red);

                opt.palette.setColor(QPalette::Inactive, QPalette::WindowText, red);
                opt.palette.setColor(QPalette::Inactive, QPalette::Button, red);
                opt.palette.setColor(QPalette::Inactive, QPalette::Text, red);
                opt.palette.setColor(QPalette::Inactive, QPalette::ButtonText, red);
                opt.palette.setColor(QPalette::Inactive, QPalette::Base, red);
                opt.palette.setColor(QPalette::Inactive, QPalette::Window, red);
                opt.palette.setColor(QPalette::Inactive, QPalette::Highlight, red);
                opt.palette.setColor(QPalette::Inactive, QPalette::HighlightedText, red);

                painter->setBackground()

                QProxyStyle::drawPrimitive(element, &opt, painter, widget);

                return;
            }
        }
    }

    if(element == QStyle::PE_IndicatorItemViewItemDrop && !option->rect.isNull()){



        if(painter){
            QStyleOption opt(*option);
            opt.rect.setLeft(0);

//            qDebug() << "";
//            qDebug() << opt.palette;


            if(noDraw){
                qDebug() << "draw tv indicator" << "no draw == TRUE";

                QColor red(250, 42, 63, 255);
                opt.palette.setColor(QPalette::Active, QPalette::WindowText, red);
                opt.palette.setColor(QPalette::Active, QPalette::Button, red);
                opt.palette.setColor(QPalette::Active, QPalette::Text, red);
                opt.palette.setColor(QPalette::Active, QPalette::ButtonText, red);
                opt.palette.setColor(QPalette::Active, QPalette::Base, red);
                opt.palette.setColor(QPalette::Active, QPalette::Window, red);
                opt.palette.setColor(QPalette::Active, QPalette::Highlight, red);
                opt.palette.setColor(QPalette::Active, QPalette::HighlightedText, red);

                opt.palette.setColor(QPalette::Disabled, QPalette::WindowText, red);
                opt.palette.setColor(QPalette::Disabled, QPalette::Button, red);
                opt.palette.setColor(QPalette::Disabled, QPalette::Text, red);
                opt.palette.setColor(QPalette::Disabled, QPalette::ButtonText, red);
                opt.palette.setColor(QPalette::Disabled, QPalette::Base, red);
                opt.palette.setColor(QPalette::Disabled, QPalette::Window, red);
                opt.palette.setColor(QPalette::Disabled, QPalette::Highlight, red);
                opt.palette.setColor(QPalette::Disabled, QPalette::HighlightedText, red);

                opt.palette.setColor(QPalette::Inactive, QPalette::WindowText, red);
                opt.palette.setColor(QPalette::Inactive, QPalette::Button, red);
                opt.palette.setColor(QPalette::Inactive, QPalette::Text, red);
                opt.palette.setColor(QPalette::Inactive, QPalette::ButtonText, red);
                opt.palette.setColor(QPalette::Inactive, QPalette::Base, red);
                opt.palette.setColor(QPalette::Inactive, QPalette::Window, red);
                opt.palette.setColor(QPalette::Inactive, QPalette::Highlight, red);
                opt.palette.setColor(QPalette::Inactive, QPalette::HighlightedText, red);

//                QTreeWidget::branch::!has-children:selected {background-color: rgb(255, 255, 255);}
//                QTreeWidget::branch::!has-children:selected:alternate {background-color: rgb(0, 0, 0);}
//                qDebug() << opt.palette;
            }
            else {
                qDebug() << "draw tv indicator" << "no draw == FALSE";
            }


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
