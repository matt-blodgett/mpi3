#include "mpi3/desktop/ui/trees/mtreecontainers.h"

#include <QDropEvent>


#include <QDebug>


MTreeContainers::MTreeContainers(QWidget *parent) : MTree(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setAlternatingRowColors(false);
    setRootIsDecorated(true);
    setHeaderHidden(true);
    setSortingEnabled(false);
}

void MTreeContainers::dropEvent(QDropEvent *event)
{
    Qt::DropAction action = event->source() == this ? Qt::MoveAction : Qt::CopyAction;

    QModelIndex dropIndex = indexAt(event->position().toPoint());
    QModelIndex parentIndex = dropIndex.parent();

    int row = dropIndex.row();
    int col = dropIndex.column();

    if(model()->canDropMimeData(event->mimeData(), action, row, col, parentIndex)) {

        if(model()->dropMimeData(event->mimeData(), action, row, col, parentIndex)) {

            selectionModel()->setCurrentIndex(model()->index(row, col, parentIndex), QItemSelectionModel::ClearAndSelect);
//            expand(dropIndex);

//            if(action == Qt::MoveAction) {

//                for(int i = 0; i < model()->rowCount(dropIndex); i++) {
//                    QModelIndex childIndex = model()->index(i, 0, dropIndex);
//                    QVariant indexData = model()->data(childIndex);
//                    selectionModel()->select(childIndex, QItemSelectionModel::ClearAndSelect);
//                    expand(childIndex);
//                }
//            }
        }
    }
}

