#include "mpi3/desktop/ui/frames/mframecontainers.h"
#include "mpi3/desktop/ui/trees/mtreecontainers.h"
#include "mpi3/desktop/ui/models/mmodelcontainers.h"
#include "mpi3/core/mmedialibrary.h"

#include <QGridLayout>
#include <QMenu>


#include <QDebug>


MFrameContainers::MFrameContainers(QWidget *parent) : MFrame(parent)
{
    m_treeContainers = new MTreeContainers(this);
    m_modelContainers = new MModelContainers(this);
    m_treeContainers->setModel(m_modelContainers);

    QGridLayout *gridMain = new QGridLayout();
    gridMain->addWidget(m_treeContainers, 0, 0, 1, 1);
    gridMain->setColumnStretch(0, 1);
    gridMain->setRowStretch(0, 1);
    gridMain->setVerticalSpacing(0);
    gridMain->setHorizontalSpacing(0);
    gridMain->setContentsMargins(0, 0, 0, 0);
    setLayout(gridMain);

    connect(m_treeContainers, &QTreeView::customContextMenuRequested, this, &MFrameContainers::contextMenuTreeview);
    connect(m_treeContainers->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this](){selectContainer();});

    connect(m_modelContainers, &QAbstractItemModel::rowsMoved, this, &MFrameContainers::containerMoved);
    connect(m_modelContainers, &QAbstractItemModel::rowsInserted, this, &MFrameContainers::containerInserted);
    connect(m_treeContainers, &QTreeView::collapsed, this, &MFrameContainers::containerCollapsed);
}

void MFrameContainers::iconFolderChanged()
{
    model()->iconFolder = QIcon(QPixmap(m_icnFolder));
}
void MFrameContainers::iconPlaylistChanged()
{
    model()->iconPlaylist = QIcon(QPixmap(m_icnPlaylist));
}

void MFrameContainers::itemDetails()
{
    qDebug();
}
void MFrameContainers::editItem()
{
    qDebug();
}
void MFrameContainers::cutItems()
{
    qDebug();
}
void MFrameContainers::copyItems()
{
    qDebug();
}
void MFrameContainers::pasteItems()
{
    qDebug();
}
void MFrameContainers::deleteItems()
{
    m_mediaLibrary->remove(model()->pidAt(tree()->currentIndex()));
}

void MFrameContainers::newFolder()
{
    m_mediaLibrary->newFolder(getInsertFolder());
}
void MFrameContainers::newPlaylist()
{
    m_mediaLibrary->newPlaylist(getInsertFolder());
}
void MFrameContainers::importPlaylists()
{
    qDebug();
}
void MFrameContainers::duplicatePlaylist()
{
    qDebug();
}


MTreeContainers *MFrameContainers::tree() const
{
    return m_treeContainers;
}
MModelContainers *MFrameContainers::model() const
{
    return m_modelContainers;
}

void MFrameContainers::setLibrary(MMediaLibrary *library)
{
    m_mediaLibrary = library;
    model()->setLibrary(m_mediaLibrary);
}

MFolder *MFrameContainers::getInsertFolder() const
{
    QModelIndex idx = tree()->selectionModel()->currentIndex();

    if(!idx.isValid()) {
        return nullptr;
    }

    QString pid = model()->pidAt(idx);
    MFolder *insertFolder = m_mediaLibrary->getFolder(pid);

    if(!insertFolder) {
        MPlaylist *insertPlaylist = m_mediaLibrary->getPlaylist(pid);
        if(insertPlaylist) {
            insertFolder = insertPlaylist->parentFolder();
        }
    }

    return insertFolder;
}
void MFrameContainers::selectContainer()
{
    QModelIndex idx = tree()->selectionModel()->currentIndex();
    QString pid = model()->pidAt(idx);
    MContainer *container = m_mediaLibrary->getContainer(pid);

    if(container) {
        emit containerSelected(container);
    }
}

void MFrameContainers::containerMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destinationIndex, int row)
{
    Q_UNUSED(parent)
    Q_UNUSED(start)
    Q_UNUSED(end)

    QModelIndex idx = model()->index(row, 0, destinationIndex);
    tree()->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::ClearAndSelect);
    tree()->expand(destinationIndex);
    tree()->expand(idx);
}
void MFrameContainers::containerInserted(const QModelIndex &parent, int first, int last)
{
    if(first == last) {
        QModelIndex idx = model()->index(first, 0, parent);
        tree()->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::ClearAndSelect);
        tree()->expand(parent);
    }
}
void MFrameContainers::containerCollapsed(const QModelIndex &index)
{
    QModelIndexList indexes = model()->match(model()->index(0, 0, index), Qt::DisplayRole, "*", -1, Qt::MatchWildcard | Qt::MatchRecursive);
    for(QModelIndex idx : indexes) {
        tree()->collapse(idx);
    }
}

void MFrameContainers::contextMenuTreeview(const QPoint &point)
{
    QMenu *menuContext = new QMenu(this);

    QAction *actExpandItem = new QAction(menuContext);
    QAction *actCollapseItem = new QAction(menuContext);
    QAction *actExpandAll = new QAction(menuContext);
    QAction *actCollapseAll = new QAction(menuContext);
    QAction *actNewFolder = new QAction(menuContext);
    QAction *actNewPlaylist = new QAction(menuContext);
    QAction *actImportPlaylists = new QAction(menuContext);
    QAction *actEditItem = new QAction(menuContext);
    QAction *actItemDetails = new QAction(menuContext);
    QAction *actDuplicatePlaylist = new QAction(menuContext);
    QAction *actDeleteItems = new QAction(menuContext);

    actExpandItem->setText("Expand");
    actCollapseItem->setText("Collapse");
    actExpandAll->setText("Expand All");
    actCollapseAll->setText("Collapse All");
    actNewFolder->setText("New Folder");
    actNewPlaylist->setText("New Playlist");
    actImportPlaylists->setText("Import Playlists");
    actEditItem->setText("Edit");
    actItemDetails->setText("Details...");
    actDuplicatePlaylist->setText("Duplicate");
    actDeleteItems->setText("Delete from Library");

    menuContext->addAction(actExpandItem);
    menuContext->addAction(actCollapseItem);
    menuContext->addSeparator();
    menuContext->addAction(actExpandAll);
    menuContext->addAction(actCollapseAll);
    menuContext->addSeparator();
    menuContext->addAction(actNewFolder);
    menuContext->addAction(actNewPlaylist);
    menuContext->addSeparator();
    menuContext->addAction(actImportPlaylists);
    menuContext->addSeparator();
    menuContext->addAction(actDuplicatePlaylist);
    menuContext->addAction(actEditItem);
    menuContext->addAction(actItemDetails);
    menuContext->addSeparator();
    menuContext->addAction(actDeleteItems);

    QModelIndex idx_at = tree()->indexAt(point);
//    QModelIndex idx_last = tree->selectionModel()->currentIndex();

    tree()->selectionModel()->setCurrentIndex(idx_at, QItemSelectionModel::ClearAndSelect);

    if(model()->itemIsPlaylist(idx_at)) {
        actExpandItem->setDisabled(true);
        actCollapseItem->setDisabled(true);
    }
    else if(model()->itemIsFolder(idx_at)) {
        actDuplicatePlaylist->setDisabled(true);
    }
    else {
        actExpandItem->setDisabled(true);
        actCollapseItem->setDisabled(true);
        actDuplicatePlaylist->setDisabled(true);
        actEditItem->setDisabled(true);
        actItemDetails->setDisabled(true);
        actDeleteItems->setDisabled(true);
    }

    connect(actExpandItem, &QAction::triggered, this, [this](){tree()->expand(tree()->currentIndex());});
    connect(actCollapseItem, &QAction::triggered, this, [this](){tree()->collapse(tree()->currentIndex());});
    connect(actExpandAll, &QAction::triggered, this, [this](){tree()->expandAll();});
    connect(actCollapseAll, &QAction::triggered, this, [this](){tree()->collapseAll();});

    connect(actNewPlaylist, &QAction::triggered, this, &MFrameContainers::newPlaylist);
    connect(actNewFolder, &QAction::triggered, this, &MFrameContainers::newFolder);

    connect(actImportPlaylists, &QAction::triggered, this, &MFrameContainers::importPlaylists);

    connect(actDuplicatePlaylist, &QAction::triggered, this, &MFrameContainers::duplicatePlaylist);
    connect(actEditItem, &QAction::triggered, this, &MFrameContainers::editItem);
    connect(actItemDetails, &QAction::triggered, this, &MFrameContainers::itemDetails);

    connect(actDeleteItems, &QAction::triggered, this, [this](){deleteItems();});

    menuContext->exec(tree()->mapToGlobal(point));
    delete menuContext;

//    if(idx_last.isValid()) {
//        tree()->selectionModel()->setCurrentIndex(idx_last, QItemSelectionModel::ClearAndSelect);
//    }
}
