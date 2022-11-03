#include "mpi3/desktop/ui/trees/mtreedrives.h"
#include "mpi3/desktop/ui/mstyle.h"


#include <QDebug>


MTreeDrives::MTreeDrives(QWidget *parent) : QTreeView(parent)
{
    setAcceptDrops(false);
    setDragEnabled(false);
    viewport()->setAcceptDrops(false);
    setDragDropMode(QAbstractItemView::NoDragDrop);

    setExpandsOnDoubleClick(false);
    setFocusPolicy(Qt::NoFocus);
    setSelectionBehavior(QAbstractItemView::SelectRows);

//    setEditTriggers(QTreeView::SelectedClicked);
    setContextMenuPolicy(Qt::CustomContextMenu);

    m_drawStyle = new MProxyStyle(style());
    setStyle(m_drawStyle);
}
