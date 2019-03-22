#include "mframetreeview.h"
#include "mmodelcontainers.h"
#include "mmodelsonglist.h"
#include "mmedialibrary.h"
#include "mtreeview.h"
#include "mactions.h"

#include <QFileDialog>
#include <QGridLayout>
#include <QHeaderView>
#include <QMenu>


#include <QDebug>


void x_importSongs(MMediaLibrary *library, MPlaylist *parentPlaylist = nullptr)
{
    QString title = "Add Media Files";
    QString files = "All Files (*.*)";
    QStringList paths = QFileDialog::getOpenFileNames(
        nullptr, title, MActions::pathDesktop(), files);

    for(QString p : paths){
        MSong *s = library->newSong(p);
        if(parentPlaylist){
            parentPlaylist->append(s);
        }
    }
}


MFrameTreeView::MFrameTreeView(QWidget *parent) : MFrame(parent)
{

}

void MFrameTreeView::importSongs()
{

}
void MFrameTreeView::downloadSongs()
{

}
void MFrameTreeView::itemDetails()
{

}
void MFrameTreeView::editItem()
{

}
void MFrameTreeView::cutItems()
{
//    if(treeParent == m_treeSonglist) {
//        QApplication::clipboard()->setMimeData()
//    }
}
void MFrameTreeView::copyItems()
{
//    if(treeParent == m_treeSonglist) {
//        QModelIndexList selectedIndexes = m_treeSonglist->selectionModel()->selectedIndexes();
//        QApplication::clipboard()->setMimeData(m_modelSonglist->mimeData(selectedIndexes));
//    }
}
void MFrameTreeView::pasteItems()
{
//    if(treeParent == m_treeSonglist) {
//        QModelIndex currentIndex = m_treeSonglist->currentIndex();
//        const QMimeData *data = QApplication::clipboard()->mimeData(QClipboard::Clipboard);
//        m_modelSonglist->dropMimeData(
//                    data, Qt::CopyAction,
//                    currentIndex.row(),
//                    currentIndex.column(),
//                    currentIndex.parent());
//    }
}
void MFrameTreeView::deleteItems()
{
//    Q_UNUSED(treeParent)
//    if(treeParent == m_treeContainers && m_treeContainers->selectionModel()->selectedRows().size() > 0) {
//        QModelIndex index = m_treeContainers->currentIndex();

//        QString pid = m_modelContainers->getPID(index);
//        MMediaElement *element = m_mediaLibrary->getElement(pid);

//        if(element == m_modelSonglist->container()) {
//            m_panelMedia->changeView(MPanelMedia::ViewAllSongs);
//            m_modelSonglist->setContainer(m_mediaLibrary);
//        }

//        if(!element) {
//            return;
//        }
//        else if(element->type() == Mpi3::FolderElement) {
//            m_mediaLibrary->discard(static_cast<MFolder*>(element));
//        }
//        else if(element->type() == Mpi3::PlaylistElement) {
//            m_mediaLibrary->discard(static_cast<MPlaylist*>(element));
//        }
//    }
//    else if(treeParent == m_treeSonglist && m_treeSonglist->selectionModel()->selectedRows().size() > 0) {
//        QModelIndexList currentIndexes = m_treeSonglist->selectionModel()->selectedIndexes();

//        QVector<MSong*> delSongs;
//        (QModelIndex idx, currentIndexes) {
//            if(idx.column() == 0) {
//                delSongs.append(m_modelSonglist->songAt(idx));
//            }
//        }

//        (MSong *song, delSongs) {
//            m_mediaLibrary->discard(song);
//        }
//    }
}
void MFrameTreeView::duplicateItems()
{

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
    gridMain->setMargin(0);
    setLayout(gridMain);

    connect(m_treeContainers,
        &QTreeView::customContextMenuRequested,
        this, &MFrameContainers::contextMenuTreeview);

    connect(m_treeContainers->selectionModel(),
        &QItemSelectionModel::selectionChanged,
        this, [this](){selectContainer();});
}

void MFrameContainers::iconFolderChanged()
{
    m_modelContainers->iconFolder = QIcon(QPixmap(m_icnFolder));
}
void MFrameContainers::iconPlaylistChanged()
{
    m_modelContainers->iconPlaylist = QIcon(QPixmap(m_icnPlaylist));
}

void MFrameContainers::importSongs()
{
    QModelIndex idx = tree()->currentIndex();
    MPlaylist *parentPlaylist = model()->getPlaylist(idx);
    x_importSongs(library(), parentPlaylist);
}
void MFrameContainers::downloadSongs()
{
    qDebug();
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
    QModelIndex idx = tree()->currentIndex();
    MContainer *container = model()->getContainer(idx);

    if(container){
        emit containerSelected(nullptr);
        library()->remove(container);
    }
}
void MFrameContainers::duplicateItems()
{
    qDebug();
}

void MFrameContainers::newFolder()
{
    library()->newFolder(getInsertFolder());
    tree()->expand(tree()->currentIndex());
}
void MFrameContainers::newPlaylist()
{
    library()->newPlaylist(getInsertFolder());
    tree()->expand(tree()->currentIndex());
}
void MFrameContainers::importPlaylists()
{
    QString title = "Open Itunes Plist File";
    QString files = "XML Files (*.xml)";
    QString path = QFileDialog::getOpenFileName(
        this, title, MActions::pathDesktop(), files);

    if(path != "") {
        library()->importItunesPlist(path, getInsertFolder());
        tree()->expand(tree()->currentIndex());
    }
}

MFolder *MFrameContainers::getInsertFolder() {
    QModelIndex currentIndex = tree()->currentIndex();
    if(currentIndex.isValid()) {
        MContainer *c = model()->getContainer(currentIndex);
        return c->type() == Mpi3::FolderElement ? static_cast<MFolder*>(c) : c->parentFolder();
    }

    return nullptr;
}

MTreeContainers *MFrameContainers::tree()
{
    return m_treeContainers;
}
MModelContainers *MFrameContainers::model()
{
    return m_modelContainers;
}
MMediaLibrary *MFrameContainers::library()
{
    return m_modelContainers->library();
}

void MFrameContainers::selectContainer()
{
    QModelIndex idx = tree()->selectionModel()->currentIndex();
    MContainer *container = model()->getContainer(idx);
    if(container) {
        emit containerSelected(container);
    }
}
void MFrameContainers::contextMenuTreeview(const QPoint &point)
{
    QMenu *menu_context = new QMenu(this);

    QAction *act_expandItem = new QAction(menu_context);
    QAction *act_collapseItem = new QAction(menu_context);
    QAction *act_expandAll = new QAction(menu_context);
    QAction *act_collapseAll = new QAction(menu_context);
    QAction *act_newFolder = new QAction(menu_context);
    QAction *act_newPlaylist = new QAction(menu_context);
    QAction *act_importPlaylists = new QAction(menu_context);
    QAction *act_importSongs = new QAction(menu_context);
    QAction *act_downloadSongs = new QAction(menu_context);
    QAction *act_editItem = new QAction(menu_context);
    QAction *act_itemDetails = new QAction(menu_context);
    QAction *act_duplicateItems = new QAction(menu_context);
    QAction *act_deleteItems = new QAction(menu_context);

    act_expandItem->setText("Expand");
    act_collapseItem->setText("Collapse");
    act_expandAll->setText("Expand All");
    act_collapseAll->setText("Collapse All");
    act_newFolder->setText("New Folder");
    act_newPlaylist->setText("New Playlist");
    act_importPlaylists->setText("Import Playlists");
    act_importSongs->setText("Import Songs");
    act_downloadSongs->setText("Download Songs");
    act_editItem->setText("Edit");
    act_itemDetails->setText("Details...");
    act_duplicateItems->setText("Duplicate");
    act_deleteItems->setText("Delete from Library");

    menu_context->addAction(act_expandItem);
    menu_context->addAction(act_collapseItem);
    menu_context->addSeparator();
    menu_context->addAction(act_expandAll);
    menu_context->addAction(act_collapseAll);
    menu_context->addSeparator();
    menu_context->addAction(act_newFolder);
    menu_context->addAction(act_newPlaylist);
    menu_context->addSeparator();
    menu_context->addAction(act_importPlaylists);
    menu_context->addAction(act_importSongs);
    menu_context->addAction(act_downloadSongs);
    menu_context->addSeparator();
    menu_context->addAction(act_duplicateItems);
    menu_context->addAction(act_editItem);
    menu_context->addAction(act_itemDetails);
    menu_context->addSeparator();
    menu_context->addAction(act_deleteItems);

    QModelIndex idx_at = tree()->indexAt(point);
//    QModelIndex idx_last = tree()->selectionModel()->currentIndex();
    MContainer *container = model()->getContainer(idx_at);
    tree()->setCurrentIndex(idx_at);

    if(!container) {
        act_expandItem->setDisabled(true);
        act_collapseItem->setDisabled(true);
        act_duplicateItems->setDisabled(true);
        act_editItem->setDisabled(true);
        act_itemDetails->setDisabled(true);
        act_deleteItems->setDisabled(true);
    }
    else if(container->type() == Mpi3::PlaylistElement) {
        act_expandItem->setDisabled(true);
        act_collapseItem->setDisabled(true);
    }
    else if(container->type() == Mpi3::FolderElement) {
        act_importSongs->setDisabled(true);
        act_downloadSongs->setDisabled(true);
        act_duplicateItems->setDisabled(true);
    }

    connect(act_expandItem, &QAction::triggered, this, [this]() {tree()->expand(tree()->currentIndex());});
    connect(act_collapseItem, &QAction::triggered, this, [this]() {tree()->collapse(tree()->currentIndex());});
    connect(act_expandAll, &QAction::triggered, this, [this]() {tree()->expandAll();});
    connect(act_collapseAll, &QAction::triggered, this, [this]() {tree()->collapseAll();});

    connect(act_newPlaylist, &QAction::triggered, this, [this]() {newPlaylist();});
    connect(act_newFolder, &QAction::triggered, this, [this]() {newFolder();});

    connect(act_importPlaylists, &QAction::triggered, this, [this]() {importPlaylists();});
    connect(act_importSongs, &QAction::triggered, this, [this]() {importSongs();});
    connect(act_downloadSongs, &QAction::triggered, this, [this]() {downloadSongs();});

    connect(act_duplicateItems, &QAction::triggered, this, &MFrameContainers::duplicateItems);
    connect(act_editItem, &QAction::triggered, this, &MFrameContainers::editItem);
    connect(act_itemDetails, &QAction::triggered, this, &MFrameContainers::itemDetails);

    connect(act_deleteItems, &QAction::triggered, this, [this]() {deleteItems();});

    menu_context->exec(tree()->mapToGlobal(point));
    delete menu_context;

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
    gridMain->setVerticalSpacing(0);
    gridMain->setHorizontalSpacing(0);
    gridMain->setMargin(0);
    setLayout(gridMain);

    connect(m_treeSonglist,
        &QTreeView::customContextMenuRequested,
        this, &MFrameSonglist::contextMenuTreeview);

    connect(m_treeSonglist->header(),
        &QHeaderView::customContextMenuRequested,
        this, &MFrameSonglist::contextMenuHeader);
}

void MFrameSonglist::importSongs()
{
    MContainer *container = model()->container();

    MPlaylist *parentPlaylist = nullptr;
    if(container && container->type() == Mpi3::PlaylistElement) {
        parentPlaylist = static_cast<MPlaylist*>(container);
    }

    x_importSongs(library(), parentPlaylist);
}
void MFrameSonglist::downloadSongs()
{
    qDebug();
}
void MFrameSonglist::itemDetails()
{
    qDebug();
}
void MFrameSonglist::editItem()
{
    qDebug();
}
void MFrameSonglist::cutItems()
{
    qDebug();
}
void MFrameSonglist::copyItems()
{
    qDebug();
}
void MFrameSonglist::pasteItems()
{
    qDebug();
}
void MFrameSonglist::deleteItems()
{
    qDebug();
}
void MFrameSonglist::duplicateItems()
{
    qDebug();
}

void MFrameSonglist::playItem()
{
    qDebug();
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
        MSong *song = model()->getSong(tree()->currentIndex());
        if(song) {
            MActions::openFileLocation(song->path());
        }
    }
}

MTreeSonglist *MFrameSonglist::tree(){
    return m_treeSonglist;
}
MModelSonglist *MFrameSonglist::model(){
    return m_modelSonglist;
}
MMediaLibrary *MFrameSonglist::library()
{
    return m_modelSonglist->library();
}
MModelSonglistProxy *MFrameSonglist::modelProxy()
{
    return m_modelSonglistProxy;
}

void MFrameSonglist::saveTreeSettings()
{
    if(m_treeSettings){
        m_treeSettings->setValues(tree(), modelProxy());
    }
}
void MFrameSonglist::setTreeSettings(MTreeSettings *settings)
{
    if(m_treeSettings){
        saveTreeSettings();
    }

    m_treeSettings = settings;
    if(!m_treeSettings->columnCount()){
        m_treeSettings->setDefaults(tree());
    }

    m_treeSettings->applyValues(tree(), modelProxy());
}

void MFrameSonglist::contextMenuHeader(const QPoint &point)
{
    QMenu *menu_context = new QMenu(this);

    QAction *actAutoFitOne = new QAction(menu_context);
    QAction *actAutoFitAll = new QAction(menu_context);

    actAutoFitOne->setText("Auto Fit Column");
    actAutoFitAll->setText("Auto All Fit Columns");

    int col = tree()->indexAt(point).column();
    connect(actAutoFitOne, &QAction::triggered, this, [=]() {tree()->resizeColumnToContents(col);});
    connect(actAutoFitAll, &QAction::triggered, this, [=]() {tree()->autoFitColumns();});

    menu_context->addAction(actAutoFitOne);
    menu_context->addAction(actAutoFitAll);
    menu_context->addSeparator();

    for(int i = 0; i < model()->columnCount(); i++) {
        QAction *act = new QAction(menu_context);
        act->setText(model()->headerData(i, Qt::Horizontal).toString());

        act->setCheckable(true);
        act->setChecked(!tree()->isColumnHidden(i));

        connect(act, &QAction::triggered, this,
                [=]() {tree()->setColumnHidden(i, !tree()->isColumnHidden(i));});
        menu_context->addAction(act);
    }

    menu_context->exec(tree()->mapToGlobal(point));
    delete menu_context;
}
void MFrameSonglist::contextMenuTreeview(const QPoint &point)
{
    QMenu *menu_context = new QMenu(this);

    QAction *act_playItem = new QAction(menu_context);
    QAction *act_editItem = new QAction(menu_context);
    QAction *act_itemDetails = new QAction(menu_context);
    QMenu *menu_addItemsTo = new QMenu(menu_context);
    QAction *act_cutItems = new QAction(menu_context);
    QAction *act_copyItems = new QAction(menu_context);
    QAction *act_pasteItems = new QAction(menu_context);
    QAction *act_openItem = new QAction(menu_context);
    QAction *act_selectAll = new QAction(menu_context);
    QAction *act_selectNone = new QAction(menu_context);
    QAction *act_importSongs = new QAction(menu_context);
    QAction *act_downloadSongs = new QAction(menu_context);
    QAction *act_removeItem = new QAction(menu_context);
    QAction *act_deleteItem = new QAction(menu_context);

    act_playItem->setText("Play");
    act_editItem->setText("Edit");
    act_itemDetails->setText("Details");
    menu_addItemsTo->setTitle("Add to Playlist...");
    act_cutItems->setText("Cut");
    act_copyItems->setText("Copy");
    act_pasteItems->setText("Paste");
    act_openItem->setText("Open File Location");
    act_selectAll->setText("Select All");
    act_selectNone->setText("Select None");
    act_importSongs->setText("Import Songs");
    act_downloadSongs->setText("Download Songs");
    act_removeItem->setText("Remove from Playlist");
    act_deleteItem->setText("Delete from Library");

    menu_context->addAction(act_playItem);
    menu_context->addAction(act_editItem);
    menu_context->addAction(act_itemDetails);
    menu_context->addMenu(menu_addItemsTo);
    menu_context->addSeparator();
    menu_context->addAction(act_cutItems);
    menu_context->addAction(act_copyItems);
    menu_context->addAction(act_pasteItems);
    menu_context->addAction(act_openItem);
    menu_context->addSeparator();
    menu_context->addAction(act_selectAll);
    menu_context->addAction(act_selectNone);
    menu_context->addSeparator();
    menu_context->addAction(act_importSongs);
    menu_context->addAction(act_downloadSongs);
    menu_context->addSeparator();
    menu_context->addAction(act_removeItem);
    menu_context->addAction(act_deleteItem);

    QModelIndex idx = tree()->indexAt(point);
    if(!idx.isValid()) {
        act_playItem->setDisabled(true);
        act_editItem->setDisabled(true);
        act_itemDetails->setDisabled(true);
        menu_addItemsTo->setDisabled(true);
        act_copyItems->setDisabled(true);
        act_cutItems->setDisabled(true);
        act_openItem->setDisabled(true);
        act_removeItem->setDisabled(true);
        act_deleteItem->setDisabled(true);
    }

    if(tree()->selectionModel()->selectedRows().size() > 1) {
        act_playItem->setDisabled(true);
        act_editItem->setDisabled(true);
        act_itemDetails->setDisabled(true);
        act_openItem->setDisabled(true);
    }

//    if(container()->type() != Mpi3::PlaylistElement) {
//        act_removeItem->setDisabled(true);
//    }
//    if(!model()->flags(idx).testFlag(Qt::ItemIsEditable)) {
//        act_editItem->setDisabled(true);
//    }
//    if(container()->type() != Mpi3::PlaylistElement) {
//        act_removeItem->setDisabled(true);
//    }
//    if(!model()->flags(idx).testFlag(Qt::ItemIsEditable)) {
//        act_editItem->setDisabled(true);
//    }

    connect(act_playItem, &QAction::triggered, this, [=]() {playItem();});
    connect(act_editItem, &QAction::triggered, this, [=]() {editItem();});
    connect(act_itemDetails, &QAction::triggered, this, [=]() {itemDetails();});

    connect(act_cutItems, &QAction::triggered, this, [this]() {cutItems();});
    connect(act_copyItems, &QAction::triggered, this, [this]() {copyItems();});
    connect(act_pasteItems, &QAction::triggered, this, [this]() {pasteItems();});
    connect(act_openItem, &QAction::triggered, this, [this]() {openItemFileLocation();});

    connect(act_selectAll, &QAction::triggered, this, [this]() {tree()->selectAll();});
    connect(act_selectNone, &QAction::triggered, this, [this]() {tree()->selectionModel()->clear();});

    connect(act_importSongs, &QAction::triggered, this, [this]() {importSongs();});
    connect(act_downloadSongs, &QAction::triggered, this, [this]() {downloadSongs();});

    connect(act_removeItem, &QAction::triggered, this, [this]() {removeItemsFrom();});
    connect(act_deleteItem, &QAction::triggered, this, [this]() {deleteItems();});

    menu_context->exec(tree()->mapToGlobal(point));
    delete menu_context;
}
