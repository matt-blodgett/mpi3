#include "libview.h"
#include "util/uistyle.h"

#include <QHeaderView>

#include <QDropEvent>
#include <QMimeData>


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

    m_drawStyle->setTVIndicatorStyle(Mpi3Style::TV_IndicatorStyleDrop);
}

void Mpi3TreeViewContainers::dragEnterEvent(QDragEnterEvent *event){
    QTreeView::dragEnterEvent(event);

    if(event->mimeData()->hasUrls()){
        QMimeData *urlData = new QMimeData();
        urlData->setUrls(event->mimeData()->urls());

        QModelIndex dropIndex = indexAt(event->pos());
        QModelIndex parentIndex = dropIndex.parent();

        int row = dropIndex.row();
        int col = dropIndex.column();

        if(model()->canDropMimeData(urlData, Qt::CopyAction, row, col, parentIndex)){
            event->acceptProposedAction();
        }
    }
}
void Mpi3TreeViewContainers::dragMoveEvent(QDragMoveEvent *event){
    Qt::DropAction action = event->source() == this ? Qt::MoveAction : Qt::CopyAction;

    QModelIndex dropIndex = indexAt(event->pos());
    QModelIndex parentIndex = dropIndex.parent();

    int row = dropIndex.row();
    int col = dropIndex.column();

    if(model()->canDropMimeData(event->mimeData(), action, row, col, parentIndex)){
        QTreeView::dragMoveEvent(event);
    }

    event->acceptProposedAction();
}
void Mpi3TreeViewContainers::dragLeaveEvent(QDragLeaveEvent *event){
    setDragDropMode(QAbstractItemView::DragDrop);
    QTreeView::dragLeaveEvent(event);
}
void Mpi3TreeViewContainers::dropEvent(QDropEvent *event){
    Qt::DropAction action = event->source() == this ? Qt::MoveAction : Qt::CopyAction;

    QModelIndex dropIndex = indexAt(event->pos());
    QModelIndex parentIndex = dropIndex.parent();

    int row = dropIndex.row();
    int col = dropIndex.column();

    if(model()->canDropMimeData(event->mimeData(), action, row, col, parentIndex)){

        QVector<QVariant> currentData;
        for(int i = 0; i < model()->rowCount(dropIndex); i++){
            currentData.append(model()->data(model()->index(i, 0, dropIndex)));
        }

        if(model()->dropMimeData(event->mimeData(), action, row, col, parentIndex)){
            expand(dropIndex);

            if(action == Qt::MoveAction){
                for(int i = 0; i < model()->rowCount(dropIndex); i++){
                    QModelIndex childIndex = model()->index(i, 0, dropIndex);
                    QVariant indexData = model()->data(childIndex);

                    if(!currentData.contains(indexData)){
                        selectionModel()->select(childIndex, QItemSelectionModel::ClearAndSelect);
                        expand(childIndex);
                    }
                }
            }

            return;
        }
    }
}


Mpi3TreeViewSonglist::Mpi3TreeViewSonglist(QWidget *parent) : Mpi3TreeView(parent){
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setAlternatingRowColors(true);
    setRootIsDecorated(false);
    setHeaderHidden(false);

    m_drawStyle->setTVIndicatorStyle(Mpi3Style::TV_IndicatorStyleMove);
}

void Mpi3TreeViewSonglist::dropEvent(QDropEvent *event){
    Qt::DropAction action = event->source() == this ? Qt::MoveAction : Qt::CopyAction;

    QModelIndex dropIndex = indexAt(event->pos());
    QModelIndex parentIndex = dropIndex.parent();

    int row = dropIndex.row();
    int col = dropIndex.column();

    if(model()->canDropMimeData(event->mimeData(), action, row, col, parentIndex)){
        model()->dropMimeData(event->mimeData(), action, row, col, parentIndex);
    }

    event->acceptProposedAction();
    event->accept();

    QTreeView::dropEvent(event);
}
