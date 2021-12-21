#include "mpi3/desktop/ui/trees/mtreesonglist.h"
#include "mpi3/desktop/ui/mstyle.h"

#include <QDropEvent>


#include <QDebug>


#define MPI3_TREESTYLE_OUTLINE \
    "QTreeView {border: 1px solid #FFFFFF;} "\
    "QTreeView::item:hover "\
    "{border-top: 1px solid #FFFFFF; "\
    "border-bottom: 1px solid #FFFFFF;}"
//    "{border-top: 1px solid transparent; "
//    "border-bottom: 1px solid transparent;}"

#define MPI3_TREESTYLE_NORMAL \
    "QTreeView {border-top: 1px solid #696969; "\
    "border-bottom: 1px solid #696969;}"\
    "QTreeView::item:hover "\
    "{border-top: 1px solid #FFFFFF; "\
    "border-bottom: 1px solid #FFFFFF;}"


MTreeSonglist::MTreeSonglist(QWidget *parent) : MTree(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setAlternatingRowColors(true);
    setRootIsDecorated(false);
    setHeaderHidden(false);
    setSortingEnabled(true);
}

void MTreeSonglist::autoFitColumns()
{
    for(int i = 0; i < model()->columnCount(); i++) {
        resizeColumnToContents(i);
    }
}

void MTreeSonglist::dragEnterEvent(QDragEnterEvent *event)
{
    Qt::DropAction action = event->proposedAction();

    QModelIndex dropIndex = indexAt(event->position().toPoint());
    QModelIndex parentIndex = dropIndex.parent();

    int row = dropIndex.row();
    int col = dropIndex.column();

    if(model()->canDropMimeData(event->mimeData(), action, row, col, parentIndex)) {
        setStyleSheet(MPI3_TREESTYLE_OUTLINE);
    }

    QTreeView::dragEnterEvent(event);
}
void MTreeSonglist::dragLeaveEvent(QDragLeaveEvent *event)
{
    setStyleSheet(MPI3_TREESTYLE_NORMAL);
    QTreeView::dragLeaveEvent(event);
}
void MTreeSonglist::dropEvent(QDropEvent *event)
{
    Qt::DropAction action = event->source() == this ? Qt::MoveAction : Qt::CopyAction;

    QModelIndex dropIndex = indexAt(event->position().toPoint());
    QModelIndex parentIndex = dropIndex.parent();

    int row = dropIndex.row();
    int col = dropIndex.column();

    int rowCount = model()->rowCount();
    int colCount = model()->columnCount();

    if(row < 0) {
        row = rowCount;
    }

    if(rowCount == 0) {
        rowCount -= 1;
    }

    if(model()->canDropMimeData(event->mimeData(), action, row, col, parentIndex)) {

        if(action == Qt::MoveAction) {
            emit moveSelected(row);
        }
        else if(model()->dropMimeData(event->mimeData(), action, row, col, parentIndex)) {
            QItemSelectionModel::SelectionFlag flags = QItemSelectionModel::Select;

            selectionModel()->clear();
            int rowDiff = model()->rowCount() - rowCount;
            for(int r = row; r < row + rowDiff; r++) {
                for(int c = 0; c < colCount; c++) {
                    selectionModel()->select(model()->index(r, c), flags);
                }
            }
        }
    }

    event->acceptProposedAction();
    event->accept();

    setStyleSheet(MPI3_TREESTYLE_NORMAL);
    QTreeView::dropEvent(event);
}
