#include "mmodelcontainers.h"
#include "mmodelitem.h"
#include "mmedialibrary.h"

#include <QMimeData>
#include <QPixmap>


#include <QDebug>


MModelContainers::MModelContainers(QObject *parent) : QAbstractItemModel(parent)
{
    m_rootItem = new MModelItem();

    m_rootItem->setData(0, "Playlists");
    insertColumns(0, 1);
}
MModelContainers::~MModelContainers()
{
    delete m_rootItem;
}

Qt::ItemFlags MModelContainers::flags(const QModelIndex &index) const
{
    if(index.isValid()) {
        return Qt::ItemIsEditable
                | Qt::ItemIsDragEnabled
                | Qt::ItemIsDropEnabled
                | Qt::ItemIsSelectable
                | Qt::ItemIsEnabled;
    }

    return Qt::ItemIsDropEnabled;
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
    mTypes << QMetaType::typeName(qMetaTypeId<QString>());
    return mTypes;
}
QMimeData *MModelContainers::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mData = new QMimeData();
    if(indexes.size() == 1) {
        QByteArray pidBytes;
        pidBytes.append(getPID(indexes.at(0)));
        mData->setData(QMetaType::typeName(qMetaTypeId<QString>()), pidBytes);
    }

    return mData;
}

bool MModelContainers::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
    bool dataIsContainer = data->hasFormat(QMetaType::typeName(qMetaTypeId<QString>()));
    bool dataIsValidMediaFiles = data->hasUrls() ? MMediaLibrary::validMediaFiles(data->urls()) : false;
    bool actionIsCopyAction = action == Qt::CopyAction;
    bool actionIsMoveAction = action == Qt::MoveAction;

    QModelIndex dropIndex = index(row, column, parent);

    if(actionIsMoveAction && dataIsContainer) {
        QString pid = data->data(QMetaType::typeName(qMetaTypeId<QString>()));

        MFolder *moveFolder = m_mediaLibrary->getFolder(pid);
        MPlaylist *movePlaylist = m_mediaLibrary->getPlaylist(pid);

        MFolder *dropFolder = m_mediaLibrary->getFolder(getPID(dropIndex));
        MPlaylist *dropPlaylist = m_mediaLibrary->getPlaylist(getPID(dropIndex));

        if(moveFolder && !dropFolder) {
            return moveFolder->parentFolder() || !dropPlaylist;
        }
        else if(movePlaylist && !dropFolder) {
            return movePlaylist->parentFolder() || !dropPlaylist;
        }
        else if(moveFolder && dropFolder) {
            return moveFolder != dropFolder && !moveFolder->childContainers().contains(dropFolder);
        }
        else if(movePlaylist && dropFolder) {
            return movePlaylist->parentFolder() != dropFolder;
        }

        return false;
    }

    MMediaElement *dropElement = m_mediaLibrary->getElement(getPID(dropIndex));

    if(!dropElement) {
        return (actionIsCopyAction && dataIsValidMediaFiles);
    }
    else if(dropElement->type() == Mpi3::PlaylistElement) {
        return actionIsCopyAction && (dataIsSonglist || dataIsValidMediaFiles);
    }

    return false;
}
bool MModelContainers::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
    bool dataIsContainer = data->hasFormat(QMetaType::typeName(qMetaTypeId<QString>()));
    bool dataIsValidMediaFiles = data->hasUrls() ? m_mediaLibrary->validMediaFiles(data->urls()) : false;
    bool actionIsCopyAction = action == Qt::CopyAction;
    bool actionIsMoveAction = action == Qt::MoveAction;

    QModelIndex dropIndex = index(row, column, parent);
    MMediaElement *dropElement = m_mediaLibrary->getElement(getPID(dropIndex));

    if(!dropElement) {

        if(actionIsMoveAction && dataIsContainer) {
            QString pid = data->data(QMetaType::typeName(qMetaTypeId<QString>()));
            MMediaElement *moveElement = m_mediaLibrary->getElement(pid);

            if(!moveElement) {
                return false;
            }
            else if(moveElement->type() == Mpi3::PlaylistElement) {
                MPlaylist *movePlaylist = static_cast<MPlaylist*>(moveElement);
                m_mediaLibrary->move(movePlaylist, nullptr);
                return true;
            }
            else if(moveElement->type() == Mpi3::FolderElement) {
                MFolder *moveFolder = static_cast<MFolder*>(moveElement);
                m_mediaLibrary->move(moveFolder, nullptr);
                return true;
            }
            else {
                return false;
            }
        }
        else if(actionIsCopyAction && dataIsValidMediaFiles) {
            foreach(QUrl url, data->urls()) {
                MSong *droppedSong = m_mediaLibrary->newSong(url.toString());
                m_mediaLibrary->insert(droppedSong);
            }
            return true;
        }
    }

    else if(dropElement->type() == Mpi3::PlaylistElement) {
        MPlaylist *parentPlaylist = static_cast<MPlaylist*>(dropElement);

        if(actionIsCopyAction && dataIsSonglist) {
            QByteArray pidBytes = data->data(QMetaType::typeName(qMetaTypeId<QStringList>()));
            foreach(MSong *song, m_mediaLibrary->songsFromBytes(pidBytes)) {
                m_mediaLibrary->insert(song, parentPlaylist);
            }
            return true;
        }
        else if(actionIsCopyAction && dataIsValidMediaFiles) {
            foreach(QUrl url, data->urls()) {
                MSong *droppedSong = m_mediaLibrary->newSong(url.toString());
                m_mediaLibrary->insert(droppedSong, parentPlaylist);
            }
            return true;
        }
    }

    else if(dropElement->type() == Mpi3::FolderElement) {
        if(actionIsMoveAction && dataIsContainer) {
            MFolder *parentFolder = static_cast<MFolder*>(dropElement);

            QString pid = data->data(QMetaType::typeName(qMetaTypeId<QString>()));
            MMediaElement *moveElement = m_mediaLibrary->getElement(pid);

            if(!moveElement) {
                return false;
            }
            else if(moveElement->type() == Mpi3::PlaylistElement) {
                MPlaylist *movePlaylist = static_cast<MPlaylist*>(moveElement);
                m_mediaLibrary->move(movePlaylist, parentFolder);
                return true;
            }
            else if(moveElement->type() == Mpi3::FolderElement) {
                MFolder *moveFolder = static_cast<MFolder*>(moveElement);
                m_mediaLibrary->move(moveFolder, parentFolder);
                return true;
            }
            else {
                return false;
            }
        }
    }

    return false;
}

int MModelContainers::rowCount(const QModelIndex &parent) const
{
    MModelItem *parentItem = getItem(parent);
    return parentItem->childCount();
}
int MModelContainers::columnCount(const QModelIndex &) const
{
    return m_rootItem->columnCount();
}

QModelIndex MModelContainers::index(int row, int column, const QModelIndex &parent) const
{
    MModelItem *parentItem = getItem(parent);
    MModelItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    }
    else {
        return QModelIndex();
    }
}
QModelIndex MModelContainers::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    MModelItem *childItem = getItem(index);
    MModelItem *parentItem = childItem->parent();
    if (parentItem == m_rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

QVariant MModelContainers::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

//    Qt::FontRole
//    Qt::TextAlignmentRole
//    Qt::BackgroundRole
//    Qt::ForegroundRole
//    Qt::CheckStateRole

    MModelItem *item = getItem(index);
    if(role == Qt::DecorationRole && index.column() == 0) {
        return item->icon();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return item->data(index.column());
    }

    return QVariant();
}
QVariant MModelContainers::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return m_rootItem->data(section);
    }

    return QVariant();
}

bool MModelContainers::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
        MModelItem *item = getItem(index);
        bool result = item->setData(index.column(), value);

        if(result) {
            QString pid = getPID(item);
            m_mediaLibrary->modify(pid, value.toString());
            emit dataChanged(index, index);
        }
        return result;
    }

    return false;
}
bool MModelContainers::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal) {
        return false;
    }

    bool result = m_rootItem->setData(section, value);

    if (result) {
        emit headerDataChanged(orientation, section, section);
    }

    return result;
}

bool MModelContainers::insertRows(int position, int count, const QModelIndex &parent)
{
    if(count > 0) {
        MModelItem *parentItem = getItem(parent);

        beginInsertRows(parent, position, position + count - 1);
        bool success = parentItem->insertChildren(position, count, m_rootItem->columnCount());
        endInsertRows();

        return success;
    }

    return false;
}
bool MModelContainers::insertColumns(int position, int count, const QModelIndex &parent)
{
    if(count > 0) {
        beginInsertColumns(parent, position, position + count - 1);
        bool success = m_rootItem->insertColumns(position, count);
        endInsertColumns();

        return success;
    }

    return false;
}

bool MModelContainers::removeRows(int position, int count, const QModelIndex &parent)
{
    if(count > 0) {
        MModelItem *parentItem = getItem(parent);

        beginRemoveRows(parent, position, position + count - 1);
        bool success = parentItem->removeChildren(position, count);
        endRemoveRows();

        return success;
    }

    return false;
}
bool MModelContainers::removeColumns(int position, int count, const QModelIndex &parent)
{
    if(count > 0) {
        beginRemoveColumns(parent, position, position + count - 1);
        bool success = m_rootItem->removeColumns(position, count);
        endRemoveColumns();

        if (m_rootItem->columnCount() == 0) {
            removeRows(0, rowCount());
        }

        return success;
    }

    return false;
}

QModelIndex MModelContainers::getIndex(const QString &pid) const
{
    foreach(QModelIndex idx, persistentIndexList()) {
        if(getPID(idx) == pid) {
            return idx;
        }
    }

    return QModelIndex();
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
    return !m_libItems.keys().contains(pid) ? m_rootItem : m_libItems[pid];
}
QString MModelContainers::getPID(const QModelIndex &index) const
{
    return getPID(getItem(index));
}
QString MModelContainers::getPID(MModelItem *item) const
{
    if(!item || !m_libItems.values().contains(item)) {
        return QString();
    }

    return m_libItems.key(item);
}

MMediaLibrary* MModelContainers::library() const
{
    return m_mediaLibrary;
}
void MModelContainers::setLibrary(MMediaLibrary *library)
{
    beginResetModel();

    if(m_mediaLibrary) {
        disconnect(m_mediaLibrary, nullptr, this, nullptr);
    }

    m_mediaLibrary = library;

    if(m_mediaLibrary) {
        connect(m_mediaLibrary, &MMediaLibrary::elementModified, this, &MModelContainers::elementModified);
        connect(m_mediaLibrary, &MMediaLibrary::elementInserted, this, &MModelContainers::elementInserted);
//        connect(m_mediaLibrary, &MMediaLibrary::elementRemoved, this, &MModelContainers::elementRemoved);
        connect(m_mediaLibrary, &MMediaLibrary::elementMoved, this, &MModelContainers::elementMoved);
        connect(m_mediaLibrary, &MMediaLibrary::elementDeleted, this, &MModelContainers::elementDeleted);

        connect(m_mediaLibrary, &MMediaLibrary::completedLoading, this, [this]() {setLibrary(m_mediaLibrary);});
        connect(m_mediaLibrary, &MMediaLibrary::completedResetting, this, [this]() {setLibrary(m_mediaLibrary);});
    }

    removeRows(0, rowCount());
    m_libItems.clear();
    populate();

    endResetModel();
}

MFolder *MModelContainers::getParentFolder(const QModelIndex &index) const
{
    if(index.isValid()) {
        MMediaContainer *container = getContainer(index);
        if(container->type() == Mpi3::FolderElement) {
            return static_cast<MFolder*>(container);
        }
        else {
            return container->parentFolder();
        }
    }

    return nullptr;
}
MMediaContainer *MModelContainers::getContainer(const QModelIndex &index) const
{
    if(index.isValid()){
        return m_mediaLibrary->getContainer(getPID(index));
    }

    return nullptr;
}
MPlaylist *MModelContainers::getPlaylist(const QModelIndex &index) const
{
    if(index.isValid()){
        return m_mediaLibrary->getPlaylist(getPID(index));
    }

    return nullptr;
}
MFolder *MModelContainers::getFolder(const QModelIndex &index) const
{
    if(index.isValid()){
        return m_mediaLibrary->getFolder(getPID(index));
    }

    return nullptr;
}

void MModelContainers::populate(MFolder *parentFolder, MModelItem *parentItem)
{
    QVector<MFolder*> childFolders = parentFolder ? parentFolder->folders() : m_mediaLibrary->rootFolders();
    QVector<MPlaylist*> childPlaylists = parentFolder ? parentFolder->playlists() : m_mediaLibrary->rootPlaylists();

    if(!parentItem) {
        parentItem = m_rootItem;
    }

    parentItem->insertChildren(0, childFolders.size() + childPlaylists.size(), columnCount());

    for(int i = 0; i < childFolders.size(); i++) {
        MFolder *childFolder = childFolders.at(i);
        MModelItem *childItem = parentItem->child(i);

        childItem->setData(0, childFolder->name());
        childItem->setIcon(iconFolder);

        m_libItems[childFolder->pid()] = childItem;

        populate(childFolder, childItem);
    }

    for(int i = 0; i < childPlaylists.size(); i++) {
        MPlaylist *childPlaylist = childPlaylists.at(i);
        MModelItem *childItem = parentItem->child(i + childFolders.size());

        childItem->setData(0, childPlaylist->name());
        childItem->setIcon(iconPlaylist);

        m_libItems[childPlaylist->pid()] = childItem;
    }
}
void MModelContainers::playlistInserted(MPlaylist *childPlaylist, MMediaElement *parentElement)
{
    parentElement = parentElement->type() == Mpi3::LibraryElement ? nullptr : parentElement;
    if(!parentElement && childPlaylist->parentFolder()) {
        return;
    }

    QModelIndex parentIndex;
    MModelItem *parentItem = m_rootItem;
    int position = m_mediaLibrary->rootPlaylists().size() - 1;
    position += m_mediaLibrary->rootFolders().size();

    if(parentElement) {
        if(parentElement->type() == Mpi3::FolderElement) {
            MFolder *parentFolder = static_cast<MFolder*>(parentElement);
            parentIndex = getIndex(parentFolder->pid());
            parentItem = getItem(parentFolder->pid());
            position = parentFolder->playlists().indexOf(childPlaylist);
            position += parentFolder->folders().size();
        }
    }

    insertRows(position, 1, parentIndex);

    MModelItem *childItem = parentItem->child(position);
    childItem->setData(0, childPlaylist->name());
    childItem->setIcon(iconPlaylist);

    m_libItems[childPlaylist->pid()] = childItem;
}
void MModelContainers::folderInserted(MFolder *childFolder, MMediaElement *parentElement)
{
    parentElement = parentElement->type() == Mpi3::LibraryElement ? nullptr : parentElement;
    if(!parentElement && childFolder->parentFolder()) {
        return;
    }

    QModelIndex parentIndex;
    MModelItem *parentItem = m_rootItem;
    int position = m_mediaLibrary->rootFolders().size() - 1;

    if(parentElement) {
        if(parentElement->type() == Mpi3::FolderElement) {
            MFolder *parentFolder = static_cast<MFolder*>(parentElement);
            parentIndex = getIndex(parentFolder->pid());
            parentItem = getItem(parentFolder->pid());
            position = parentFolder->folders().indexOf(childFolder);
        }
    }

    insertRows(position, 1, parentIndex);

    MModelItem *childItem = parentItem->child(position);
    childItem->setData(0, childFolder->name());
    childItem->setIcon(iconFolder);

    m_libItems[childFolder->pid()] = childItem;

    populate(childFolder, childItem);
}

void MModelContainers::elementModified(MMediaElement *elemModified)
{
    if(m_libItems.keys().contains(elemModified->pid())) {
        m_libItems[elemModified->pid()]->setData(0, elemModified->name());

        QModelIndex changedIndex = getIndex(elemModified->pid());
        emit dataChanged(changedIndex, changedIndex);
    }
}
void MModelContainers::elementInserted(MMediaElement *elemInserted, MMediaElement *elemParent)
{
    if(elemInserted->type() == Mpi3::PlaylistElement) {
        MPlaylist *sc_insertedPlaylist = static_cast<MPlaylist*>(elemInserted);
        playlistInserted(sc_insertedPlaylist, elemParent);
    }
    else if(elemInserted->type() == Mpi3::FolderElement) {
        MFolder *sc_insertedFolder = static_cast<MFolder*>(elemInserted);
        folderInserted(sc_insertedFolder, elemParent);
    }
}
//void MModelContainers::elementRemoved(MMediaElement *elemRemoved, MMediaElement *elemParent)
//{
//    if(elemRemoved->type() == Mpi3::FolderElement || elemRemoved->type() == Mpi3::PlaylistElement) {
//        QModelIndex parentIndex = getIndex(elemParent->pid());
//        QModelIndex childIndex = getIndex(elemRemoved->pid());

//        if(childIndex.isValid()) {
//            removeRows(childIndex.row(), 1, parentIndex);
//            m_libItems.remove(elemRemoved->pid());
//        }
//    }
//}
void MModelContainers::elementMoved(MMediaElement *elemMoved, MMediaElement *elemParent)
{
    if(elemMoved->type() != Mpi3::SongElement) {
        QModelIndex moveIndex = getIndex(elemMoved->pid());
        QModelIndex sourceIndex = moveIndex.parent();
        removeRows(moveIndex.row(), 1, sourceIndex);
    }

    if(elemMoved->type() == Mpi3::PlaylistElement) {
        MPlaylist *sc_movedPlaylist = static_cast<MPlaylist*>(elemMoved);
        playlistInserted(sc_movedPlaylist, elemParent);
    }
    else if(elemMoved->type() == Mpi3::FolderElement) {
        MFolder *sc_movedFolder = static_cast<MFolder*>(elemMoved);
        folderInserted(sc_movedFolder, elemParent);
    }
}
void MModelContainers::elementDeleted(MMediaElement *elemDeleted)
{
    qDebug() << "model deleted";
    if(elemDeleted->type() == Mpi3::FolderElement || elemDeleted->type() == Mpi3::PlaylistElement) {
        QModelIndex childIndex = getIndex(elemDeleted->pid());
        QModelIndex parentIndex = childIndex.parent();

        qDebug() << childIndex.row();

        removeRows(childIndex.row(), 1, parentIndex);

        m_libItems.remove(elemDeleted->pid());

        qDebug() << elemDeleted->pid();

        if(elemDeleted->type() == Mpi3::FolderElement) {
            MFolder *folder = static_cast<MFolder*>(elemDeleted);
            foreach(MMediaContainer *childContainer, folder->childContainers()) {
                m_libItems.remove(childContainer->pid());
            }
        }
    }
}
