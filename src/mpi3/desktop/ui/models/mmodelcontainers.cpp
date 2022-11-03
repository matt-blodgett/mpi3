#include "mpi3/desktop/ui/models/mmodelcontainers.h"
#include "mpi3/desktop/ui/models/mmodelcontainersitem.h"
#include "mpi3/core/mmedialibrary.h"
#include "mpi3/core/mmediautil.h"

#include <QMimeData>
#include <QUrl>


#include <QDebug>


typedef MModelContainersItem MModelItem;


static void populateItem(MModelItem *item, MContainer *container)
{
    item->setPID(container->pid());
    item->setLabel(container->name());

    if(container->type() == Mpi3::FolderElement) {
        item->setType(MModelItem::FolderItem);
    }
    else if(container->type() == Mpi3::PlaylistElement) {
        item->setType(MModelItem::PlaylistItem);
    }
}
static MFolderList allChildFolders(MFolder *parentFolder)
{
    MFolderList folders;
    for(MFolder *childFolder : parentFolder->childFolders()) {
        folders.append(childFolder);
        for(MFolder *f : allChildFolders(childFolder)) {
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

        if(actionIsMoveAction && dataIsContainer) {
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

            if(dropContainer && dropContainer->parentFolder()) {
                m_mediaLibrary->edit(dropContainer, "parentFolder", QVariant());
                return true;
            }
        }
        else if(actionIsCopyAction && dataIsValidMediaFiles) {
            for(const QUrl &url : data->urls()) {
                m_mediaLibrary->newSong(url);
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
                MSong *song = m_mediaLibrary->newSong(url);
                if (song) {
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

            if(dropContainer) {
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
            if(item->type() == MModelItem::FolderItem) {
                return iconFolder;
            }
            else if(item->type() == MModelItem::PlaylistItem) {
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
        MContainer *container = m_mediaLibrary->getContainer(item->pid());
        m_mediaLibrary->edit(container, "name", value);
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
    if(!parentItem) {
        parentItem = m_rootItem;
    }

    QList<MModelItem*> childItems;
    for(int i = 0; i < parentItem->childCount(); i++) {
        MModelItem *childItem = parentItem->child(i);
        childItems << childItem;

        for(MModelItem *child : allItems(childItem)) {
            childItems.append(child);
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
    for(MModelItem *item : allItems()) {
        if(item->pid() == pid) {
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
    connect(m_mediaLibrary, &MMediaLibrary::libraryReset, this, [this]() {beginResetModel(); m_rootItem->removeChildren(0, rowCount()); endResetModel();});
    connect(m_mediaLibrary, &MMediaLibrary::folderCreated, this, &MModelContainers::folderCreated);
    connect(m_mediaLibrary, &MMediaLibrary::playlistCreated, this, &MModelContainers::playlistCreated);
    connect(m_mediaLibrary, &MMediaLibrary::folderDeleted, this, &MModelContainers::folderDeleted);
    connect(m_mediaLibrary, &MMediaLibrary::playlistDeleted, this, &MModelContainers::playlistDeleted);
    connect(m_mediaLibrary, &MMediaLibrary::folderChanged, this, &MModelContainers::folderChanged);
    connect(m_mediaLibrary, &MMediaLibrary::playlistChanged, this, &MModelContainers::playlistChanged);
    connect(m_mediaLibrary, &MMediaLibrary::parentFolderChanged, this, &MModelContainers::parentFolderChanged);

    MFolderList parentFolders;
    MFolderList childFolders;

    for(MFolder *folder : m_mediaLibrary->folders()) {
        if(folder->parentFolder()) {
            childFolders.append(folder);
        }
        else {
            parentFolders.append(folder);
        }
    }

    for(MFolder *folder : parentFolders) {
        int position = m_rootItem->childCount();
        m_rootItem->insertChildren(position, 1);
        populateItem(m_rootItem->child(position), folder);
    }

    while(allItems().size() != m_mediaLibrary->folders().size()) {

        for(MFolder *folder : childFolders) {
            MModelItem *item = getItem(folder->parentFolder()->pid());

            if(item != m_rootItem) {
                int position = item->childCount();
                item->insertChildren(position, 1);
                populateItem(item->child(position), folder);
            }
        }
    }

    for(MPlaylist *playlist : m_mediaLibrary->playlists()) {
        MModelItem *item = m_rootItem;

        if(playlist->parentFolder()) {
            item = getItem(playlist->parentFolder()->pid());
        }

        int position = item->childCount();
        item->insertChildren(position, 1);
        populateItem(item->child(position), playlist);
    }

    endResetModel();
}

void MModelContainers::containerCreated(MContainer *container)
{
    MModelItem *parentItem = m_rootItem;
    QModelIndex parentIndex = QModelIndex();

    if(container->parentFolder()) {
        parentItem = getItem(container->parentFolder()->pid());
        parentIndex = getIndex(container->parentFolder()->pid());
    }

    int row = parentItem->childCount();
    if(container->type() == Mpi3::FolderElement) {
        for(int i = 0; i < parentItem->childCount(); i++) {
            if(parentItem->child(i)->type() == MModelItem::PlaylistItem) {
                row = i;
                break;
            }
        }
    }

    beginInsertRows(parentIndex, row, row);
    parentItem->insertChildren(row, 1);
    populateItem(parentItem->child(row), container);
    endInsertRows();
}
void MModelContainers::containerDeleted(MContainer *container)
{
    MModelItem *childItem = getItem(container->pid());

    if(childItem != m_rootItem) {
        QModelIndex idx = getIndex(container->pid());
        beginRemoveRows(idx.parent(), idx.row(), idx.row());
        childItem->parent()->removeChildren(childItem->childNumber(), 1);
        endRemoveRows();
    }
}
void MModelContainers::containerChanged(MContainer *container)
{
    MModelItem *childItem = getItem(container->pid());

    if(childItem != m_rootItem) {
        QModelIndex idx = getIndex(container->pid());
        childItem->setLabel(container->name());

        QVector<int> roles = { Qt::DisplayRole };
        emit dataChanged(idx, idx, roles);
    }
}

void MModelContainers::folderCreated(MFolder *folder)
{
    containerCreated(folder);
}
void MModelContainers::playlistCreated(MPlaylist *playlist)
{
    containerCreated(playlist);
}
void MModelContainers::folderDeleted(MFolder *folder)
{
    containerDeleted(folder);
}
void MModelContainers::playlistDeleted(MPlaylist *playlist)
{
    containerDeleted(playlist);
}
void MModelContainers::folderChanged(MFolder *folder)
{
    containerChanged(folder);
}
void MModelContainers::playlistChanged(MPlaylist *playlist)
{
    containerChanged(playlist);
}
void MModelContainers::parentFolderChanged(MContainer *container)
{
    QModelIndex sourceIdx = getIndex(container->pid());
    QModelIndex parentIdx = container->parentFolder() ? getIndex(container->parentFolder()->pid()) : QModelIndex();

    MModelItem *sourceItem = getItem(sourceIdx);
    MModelItem *parentItem = getItem(parentIdx);

    int row = parentItem->childCount();
    if(container->type() == Mpi3::FolderElement) {
        for(int i = 0; i < parentItem->childCount(); i++) {
            if(parentItem->child(i)->type() == MModelItem::PlaylistItem) {
                row = i;
                break;
            }
        }
    }

    beginMoveRows(sourceIdx, sourceIdx.row(), sourceIdx.row(), parentIdx, row);
    sourceItem->move(parentItem, row);
    endMoveRows();
}
