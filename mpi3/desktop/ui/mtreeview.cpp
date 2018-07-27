#include "mtreeview.h"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>

#include <QMimeData>


#include <QHeaderView>


#include <QDebug>


Mpi3TreeView::Mpi3TreeView(QWidget *parent) : QTreeView(parent){
    m_drawStyle = new Mpi3TreeViewStyle(style());
    setStyle(m_drawStyle);

    setDragEnabled(true);
    setAcceptDrops(true);
    viewport()->setAcceptDrops(true);
    viewport()->installEventFilter(this);

    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DragDrop);

    setSelectionBehavior(QAbstractItemView::SelectRows);

    setContextMenuPolicy(Qt::CustomContextMenu);
    header()->setContextMenuPolicy(Qt::CustomContextMenu);
}

Mpi3TreeView::DisplayStyle Mpi3TreeView::displayStyle() const{
    return m_displayStyle;
}
void Mpi3TreeView::setDisplayStyle(Mpi3TreeView::DisplayStyle dStyle){
    m_displayStyle = dStyle;

    switch(m_displayStyle){
        case Mpi3TreeView::DisplayStyleSonglist:
            setSelectionMode(QAbstractItemView::ExtendedSelection);
            setAlternatingRowColors(true);
            setRootIsDecorated(false);
            setHeaderHidden(false);
            setIndentation(12);
            break;

        case Mpi3TreeView::DisplayStyleContainers:
            setSelectionMode(QAbstractItemView::SingleSelection);
            setAlternatingRowColors(false);
            setRootIsDecorated(true);
            setHeaderHidden(true);
            setIndentation(12);
            break;
    }
}

bool Mpi3TreeView::eventFilter(QObject *object, QEvent *event){

    if(object == viewport()){

        if(event->type() == QEvent::DragEnter){

//            QDragEnterEvent *sc_event = static_cast<QDragEnterEvent*>(event);

//            int x = width() / 2;
//            int y = sc_event->pos().y();

//            qDebug() << sc_event->pos().x() << sc_event->pos().y() << "-" << x << y;

//            sc_event->acceptProposedAction();
//            QTreeView::dragEnterEvent(sc_event);
//            return true;
        }
        else if(event->type() == QEvent::DragMove){
//            QDragMoveEvent *sc_event = static_cast<QDragMoveEvent*>(event);

//            int x = width() / 2;
//            int y = sc_event->pos().y();

//            DropIndicatorPosition dropPos = dropIndicatorPosition();
//            if(dropPos != QAbstractItemView::OnItem){
//                double y_offset = round(21 / 4) + 1;
//                if(dropPos == QAbstractItemView::AboveItem){
//                    y += y_offset;
//                }
//                else if(dropPos == QAbstractItemView::BelowItem){
//                    y -= y_offset;
//                }
//            }

//            qDebug() << "";
//            qDebug() << sc_event->pos().x() << sc_event->pos().y() << "-" << x << y;



//            sc_event->setAccepted(false);



////            sc_event->pos().setX(x);
//            sc_event->pos().setY(y);

//            sc_event->acceptProposedAction();
//            QTreeView::dragMoveEvent(sc_event);
//            return true;
        }
    }

    return QTreeView::eventFilter(object, event);
}

//void Mpi3TreeView::dragEnterEvent(QDragEnterEvent *event){
////    if(event->source() == this){
////        event->setDropAction(Qt::MoveAction);
////        m_drawStyle->setIndicatorStyle(Mpi3TreeViewStyle::IndicatorStyleMove);
////    }
////    else {
////        event->setDropAction(Qt::CopyAction);
////        m_drawStyle->setIndicatorStyle(Mpi3TreeViewStyle::IndicatorStyleDrop);
////    }


//    event->acceptProposedAction();
////    qDebug() << "accept";
////    event->acceptProposedAction();
////    QTreeView::eventFilter(this, event);

////    QWidget::dragEnterEvent(event);
//}
//void Mpi3TreeView::dragMoveEvent(QDragMoveEvent *event){

////    qDebug() << "1234";

////    int x = width() / 2;
////    int y = event->pos().y();

////    DropIndicatorPosition dropPos = dropIndicatorPosition();
////    if(dropPos != QAbstractItemView::OnItem){
////        double y_offset = round(21 / 4) + 1;
////        if(dropPos == QAbstractItemView::AboveItem){
////            y += y_offset;
////        }
////        else if(dropPos == QAbstractItemView::BelowItem){
////            y -= y_offset;
////        }

////        qDebug() << y_offset;
////    }

////    event->pos().setX(x);
////    event->pos().setY(y);


////    qDebug() << "asdf";

//    event->acceptProposedAction();
////    QTreeView::dragMoveEvent(event);

////    QTreeView::eventFilter(this, event);
//}
//void Mpi3TreeView::dragLeaveEvent(QDragLeaveEvent *event){
//    QTreeView::dragLeaveEvent(event);
//}
//void Mpi3TreeView::dropEvent(QDropEvent *event){
//    QTreeView::dropEvent(event);
//}

Mpi3TreeViewStyle::Mpi3TreeViewStyle(QStyle* style) : QProxyStyle(style){}

Mpi3TreeViewStyle::IndicatorStyle Mpi3TreeViewStyle::indicatorStyle() const{
    return m_indicatorStyle;
}
void Mpi3TreeViewStyle::setIndicatorStyle(Mpi3TreeViewStyle::IndicatorStyle iStyle){
    m_indicatorStyle = iStyle;
}

void Mpi3TreeViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const{
    if(element == QStyle::PE_IndicatorItemViewItemDrop && !option->rect.isNull()){
        QStyleOption opt(*option);
        opt.rect.setLeft(0);

        if(widget){
            opt.rect.setRight(widget->width());
        }

        switch(m_indicatorStyle){
            case Mpi3TreeViewStyle::IndicatorStyleMove:
                opt.rect.setHeight(0);
                break;
            case Mpi3TreeViewStyle::IndicatorStyleDrop:
                if(option->rect.y() % 21 > 0){
                    opt.rect.setY(option->rect.y() - 21 + 1);
                }
                opt.rect.setHeight(21);
                break;
        }

        QProxyStyle::drawPrimitive(element, &opt, painter, widget);
        return;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
