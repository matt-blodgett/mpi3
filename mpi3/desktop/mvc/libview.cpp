#include "libview.h"

#include "desktop/ui/uistyle.h"

#include <QHeaderView>
#include <QDropEvent>


#include <QDebug>


Mpi3TreeView::Mpi3TreeView(QWidget *parent) : QTreeView(parent){
    m_drawStyle = new Mpi3Style(style());
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

    m_drawStyle->setTVIndicatorStyle(Mpi3Style::TV_IndicatorStyleDrop);
}

void Mpi3TreeViewContainers::dragEnterEvent(QDragEnterEvent *event){


    Qt::DropAction action = event->source() == this ? Qt::MoveAction : Qt::CopyAction;
    QModelIndex dropIndex = currentIndex();
    if(model()->canDropMimeData(event->mimeData(), action, dropIndex.row(), dropIndex.column(), dropIndex.parent())){
//        qDebug() << "tree: drag enter - accepted" << action << dropIndex.row() << dropIndex.column() << dropIndex.parent().row() << dropIndex.parent().column();

    }
    else {
//        qDebug() << "tree: drag enter - NOT accepted" << action << dropIndex.row() << dropIndex.column() << dropIndex.parent().row() << dropIndex.parent().column();;

    }
    qDebug() << "";


    QTreeView::dragEnterEvent(event);
    event->acceptProposedAction();



}
void Mpi3TreeViewContainers::dragMoveEvent(QDragMoveEvent *event){




    Qt::DropAction action = event->source() == this ? Qt::MoveAction : Qt::CopyAction;
    QModelIndex dropIndex = indexAt(event->pos());


//    qDebug() << "";
    if(model()->canDropMimeData(event->mimeData(), action, dropIndex.row(), dropIndex.column(), dropIndex.parent())){
//        qDebug() << "tree: drag move - accepted" << action << dropIndex.row() << dropIndex.column() << dropIndex.parent().row() << dropIndex.parent().column();

        m_drawStyle->noDraw = false;

//        event->acceptProposedAction();
//        event->accept();
    }
    else {
//        qDebug() << "tree: drag move - NOT accepted" << action << dropIndex.row() << dropIndex.column() << dropIndex.parent().row() << dropIndex.parent().column();


        m_drawStyle->noDraw = true;


//        setDropIndicatorShown(false);

    }


    QTreeView::dragMoveEvent(event);
    event->acceptProposedAction();



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


}

Mpi3TreeViewSonglist::Mpi3TreeViewSonglist(QWidget *parent) : Mpi3TreeView(parent){
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setAlternatingRowColors(true);
    setRootIsDecorated(false);
    setHeaderHidden(false);
    setIndentation(12);

    m_drawStyle->setTVIndicatorStyle(Mpi3Style::TV_IndicatorStyleMove);
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
