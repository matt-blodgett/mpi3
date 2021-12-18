#include "mpi3/desktop/ui/models/mmodelcontainers.h"
#include "mpi3/desktop/ui/models/mmodelcontainersitem.h"
#include "mpi3/core/mmedialibrary.h"
#include "mpi3/core/mmediautil.h"
#include "mpi3/core/maudioengine.h"

#include <QMimeData>
#include <QUrl>


#include <QDebug>


typedef MModelContainersItem MModelItem;


static void populateItem(MModelItem *item, MContainer *c)
{
    item->setPID(c->pid());
    item->setLabel(c->name());

    if(c->type() == Mpi3::FolderElement){
        item->setType(MModelItem::FolderItem);
    }
    else if(c->type() == Mpi3::PlaylistElement){
        item->setType(MModelItem::PlaylistItem);
    }
}
static MFolderList allChildFolders(MFolder *parentFolder)
{
    MFolderList folders;
    for(MFolder *childFolder : parentFolder->childFolders()){
        folders.append(childFolder);
        for(MFolder *f : allChildFolders(childFolder)){
            folders.append(f);
        }
    }

    return folders;
}


MModelContainers::MModelContainers(QObject *parent) : QAbstractItemModel(parent)
{
    m_rootItem = new MModelItem();
    m_rootItem->setLabel("Playlists");
}
MModelContainers::~MModelContainers()
{
    delete m_rootItem;
}

Qt::ItemFlags MModelContainers::flags(const QModelIndex &index) const
{
    Qt::ItemFlags idxFlags = Qt::ItemIsDropEnabled;

    if(index.isValid()) {
        idxFlags |= Qt::ItemIsEnabled;
        idxFlags |= Qt::ItemIsEditable;
        idxFlags |= Qt::ItemIsSelectable;
        idxFlags |= Qt::ItemIsDragEnabled;
    }

    return idxFlags;
}
Qt::DropActions MModelContainers::supportedDragActions() const
{
    return Qt::MoveAction;
}
Qt::DropActions MModelContainers::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList MModelContainers::mimeTypes() const
{
    QStringList mTypes;

    mTypes << MPI3_MIME_TYPE_PIDS_CONTAINERS;

    return mTypes;
}
QMimeData *MModelContainers::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mData = new QMimeData();

    if(indexes.size() == 1) {
        QByteArray pidBytes;
        pidBytes.append(pidAt(indexes.at(0)).toStdString().c_str());
        mData->setData(MPI3_MIME_TYPE_PIDS_CONTAINERS, pidBytes);
    }

    return mData;
}

bool MModelContainers::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    bool dataIsSonglist = data->hasFormat(MPI3_MIME_TYPE_PIDS_SONGS);
    bool dataIsContainer = data->hasFormat(MPI3_MIME_TYPE_PIDS_CONTAINERS);
    bool dataIsValidMediaFiles = data->hasUrls() ? Mpi3::Core::validMediaFiles(data->urls()) : false;
    bool actionIsCopyAction = action == Qt::CopyAction;
    bool actionIsMoveAction = action == Qt::MoveAction;

    QModelIndex targetIndex = index(row, column, parent);
    MContainer *targetContainer = m_mediaLibrary->getContainer(pidAt(targetIndex));

    if(!targetContainer) {
        return dataIsContainer || dataIsValidMediaFiles;
    }
    else if(targetContainer->type() == Mpi3::PlaylistElement) {
        return actionIsCopyAction && (dataIsSonglist || dataIsValidMediaFiles);
    }
    else if(targetContainer->type() == Mpi3::FolderElement) {

        if(actionIsMoveAction && dataIsContainer){
            QString pid = data->data(MPI3_MIME_TYPE_PIDS_CONTAINERS);

            MFolder *dropFolder = m_mediaLibrary->getFolder(pid);
            MPlaylist *dropPlaylist = m_mediaLibrary->getPlaylist(pid);

            MFolder *targetFolder = static_cast<MFolder*>(targetContainer);
            MPlaylist *targetPlaylist = static_cast<MPlaylist*>(targetContainer);

            if(dropFolder && !targetFolder) {
                return dropFolder->parentFolder() || !targetPlaylist;
            }
            else if(dropPlaylist && !targetFolder) {
                return dropPlaylist->parentFolder() || !targetPlaylist;
            }
            else if(dropFolder && targetFolder) {
                return dropFolder != targetFolder && !allChildFolders(dropFolder).contains(targetFolder);
            }
            else if(dropPlaylist && targetFolder) {
                return dropPlaylist->parentFolder() != targetFolder;
            }
        }
    }

    return false;
}
bool MModelContainers::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    bool dataIsSonglist = data->hasFormat(MPI3_MIME_TYPE_PIDS_SONGS);
    bool dataIsContainer = data->hasFormat(MPI3_MIME_TYPE_PIDS_CONTAINERS);
    bool dataIsValidMediaFiles = data->hasUrls() ? Mpi3::Core::validMediaFiles(data->urls()) : false;
    bool actionIsCopyAction = action == Qt::CopyAction;
    bool actionIsMoveAction = action == Qt::MoveAction;

    QModelIndex targetIndex = index(row, column, parent);
    MContainer *targetContainer = m_mediaLibrary->getContainer(pidAt(targetIndex));

    if(!targetContainer) {
        if(actionIsMoveAction && dataIsContainer) {
            QString pid = data->data(MPI3_MIME_TYPE_PIDS_CONTAINERS);
            MContainer *dropContainer = m_mediaLibrary->getContainer(pid);

            if(dropContainer && dropContainer->parentFolder()){
                m_mediaLibrary->edit(dropContainer, "parentFolder", QVariant());
                return true;
            }
        }
        else if(actionIsCopyAction && dataIsValidMediaFiles) {
            for(const QUrl &url : data->urls()) {
                MSongInfo songInfo;
                if (songInfo.load(url.toString())) {
                    m_mediaLibrary->newSong(songInfo.songInfoMap());
                }
            }
            return true;
        }
    }
    else if(targetContainer->type() == Mpi3::PlaylistElement) {
        MPlaylist *targetPlaylist = static_cast<MPlaylist*>(targetContainer);

        if(actionIsCopyAction && dataIsSonglist) {
            QByteArray pidBytes = data->data(MPI3_MIME_TYPE_PIDS_SONGS);
            QStringList pidStrings = Mpi3::Core::bytesToSongs(pidBytes);
            QStringList pidStringsCombined = targetPlaylist->songsPidList();
            pidStringsCombined.append(pidStrings);
            m_mediaLibrary->edit(targetPlaylist, "songs", pidStringsCombined);
            return true;
        }
        else if(actionIsCopyAction && dataIsValidMediaFiles) {
            QStringList pidStrings;
            for(const QUrl &url : data->urls()) {
                MSongInfo songInfo;
                if (songInfo.load(url.toString())) {
                    MSong *song = m_mediaLibrary->newSong(songInfo.songInfoMap());
                    pidStrings << song->pid();
                }
            }
            QStringList pidStringsCombined = targetPlaylist->songsPidList();
            pidStringsCombined.append(pidStrings);
            m_mediaLibrary->edit(targetPlaylist, "songs", pidStringsCombined);
            return true;
        }
    }
    else if(targetContainer->type() == Mpi3::FolderElement) {
        MFolder *targetFolder = static_cast<MFolder*>(targetContainer);

        if(actionIsMoveAction && dataIsContainer) {
            QString pid = data->data(MPI3_MIME_TYPE_PIDS_CONTAINERS);
            MContainer *dropContainer = m_mediaLibrary->getContainer(pid);

            if(dropContainer){
                m_mediaLibrary->edit(dropContainer, "parentFolder", targetFolder->pid());
                return true;
            }
        }
    }

    return false;
}

QModelIndex MModelContainers::index(int row, int column, const QModelIndex &parent) const
{
    MModelItem *parentItem = getItem(parent);
    MModelItem *childItem = parentItem->child(row);

    if(childItem) {
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}
QModelIndex MModelContainers::parent(const QModelIndex &index) const
{
    if(index.isValid()) {
        MModelItem *childItem = getItem(index);
        MModelItem *parentItem = childItem->parent();

        if(parentItem != m_rootItem) {
            return createIndex(parentItem->childNumber(), 0, parentItem);
        }
    }

    return QModelIndex();
}

int MModelContainers::rowCount(const QModelIndex &parent) const
{
    MModelItem *parentItem = getItem(parent);
    return parentItem->childCount();
}
int MModelContainers::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant MModelContainers::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && index.column() == 0) {
        if(role == Qt::DecorationRole) {
            MModelItem *item = getItem(index);
            if(item->type() == MModelItem::FolderItem){
                return iconFolder;
            }
            else if(item->type() == MModelItem::PlaylistItem){
                return iconPlaylist;
            }
        }
        else if(role == Qt::DisplayRole || role == Qt::EditRole) {
            MModelItem *item = getItem(index);
            return item->label();
        }
    }

    return QVariant();
}
bool MModelContainers::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole) {
        MModelItem *item = getItem(index);
        MContainer *c = m_mediaLibrary->getContainer(item->pid());
        m_mediaLibrary->edit(c, "name", value);
        return true;
    }

    return false;
}

QVariant MModelContainers::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0) {
        return m_rootItem->label();
    }

    return QVariant();
}

QList<MModelItem*> MModelContainers::allItems(MModelItem *parentItem) const
{
    if(!parentItem){
        parentItem = m_rootItem;
    }

    QList<MModelItem*> childItems;
    for(int i = 0; i < parentItem->childCount(); i++){
        MModelItem *childItem = parentItem->child(i);
        childItems.append(childItem);

        for(MModelItem *c : allItems(childItem)){
            childItems.append(c);
        }
    }

    return childItems;
}
MModelItem *MModelContainers::getItem(const QModelIndex &index) const
{
    if(index.isValid()) {
        MModelItem *item = static_cast<MModelItem*>(index.internalPointer());
        if(item) {
            return item;
        }
    }

    return m_rootItem;
}
MModelItem *MModelContainers::getItem(const QString &pid) const
{
    for(MModelItem *item : allItems()){
        if(item->pid() == pid){
            return item;
        }
    }

    return m_rootItem;
}
QModelIndex MModelContainers::getIndex(const QString &pid) const
{
    for(QModelIndex idx : persistentIndexList()) {
        if(getItem(idx)->pid() == pid) {
            return idx;
        }
    }

    return QModelIndex();
}
QString MModelContainers::pidAt(const QModelIndex &index) const
{
    return getItem(index)->pid();
}

bool MModelContainers::itemIsPlaylist(const QModelIndex &index) const
{
    return getItem(index)->type() == MModelItem::PlaylistItem;
}
bool MModelContainers::itemIsFolder(const QModelIndex &index) const
{
    return getItem(index)->type() == MModelItem::FolderItem;
}

void MModelContainers::setLibrary(MMediaLibrary *library)
{
    beginResetModel();

    m_rootItem->removeChildren(0, rowCount());

    if(m_mediaLibrary) {
        disconnect(m_mediaLibrary, nullptr, this, nullptr);
    }

    m_mediaLibrary = library;
    connect(m_mediaLibrary, &MMediaLibrary::libraryReset, this, [this](){beginResetModel(); m_rootItem->removeChildren(0, rowCount()); endResetModel();});
    connect(m_mediaLibrary, &MMediaLibrary::folderCreated, this, &MModelContainers::folderCreated);
    connect(m_mediaLibrary, &MMediaLibrary::playlistCreated, this, &MModelContainers::playlistCreated);
    connect(m_mediaLibrary, &MMediaLibrary::folderDeleted, this, &MModelContainers::folderDeleted);
    connect(m_mediaLibrary, &MMediaLibrary::playlistDeleted, this, &MModelContainers::playlistDeleted);
    connect(m_mediaLibrary, &MMediaLibrary::folderChanged, this, &MModelContainers::folderChanged);
    connect(m_mediaLibrary, &MMediaLibrary::playlistChanged, this, &MModelContainers::playlistChanged);
    connect(m_mediaLibrary, &MMediaLibrary::parentFolderChanged, this, &MModelContainers::parentFolderChanged);

    MFolderList parentFolders;
    MFolderList childFolders;

    for(MFolder *f : m_mediaLibrary->folders()){
        if(f->parentFolder()){
            childFolders.append(f);
        }
        else {
            parentFolders.append(f);
        }
    }

    for(MFolder *f : parentFolders){
        int position = m_rootItem->childCount();
        m_rootItem->insertChildren(position, 1);
        populateItem(m_rootItem->child(position), f);
    }

    while(allItems().size() != m_mediaLibrary->folders().size()){

        for(MFolder *f : childFolders){
            MModelItem *item = getItem(f->parentFolder()->pid());

            if(item != m_rootItem){
                int position = item->childCount();
                item->insertChildren(position, 1);
                populateItem(item->child(position), f);
            }
        }
    }

    for(MPlaylist *p : m_mediaLibrary->playlists()){
        MModelItem *item = m_rootItem;

        if(p->parentFolder()){
            item = getItem(p->parentFolder()->pid());
        }

        int position = item->childCount();
        item->insertChildren(position, 1);
        populateItem(item->child(position), p);
    }

    endResetModel();
}

void MModelContainers::containerCreated(MContainer *c)
{
    MModelItem *parentItem = m_rootItem;
    QModelIndex parentIndex = QModelIndex();

    if(c->parentFolder()){
        parentItem = getItem(c->parentFolder()->pid());
        parentIndex = getIndex(c->parentFolder()->pid());
    }

    int row = parentItem->childCount();
    if(c->type() == Mpi3::FolderElement){
        for(int i = 0; i < parentItem->childCount(); i++){
            if(parentItem->child(i)->type() == MModelItem::PlaylistItem){
                row = i;
                break;
            }
        }
    }

    beginInsertRows(parentIndex, row, row);
    parentItem->insertChildren(row, 1);
    populateItem(parentItem->child(row), c);
    endInsertRows();
}
void MModelContainers::containerDeleted(MContainer *c)
{
    MModelItem *childItem = getItem(c->pid());

    if(childItem != m_rootItem){
        QModelIndex idx = getIndex(c->pid());
        beginRemoveRows(idx.parent(), idx.row(), idx.row());
        childItem->parent()->removeChildren(childItem->childNumber(), 1);
        endRemoveRows();
    }
}
void MModelContainers::containerChanged(MContainer *c)
{
    MModelItem *childItem = getItem(c->pid());

    if(childItem != m_rootItem){
        QModelIndex idx = getIndex(c->pid());
        childItem->setLabel(c->name());

        QVector<int> roles = {Qt::DisplayRole};
        emit dataChanged(idx, idx, roles);
    }
}

void MModelContainers::folderCreated(MFolder *f)
{
    containerCreated(f);
}
void MModelContainers::playlistCreated(MPlaylist *p)
{
    containerCreated(p);
}
void MModelContainers::folderDeleted(MFolder *f)
{
    containerDeleted(f);
}
void MModelContainers::playlistDeleted(MPlaylist *p)
{
    containerDeleted(p);
}
void MModelContainers::folderChanged(MFolder *f)
{
    containerChanged(f);
}
void MModelContainers::playlistChanged(MPlaylist *p)
{
    containerChanged(p);
}
void MModelContainers::parentFolderChanged(MContainer *c)
{
    QModelIndex sourceIdx = getIndex(c->pid());
    QModelIndex parentIdx = c->parentFolder() ? getIndex(c->parentFolder()->pid()) : QModelIndex();

    MModelItem *sourceItem = getItem(sourceIdx);
    MModelItem *parentItem = getItem(parentIdx);

    int row = parentItem->childCount();
    if(c->type() == Mpi3::FolderElement){
        for(int i = 0; i < parentItem->childCount(); i++){
            if(parentItem->child(i)->type() == MModelItem::PlaylistItem){
                row = i;
                break;
            }
        }
    }

    beginMoveRows(sourceIdx, sourceIdx.row(), sourceIdx.row(), parentIdx, row);
    sourceItem->move(parentItem, row);
    endMoveRows();
}
