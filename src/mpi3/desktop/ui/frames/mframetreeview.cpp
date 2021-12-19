#include "mpi3/desktop/ui/models/mmodelcontainers.h"
#include "mpi3/desktop/ui/models/mmodelsonglist.h"
#include "mpi3/desktop/ui/models/mmodelsonglistproxy.h"
#include "mpi3/desktop/ui/models/mmodeldrives.h"
#include "mpi3/desktop/ui/frames/mframetreeview.h"
#include "mpi3/desktop/ui/widgets/mtreeview.h"
#include "mpi3/desktop/ui/mactions.h"
#include "mpi3/core/mmedialibrary.h"
#include "mpi3/core/mmediautil.h"
#include "mpi3/core/maudioengine.h"

#include <QFileDialog>
#include <QGridLayout>
#include <QHeaderView>
#include <QMenu>


#include <QDebug>


MFrameTreeView::MFrameTreeView(QWidget *parent) : MFrame(parent)
{

}

void MFrameTreeView::itemDetails()
{
    qDebug();
}
void MFrameTreeView::editItem()
{
    qDebug();
}
void MFrameTreeView::cutItems()
{
    qDebug();
}
void MFrameTreeView::copyItems()
{
    qDebug();
}
void MFrameTreeView::pasteItems()
{
    qDebug();
}
void MFrameTreeView::deleteItems()
{
    qDebug();
}


MFrameContainers::MFrameContainers(QWidget *parent) : MFrameTreeView(parent)
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


MFrameSonglist::MFrameSonglist(QWidget *parent) : MFrameTreeView(parent)
{
    m_treeSonglist = new MTreeSonglist(this);
    m_modelSonglist = new MModelSonglist(this);
    m_modelSonglistProxy = new MModelSonglistProxy(this);
    m_modelSonglistProxy->setSourceModel(m_modelSonglist);
    m_treeSonglist->setModel(m_modelSonglistProxy);
    m_modelSonglistProxy->setSortCaseSensitivity(Qt::CaseInsensitive);

    QGridLayout *gridMain = new QGridLayout();
    gridMain->addWidget(m_treeSonglist, 0, 0, 1, 1);
    gridMain->setColumnStretch(0, 1);
    gridMain->setRowStretch(0, 1);
    gridMain->setContentsMargins(0, 0, 0, 0);
    setLayout(gridMain);

    connect(m_treeSonglist, &QTreeView::customContextMenuRequested, this, &MFrameSonglist::contextMenuTreeview);
    connect(m_treeSonglist->header(), &QHeaderView::customContextMenuRequested, this, &MFrameSonglist::contextMenuHeader);

    connect(m_treeSonglist, &MTreeSonglist::moveSelected, this, &MFrameSonglist::moveSelected);
    connect(m_treeSonglist, &MTreeView::doubleClicked, this, [this](){playItemSelected();});
}

//void MFrameSonglist::itemDetails()
//{
//    qDebug();
//}
//void MFrameSonglist::editItem()
//{
//    qDebug();
//}
//void MFrameSonglist::cutItems()
//{
//    qDebug();
//}
//void MFrameSonglist::copyItems()
//{
//    qDebug();
//}
//void MFrameSonglist::pasteItems()
//{
//    qDebug();
//}
void MFrameSonglist::deleteItems()
{
    QModelIndexList removeIndexes = tree()->selectionModel()->selectedRows(0);

    QStringList pidList;
    for(QModelIndex idxProxy : removeIndexes) {
        QModelIndex idxSource = modelProxy()->mapToSource(idxProxy);
        pidList.append(model()->pidFromIndex(idxSource));
    }

    pidList.removeDuplicates();
    for(const QString &pid : pidList) {
        m_mediaLibrary->remove(pid);
    }
}

void MFrameSonglist::importSongs()
{
    MPlaylist *parentPlaylist = m_mediaLibrary->getPlaylist(model()->pidCurrentSonglist());

    QString title = "Add Media Files";
    QString files = "All Files (*.*)";
    QStringList paths = QFileDialog::getOpenFileNames(nullptr, title, MActions::pathDesktop(), files);

    QList<QUrl> urls;
    for(const QString &p : paths) {
        urls.append(QUrl(p));
    }

    if(Mpi3::Core::validMediaFiles(urls)) {
        QStringList pidList;
        for(const QString &p : paths) {
            MSongInfo songInfo;
            if (songInfo.load(p)) {
                MSong *s = m_mediaLibrary->newSong(songInfo.songInfoMap());
                pidList.append(s->pid());
            }
        }

        if(parentPlaylist) {
            QStringList pidListCombined = parentPlaylist->songsPidList();
            pidListCombined.append(pidList);
            m_mediaLibrary->edit(parentPlaylist, "songs", pidListCombined);
        }
    }
}

void MFrameSonglist::playItemSelected()
{
    if(tree()->selectionModel()->selectedRows().size() == 1) {
        QModelIndex idxCurrent = tree()->currentIndex();
        QModelIndex idxSource = modelProxy()->mapToSource(idxCurrent);
        QString pid = model()->pidFromIndex(idxSource);
        m_pidCurrentPlayingSong = pid;
        emit currentPlayingSongChanged(pid);
    }
}
void MFrameSonglist::playItemShift(const int &shift)
{
    QModelIndex idxCurrent = model()->pidToIndex(m_pidCurrentPlayingSong);
    QModelIndex idxProxy = modelProxy()->mapFromSource(idxCurrent);

    int rowCurrent = idxProxy.row();
    int rowDestination = rowCurrent + shift;
    if (rowDestination >= 0 && rowDestination < model()->rowCount()) {
        QModelIndex nextIndex = modelProxy()->index(rowDestination, 0);
        if (nextIndex.isValid()) {
            tree()->selectionModel()->setCurrentIndex(nextIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            playItemSelected();
        }
    }
}
void MFrameSonglist::playItemNext()
{
    playItemShift(1);
}
void MFrameSonglist::playItemPrev()
{
    playItemShift(-1);
}

void MFrameSonglist::addItemsTo()
{
    qDebug();
}
void MFrameSonglist::removeItemsFrom()
{
    qDebug();
}
void MFrameSonglist::openItemFileLocation()
{
    if(tree()->selectionModel()->selectedRows().size() == 1) {
        QModelIndex idxCurrent = tree()->currentIndex();
        QModelIndex idxSource = modelProxy()->mapToSource(idxCurrent);
        QString pid = model()->pidFromIndex(idxSource);
        MSong *s = m_mediaLibrary->getSong(pid);
        MActions::openFileLocation(s->path());
    }
}

MTreeSonglist *MFrameSonglist::tree()
{
    return m_treeSonglist;
}
MModelSonglist *MFrameSonglist::model()
{
    return m_modelSonglist;
}
MModelSonglistProxy *MFrameSonglist::modelProxy()
{
    return m_modelSonglistProxy;
}

void MFrameSonglist::setLibrary(MMediaLibrary *library)
{
    m_mediaLibrary = library;
    model()->setLibrary(m_mediaLibrary);
    m_pidCurrentPlayingSong = QString();
}
void MFrameSonglist::setPlaylist(MPlaylist *playlist)
{
    if(!playlist) {
        model()->setSongList(m_mediaLibrary->songs(), m_mediaLibrary->pid());
    }
    else {
        model()->setSongList(playlist->songs(), playlist->pid());
    }
    m_pidCurrentPlayingSong = QString();
}

void MFrameSonglist::setLayoutSettings(MTreeViewLayoutSettings *settings)
{
    if(m_layoutSettings) {
        saveLayoutSettings();
    }

    m_layoutSettings = settings;
    if(!m_layoutSettings->columnCount()) {
        m_layoutSettings->setDefaults(tree());
    }

    m_layoutSettings->applyValues(tree(), modelProxy());
}
void MFrameSonglist::saveLayoutSettings()
{
    if(m_layoutSettings) {
        m_layoutSettings->setValues(tree(), modelProxy());
    }
}

void MFrameSonglist::moveSelected(int row)
{
    MPlaylist *playlist = m_mediaLibrary->getPlaylist(model()->pidCurrentSonglist());

    bool isSortedAscending = modelProxy()->sortOrder() == Qt::AscendingOrder;
    bool isSortedRoot = modelProxy()->sortColumn() == 0;

    if(playlist && (isSortedAscending && isSortedRoot)) {

        QModelIndexList moveIndexes = tree()->selectionModel()->selectedRows(0);

        QList<int> indexList;
        for(QModelIndex idx : moveIndexes) {
            indexList.append(idx.row());
        }

        QStringList newList;
        for(int i = 0; i < playlist->songsPidList().size(); i++) {

            if(i == row) {
                for(QModelIndex idx : moveIndexes) {
                    newList.append(model()->pidFromIndex(idx));
                }
            }

            if(!indexList.contains(i)) {
                newList.append(playlist->songsPidList().at(i));
            }
        }

        if(row == model()->rowCount()) {
            for(QModelIndex idx : moveIndexes) {
                newList.append(model()->pidFromIndex(idx));
            }
        }

        m_mediaLibrary->edit(playlist, "songs", newList);
    }
}

void MFrameSonglist::contextMenuHeader(const QPoint &point)
{
    QMenu *menuContext = new QMenu(this);

    QAction *actAutoFitOne = new QAction(menuContext);
    QAction *actAutoFitAll = new QAction(menuContext);

    actAutoFitOne->setText("Auto Fit Column");
    actAutoFitAll->setText("Auto All Fit Columns");

    int col = tree()->indexAt(point).column();
    connect(actAutoFitOne, &QAction::triggered, this, [=](){tree()->resizeColumnToContents(col);});
    connect(actAutoFitAll, &QAction::triggered, this, [=](){tree()->autoFitColumns();});

    menuContext->addAction(actAutoFitOne);
    menuContext->addAction(actAutoFitAll);
    menuContext->addSeparator();

    for(int i = 0; i < model()->columnCount(); i++) {
        QAction *act = new QAction(menuContext);
        act->setText(model()->headerData(i, Qt::Horizontal).toString());

        bool isHidden = tree()->isColumnHidden(i);
        act->setCheckable(true);
        act->setChecked(!isHidden);

        connect(act, &QAction::triggered, this, [=](){tree()->setColumnHidden(i, !isHidden);});
        menuContext->addAction(act);
    }

    menuContext->exec(tree()->mapToGlobal(point));
    delete menuContext;
}
void MFrameSonglist::contextMenuTreeview(const QPoint &point)
{
    QMenu *menuContext = new QMenu(this);

    QAction *actPlayItem = new QAction(menuContext);
    QAction *actEditItem = new QAction(menuContext);
    QAction *actItemDetails = new QAction(menuContext);
    QMenu *menuAddItemsTo = new QMenu(menuContext);
    QAction *actCutItems = new QAction(menuContext);
    QAction *actCopyItems = new QAction(menuContext);
    QAction *actPasteItems = new QAction(menuContext);
    QAction *actOpenItem = new QAction(menuContext);
    QAction *actSelectAll = new QAction(menuContext);
    QAction *actSelectNone = new QAction(menuContext);
    QAction *actImportSongs = new QAction(menuContext);
    QAction *actRemoveItem = new QAction(menuContext);
    QAction *actDeleteItem = new QAction(menuContext);

    actPlayItem->setText("Play");
    actEditItem->setText("Edit");
    actItemDetails->setText("Details");
    menuAddItemsTo->setTitle("Add to Playlist...");
    actCutItems->setText("Cut");
    actCopyItems->setText("Copy");
    actPasteItems->setText("Paste");
    actOpenItem->setText("Open File Location");
    actSelectAll->setText("Select All");
    actSelectNone->setText("Select None");
    actImportSongs->setText("Import Songs");
    actRemoveItem->setText("Remove from Playlist");
    actDeleteItem->setText("Delete from Library");

    menuContext->addAction(actPlayItem);
    menuContext->addAction(actEditItem);
    menuContext->addAction(actItemDetails);
    menuContext->addMenu(menuAddItemsTo);
    menuContext->addSeparator();
    menuContext->addAction(actCutItems);
    menuContext->addAction(actCopyItems);
    menuContext->addAction(actPasteItems);
    menuContext->addAction(actOpenItem);
    menuContext->addSeparator();
    menuContext->addAction(actSelectAll);
    menuContext->addAction(actSelectNone);
    menuContext->addSeparator();
    menuContext->addAction(actImportSongs);
    menuContext->addSeparator();
    menuContext->addAction(actRemoveItem);
    menuContext->addAction(actDeleteItem);

    QModelIndex idx = tree()->indexAt(point);

    if(!idx.isValid()) {
        actPlayItem->setDisabled(true);
        actEditItem->setDisabled(true);
        actItemDetails->setDisabled(true);
        menuAddItemsTo->setDisabled(true);
        actCopyItems->setDisabled(true);
        actCutItems->setDisabled(true);
        actOpenItem->setDisabled(true);
        actRemoveItem->setDisabled(true);
        actDeleteItem->setDisabled(true);
    }

    if(tree()->selectionModel()->selectedRows().size() > 1) {
        actPlayItem->setDisabled(true);
        actEditItem->setDisabled(true);
        actItemDetails->setDisabled(true);
        actOpenItem->setDisabled(true);
    }

    if(!m_mediaLibrary->getPlaylist(model()->pidCurrentSonglist())) {
        actRemoveItem->setDisabled(true);
    }

    connect(actPlayItem, &QAction::triggered, this, [=](){playItemSelected();});
    connect(actEditItem, &QAction::triggered, this, [=](){editItem();});
    connect(actItemDetails, &QAction::triggered, this, [=](){itemDetails();});

    connect(actCutItems, &QAction::triggered, this, [this](){cutItems();});
    connect(actCopyItems, &QAction::triggered, this, [this](){copyItems();});
    connect(actPasteItems, &QAction::triggered, this, [this](){pasteItems();});
    connect(actOpenItem, &QAction::triggered, this, [this](){openItemFileLocation();});

    connect(actSelectAll, &QAction::triggered, this, [this](){tree()->selectAll();});
    connect(actSelectNone, &QAction::triggered, this, [this](){tree()->selectionModel()->clear();});

    connect(actImportSongs, &QAction::triggered, this, [this](){importSongs();});

    connect(actRemoveItem, &QAction::triggered, this, [this](){removeItemsFrom();});
    connect(actDeleteItem, &QAction::triggered, this, [this](){deleteItems();});

    menuContext->exec(tree()->mapToGlobal(point));
    delete menuContext;
}



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

