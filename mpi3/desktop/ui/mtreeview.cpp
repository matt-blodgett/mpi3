#include "mtreeview.h"


Mpi3TreeView::Mpi3TreeView(QWidget *parent) : QTreeView(parent){
    setStyle(new Mpi3TreeViewStyle(style()));
}

Mpi3TreeView::IndicatorStyle Mpi3TreeView::dropIndicatorStyle() const{
    return m_currentStyle;
}
void Mpi3TreeView::setDropIndicatorStyle(IndicatorStyle style){
    m_currentStyle = style;
}

void Mpi3TreeView::paintEvent(QPaintEvent *event){
    DropIndicatorPosition position = dropIndicatorPosition();

    if(m_currentStyle == Mpi3TreeView::MoveIndicator){
        setDropIndicatorShown(position == QAbstractItemView::BelowItem || position == QAbstractItemView::AboveItem);
    }
    else if (m_currentStyle == Mpi3TreeView::DropIndicator){
        setDropIndicatorShown(position == QAbstractItemView::OnItem);
    }

    QTreeView::paintEvent(event);
    setDropIndicatorShown(true);
}


Mpi3TreeViewStyle::Mpi3TreeViewStyle(QStyle* style) : QProxyStyle(style){}

void Mpi3TreeViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const{
    if(element == QStyle::PE_IndicatorItemViewItemDrop && !option->rect.isNull()){
        QStyleOption opt(*option);
        opt.rect.setLeft(0);
        if (widget){
            opt.rect.setRight(widget->width());
        }

        QProxyStyle::drawPrimitive(element, &opt, painter, widget);
        return;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
