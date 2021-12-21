#include "mpi3/desktop/ui/trees/mtree.h"
#include "mpi3/desktop/ui/mstyle.h"

#include <QHeaderView>


#include <QDebug>


MTree::MTree(QWidget *parent) : QTreeView(parent)
{
    setAcceptDrops(true);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDrop);

    setFocusPolicy(Qt::NoFocus);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    setEditTriggers(QTreeView::SelectedClicked);
    setContextMenuPolicy(Qt::CustomContextMenu);
    header()->setContextMenuPolicy(Qt::CustomContextMenu);

    m_drawStyle = new MProxyStyle(style());
    setStyle(m_drawStyle);
}
