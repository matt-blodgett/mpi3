#include "mtreeview.h"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>

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
            QDragEnterEvent *sc_event = static_cast<QDragEnterEvent*>(event);
            QModelIndex idx = indexAt(sc_event->pos());
            if(!idx.isValid()){
                sc_event->setDropAction(Qt::IgnoreAction);
            }
            else {
                sc_event->setDropAction(Qt::CopyAction);
            }
        }
        else if(event->type() == QEvent::DragMove){
            QDragMoveEvent *sc_event = static_cast<QDragMoveEvent*>(event);
            QModelIndex idx = indexAt(sc_event->pos());
            if(!idx.isValid()){
                sc_event->setDropAction(Qt::IgnoreAction);
            }
            else {
                sc_event->setDropAction(Qt::CopyAction);
            }
        }
    }

    return QTreeView::eventFilter(object, event);
}

void Mpi3TreeView::dragEnterEvent(QDragEnterEvent *event){
//    if(event->source() == this){
//        event->setDropAction(Qt::MoveAction);
//        m_drawStyle->setIndicatorStyle(Mpi3TreeViewStyle::IndicatorStyleMove);
//    }
//    else {
//        event->setDropAction(Qt::CopyAction);
//        m_drawStyle->setIndicatorStyle(Mpi3TreeViewStyle::IndicatorStyleDrop);
//    }

    QTreeView::dragEnterEvent(event);
    if(event->dropAction() == Qt::CopyAction){
        event->acceptProposedAction();
    }
}
void Mpi3TreeView::dragMoveEvent(QDragMoveEvent *event){
    QTreeView::dragMoveEvent(event);
    event->acceptProposedAction();
}
void Mpi3TreeView::dragLeaveEvent(QDragLeaveEvent *event){
    QTreeView::dragLeaveEvent(event);
    event->accept();
}
void Mpi3TreeView::dropEvent(QDropEvent *event){
    QTreeView::dropEvent(event);
    event->acceptProposedAction();
    event->accept();
}

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
