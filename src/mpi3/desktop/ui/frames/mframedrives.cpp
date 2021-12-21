#include "mpi3/desktop/ui/frames/mframedrives.h"
#include "mpi3/desktop/ui/trees/mtreedrives.h"
#include "mpi3/desktop/ui/models/mmodeldrives.h"
#include "mpi3/desktop/ui/mactions.h"
#include "mpi3/core/mmediautil.h"

#include <QGridLayout>
#include <QMenu>


#include <QDebug>


MFrameDrives::MFrameDrives(QWidget *parent) : MFrame(parent)
{
    m_treeDrives = new MTreeDrives(this);
    m_modelDrives = new MModelDrives(this);

    m_treeDrives->setModel(m_modelDrives);
    m_treeDrives->setSelectionMode(QAbstractItemView::SingleSelection);
    m_treeDrives->setAlternatingRowColors(false);
    m_treeDrives->setRootIsDecorated(true);
    m_treeDrives->setHeaderHidden(false);
    m_treeDrives->setSortingEnabled(false);
    m_treeDrives->setExpandsOnDoubleClick(false);
    m_treeDrives->setItemsExpandable(false);

    QGridLayout *gridMain = new QGridLayout();
    gridMain->addWidget(m_treeDrives, 0, 0, 1, 1);
    gridMain->setColumnStretch(0, 1);
    gridMain->setRowStretch(0, 1);
    gridMain->setVerticalSpacing(0);
    gridMain->setHorizontalSpacing(0);
    gridMain->setContentsMargins(0, 0, 0, 0);
    setLayout(gridMain);

    connect(m_treeDrives, &QTreeView::customContextMenuRequested, this, &MFrameDrives::contextMenuTreeview);
    connect(m_treeDrives->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MFrameDrives::selectionChanged);
}

void MFrameDrives::selectionChanged()
{
    if(tree()->selectionModel()->selectedRows().length() == 1) {
        QStorageInfo info = m_modelDrives->storageInfoAt(m_treeDrives->currentIndex());
        emit selectedDriveChanged(info);
    }
}

MTreeDrives *MFrameDrives::tree() const
{
    return m_treeDrives;
}
MModelDrives *MFrameDrives::model() const
{
    return m_modelDrives;
}

void MFrameDrives::openItemFileLocation()
{
    if(tree()->selectionModel()->selectedRows().size() == 1) {
        QStorageInfo info = m_modelDrives->storageInfoAt(m_treeDrives->currentIndex());
        MActions::openFileLocation(info.rootPath());
    }
}

void MFrameDrives::contextMenuTreeview(const QPoint &point)
{
    QMenu *menuContext = new QMenu(this);

    QAction *actOpenItem = new QAction(menuContext);

    actOpenItem->setText("Open File Location");

    menuContext->addAction(actOpenItem);

    connect(actOpenItem, &QAction::triggered, this, &MFrameDrives::openItemFileLocation);

    menuContext->exec(tree()->mapToGlobal(point));
    delete menuContext;
}

