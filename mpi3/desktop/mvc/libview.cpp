#include "libview.h"

#include "desktop/ui/uistyle.h"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>

#include <QHeaderView>

#include <QPainter>
#include <QPaintEngine>
#include <QDebug>


Mpi3TreeViewStyle::Mpi3TreeViewStyle(QStyle* style) : QProxyStyle(style){}

Mpi3TreeViewStyle::IndicatorStyle Mpi3TreeViewStyle::indicatorStyle() const{
    return m_indicatorStyle;
}
void Mpi3TreeViewStyle::setIndicatorStyle(Mpi3TreeViewStyle::IndicatorStyle iStyle){
    m_indicatorStyle = iStyle;
}

void Mpi3TreeViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const{
    if(element == QStyle::PE_IndicatorItemViewItemDrop && !option->rect.isNull()){
        if(painter){
            QStyleOption opt(*option);
            opt.rect.setLeft(0);

            if(widget){
                opt.rect.setRight(widget->width());
            }

            switch(m_indicatorStyle){

                case Mpi3TreeViewStyle::IndicatorStyleMove: {
                    opt.rect.setHeight(0);
                    break;
                }

                case Mpi3TreeViewStyle::IndicatorStyleDrop: {
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


Mpi3TreeView::Mpi3TreeView(QWidget *parent) : QTreeView(parent){
    m_drawStyle = new Mpi3TreeViewStyle(style());
    setStyle(m_drawStyle);

    setDragEnabled(true);
    setAcceptDrops(true);
    viewport()->setAcceptDrops(true);

    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DragDrop);

    setSelectionBehavior(QAbstractItemView::SelectRows);

    setContextMenuPolicy(Qt::CustomContextMenu);
    header()->setContextMenuPolicy(Qt::CustomContextMenu);
}


Mpi3TreeViewContainers::Mpi3TreeViewContainers(QWidget *parent) : Mpi3TreeView(parent){
    setSelectionMode(QAbstractItemView::SingleSelection);
    setAlternatingRowColors(false);
    setRootIsDecorated(true);
    setHeaderHidden(true);
    setIndentation(12);
}

void Mpi3TreeViewContainers::dragEnterEvent(QDragEnterEvent *event){
    if(event->source() == this){
        m_drawStyle->setIndicatorStyle(Mpi3TreeViewStyle::IndicatorStyleMove);
    }
    else {
        m_drawStyle->setIndicatorStyle(Mpi3TreeViewStyle::IndicatorStyleDrop);
    }

//    Qt::DropAction action = event->source() == this ? Qt::MoveAction : Qt::CopyAction;
//    QModelIndex dropIndex = indexAt(event->pos());
//    if(model()->canDropMimeData(event->mimeData(), action, dropIndex.row(), dropIndex.column(), dropIndex.parent())){
//        qDebug() << "tree: drag enter - accepted";
////        event->acceptProposedAction();
////        event->accept();
//    }
//    else {
//        qDebug() << "tree: drag enter - NOT accepted";
//    }

    Qt::DropAction action = event->source() == this ? Qt::MoveAction : Qt::CopyAction;

    QModelIndex dropIndex = currentIndex();
    if(model()->canDropMimeData(event->mimeData(), action, dropIndex.row(), dropIndex.column(), dropIndex.parent())){
        qDebug() << "tree: drag enter - accepted" << action << dropIndex.row() << dropIndex.column() << dropIndex.parent().row() << dropIndex.parent().column();
//        event->acceptProposedAction();
//        event->accept();
    }
    else {
        qDebug() << "tree: drag enter - NOT accepted" << action << dropIndex.row() << dropIndex.column() << dropIndex.parent().row() << dropIndex.parent().column();;

    }
    qDebug() << "";

    QTreeView::dragEnterEvent(event);
    event->acceptProposedAction();
}
void Mpi3TreeViewContainers::dragMoveEvent(QDragMoveEvent *event){
    Qt::DropAction action = event->source() == this ? Qt::MoveAction : Qt::CopyAction;

//    QModelIndex dropIndex = currentIndex();

    QModelIndex dropIndex = indexAt(event->pos());

//    if(model()->canDropMimeData(event->mimeData(), action, dropIndex.row(), dropIndex.column(), dropIndex.parent())){
//        qDebug() << "tree: drag move - accepted" << action << dropIndex.row() << dropIndex.column() << dropIndex.parent().row() << dropIndex.parent().column();;
////        event->acceptProposedAction();
////        event->accept();
//    }
//    else {
//        qDebug() << "tree: drag move - NOT accepted" << action << dropIndex.row() << dropIndex.column() << dropIndex.parent().row() << dropIndex.parent().column();;

//    }


    QTreeView::dragMoveEvent(event);


    if(model()->canDropMimeData(event->mimeData(), action, dropIndex.row(), dropIndex.column(), dropIndex.parent())){
        qDebug() << "tree: drag move - accepted" << action << dropIndex.row() << dropIndex.column() << dropIndex.parent().row() << dropIndex.parent().column();;
        event->acceptProposedAction();
//        event->accept();
    }
    else {
        qDebug() << "tree: drag move - NOT accepted" << action << dropIndex.row() << dropIndex.column() << dropIndex.parent().row() << dropIndex.parent().column();;

    }

//    qDebug() << "";

//    event->acceptProposedAction();

}
void Mpi3TreeViewContainers::dropEvent(QDropEvent *event){
    Qt::DropAction action = event->source() == this ? Qt::MoveAction : Qt::CopyAction;
    QModelIndex dropIndex = indexAt(event->pos());
    if(model()->canDropMimeData(event->mimeData(), action, dropIndex.row(), dropIndex.column(), dropIndex.parent())){

        qDebug() << "tree: drag DROP - accepted" << action << dropIndex.row() << dropIndex.column() << dropIndex.parent().row() << dropIndex.parent().column();;

//        model()->dropMimeData(event->mimeData(), action, dropIndex.row(), dropIndex.column(), dropIndex.parent());
//        event->acceptProposedAction();
//        event->accept();
    }
    else {
        qDebug() << "tree: drag DROP - NOT accepted" << action << dropIndex.row() << dropIndex.column() << dropIndex.parent().row() << dropIndex.parent().column();;

    }

    qDebug() << "";
    update();

}

Mpi3TreeViewSonglist::Mpi3TreeViewSonglist(QWidget *parent) : Mpi3TreeView(parent){
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setAlternatingRowColors(true);
    setRootIsDecorated(false);
    setHeaderHidden(false);
    setIndentation(12);

    m_drawStyle->setIndicatorStyle(Mpi3TreeViewStyle::IndicatorStyleMove);
}

void Mpi3TreeViewSonglist::dropEvent(QDropEvent *event){
    Qt::DropAction action = event->source() == this ? Qt::MoveAction : Qt::CopyAction;
    QModelIndex dropIndex = indexAt(event->pos());
    if(model()->canDropMimeData(event->mimeData(), action, dropIndex.row(), dropIndex.column(), dropIndex.parent())){
        model()->dropMimeData(event->mimeData(), action, dropIndex.row(), dropIndex.column(), dropIndex.parent());
        event->acceptProposedAction();
        event->accept();
    }
}
