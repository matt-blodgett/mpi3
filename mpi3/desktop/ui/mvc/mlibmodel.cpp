#include "mlibmodel.h"
#include "mlibitem.h"

#include "core/mmedialibrary.h"

#include <QMimeData>
#include <QPixmap>
#include <QUrl>

#include <QDebug>


MModelContainers::MModelContainers(QObject *parent) : QAbstractItemModel(parent){
    m_rootItem = new MModelItem();

    m_rootItem->setData(0, "Playlists");
    insertColumns(0, 1);

    m_icnFolder.addPixmap(QPixmap(":/icons/treeview/folder.png"));
    m_icnPlaylist.addPixmap(QPixmap(":/icons/treeview/playlist.png"));
}
MModelContainers::~MModelContainers(){
    delete m_rootItem;
}

Qt::ItemFlags MModelContainers::flags(const QModelIndex &index) const{
    if(index.isValid()){
        return Qt::ItemIsEditable
                | Qt::ItemIsDragEnabled
                | Qt::ItemIsDropEnabled
                | Qt::ItemIsSelectable
                | Qt::ItemIsEnabled;
    }

    return Qt::ItemIsDropEnabled;
}
Qt::DropActions MModelContainers::supportedDragActions() const{
    return Qt::MoveAction;
}
Qt::DropActions MModelContainers::supportedDropActions() const{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList MModelContainers::mimeTypes() const {
    QStringList mTypes;
    mTypes << QMetaType::typeName(qMetaTypeId<QString>());
    return mTypes;
}
QMimeData* MModelContainers::mimeData(const QModelIndexList &indexes) const{
    QMimeData *mData = new QMimeData();
    if(indexes.size() == 1){
        QByteArray pidBytes;
        pidBytes.append(getPID(indexes.at(0)));
        mData->setData(QMetaType::typeName(qMetaTypeId<QString>()), pidBytes);
    }

    return mData;
}

bool MModelContainers::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const {

    bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
    bool dataIsContainer = data->hasFormat(QMetaType::typeName(qMetaTypeId<QString>()));
    bool dataIsValidMediaFiles = data->hasUrls() ? m_mediaLibrary->validMediaFiles(data->urls()) : false;
    bool actionIsCopyAction = action == Qt::CopyAction;
    bool actionIsMoveAction = action == Qt::MoveAction;

    QModelIndex dropIndex = index(row, column, parent);

    if(actionIsMoveAction && dataIsContainer){
        QString pid = data->data(QMetaType::typeName(qMetaTypeId<QString>()));

        MFolder *moveFolder = m_mediaLibrary->getFolder(pid);
        MPlaylist *movePlaylist = m_mediaLibrary->getPlaylist(pid);

        MFolder *dropFolder = m_mediaLibrary->getFolder(getPID(dropIndex));
        MPlaylist *dropPlaylist = m_mediaLibrary->getPlaylist(getPID(dropIndex));

        if(moveFolder && !dropFolder){
            return moveFolder->parent || !dropPlaylist;
        }
        else if(movePlaylist && !dropFolder){
            return movePlaylist->parent || !dropPlaylist;
        }
        else if(moveFolder && dropFolder){
            return moveFolder != dropFolder && !m_mediaLibrary->allChildFolders(moveFolder).contains(dropFolder);
        }
        else if(movePlaylist && dropFolder){
            return dropPlaylist->parent != dropFolder;
        }

        return false;
    }

    MMediaElement *dropElement = m_mediaLibrary->getElement(getPID(dropIndex));

    if(!dropElement){
        return (actionIsCopyAction && dataIsValidMediaFiles);
    }
    else if(dropElement->type() == Mpi3::PlaylistElement){
        return actionIsCopyAction && (dataIsSonglist || dataIsValidMediaFiles);
    }

    return false;
}
bool MModelContainers::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent){

    bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
    bool dataIsContainer = data->hasFormat(QMetaType::typeName(qMetaTypeId<QString>()));
    bool dataIsValidMediaFiles = data->hasUrls() ? m_mediaLibrary->validMediaFiles(data->urls()) : false;
    bool actionIsCopyAction = action == Qt::CopyAction;
    bool actionIsMoveAction = action == Qt::MoveAction;

    QModelIndex dropIndex = index(row, column, parent);
    MMediaElement *dropElement = m_mediaLibrary->getElement(getPID(dropIndex));

    if(!dropElement){
        if(actionIsMoveAction && dataIsContainer){
            QString pid = data->data(QMetaType::typeName(qMetaTypeId<QString>()));
            MMediaElement *moveElement = m_mediaLibrary->getElement(pid);

            if(!moveElement){
                return false;
            }
            else if(moveElement->type() == Mpi3::PlaylistElement){
                MPlaylist *movePlaylist = static_cast<MPlaylist*>(moveElement);
                m_mediaLibrary->move(movePlaylist, nullptr);
                return true;
            }
            else if(moveElement->type() == Mpi3::FolderElement){
                MFolder *moveFolder = static_cast<MFolder*>(moveElement);
                m_mediaLibrary->move(moveFolder, nullptr);
                return true;
            }
            else {
                return false;
            }
        }
        else if(actionIsCopyAction && dataIsValidMediaFiles){
            for(int i = 0; i < data->urls().size(); i++){
                MSong *droppedSong = m_mediaLibrary->newSong(data->urls().at(i).toString());
                m_mediaLibrary->insert(droppedSong);
            }
            return true;
        }
    }
    else if(dropElement->type() == Mpi3::PlaylistElement){
        MPlaylist *parentPlaylist = static_cast<MPlaylist*>(dropElement);

        if(actionIsCopyAction && dataIsSonglist){
            QByteArray pidBytes = data->data(QMetaType::typeName(qMetaTypeId<QStringList>()));
            QVector<MSong*> droppedSongs = m_mediaLibrary->songsFromData(pidBytes);

            for(int i = 0; i < droppedSongs.size(); i++){
                m_mediaLibrary->insert(droppedSongs.at(i), parentPlaylist);
            }

            return true;
        }
        else if(actionIsCopyAction && dataIsValidMediaFiles){
            for(int i = 0; i < data->urls().size(); i++){
                MSong *droppedSong = m_mediaLibrary->newSong(data->urls().at(i).toString());
                m_mediaLibrary->insert(droppedSong, parentPlaylist);
            }
            return true;
        }
    }
    else if(dropElement->type() == Mpi3::FolderElement){
        if(actionIsMoveAction && dataIsContainer){
            MFolder *parentFolder = static_cast<MFolder*>(dropElement);

            QString pid = data->data(QMetaType::typeName(qMetaTypeId<QString>()));
            MMediaElement *moveElement = m_mediaLibrary->getElement(pid);

            if(!moveElement){
                return false;
            }
            else if(moveElement->type() == Mpi3::PlaylistElement){
                MPlaylist *movePlaylist = static_cast<MPlaylist*>(moveElement);
                m_mediaLibrary->move(movePlaylist, parentFolder);
                return true;
            }
            else if(moveElement->type() == Mpi3::FolderElement){
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

int MModelContainers::rowCount(const QModelIndex &parent) const{
    MModelItem *parentItem = getItem(parent);
    return parentItem->childCount();
}
int MModelContainers::columnCount(const QModelIndex &) const{
    return m_rootItem->columnCount();
}

QModelIndex MModelContainers::index(int row, int column, const QModelIndex &parent) const{
    MModelItem *parentItem = getItem(parent);
    MModelItem *childItem = parentItem->child(row);
    if (childItem){
        return createIndex(row, column, childItem);
    }
    else{
        return QModelIndex();
    }
}
QModelIndex MModelContainers::parent(const QModelIndex &index) const{
    if (!index.isValid()){
        return QModelIndex();
    }

    MModelItem *childItem = getItem(index);
    MModelItem *parentItem = childItem->parent();
    if (parentItem == m_rootItem){
        return QModelIndex();
    }

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

QVariant MModelContainers::data(const QModelIndex &index, int role) const{
    if (!index.isValid()){
        return QVariant();
    }

//    Qt::FontRole
//    Qt::TextAlignmentRole
//    Qt::BackgroundRole
//    Qt::ForegroundRole
//    Qt::CheckStateRole

    MModelItem *item = getItem(index);
    if(role == Qt::DecorationRole && index.column() == 0){
        return item->icon();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole){
        return item->data(index.column());
    }

    return QVariant();
}
QVariant MModelContainers::headerData(int section, Qt::Orientation orientation, int role) const{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return m_rootItem->data(section);
    }

    return QVariant();
}

bool MModelContainers::setData(const QModelIndex &index, const QVariant &value, int role){
    if (role == Qt::EditRole){
        MModelItem *item = getItem(index);
        bool result = item->setData(index.column(), value);

        if(result){
            QString pid = getPID(item);
            m_mediaLibrary->modify(pid, value.toString());
            emit dataChanged(index, index);
        }
        return result;
    }

    return false;
}
bool MModelContainers::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role){
    if (role != Qt::EditRole || orientation != Qt::Horizontal){
        return false;
    }

    bool result = m_rootItem->setData(section, value);

    if (result){
        emit headerDataChanged(orientation, section, section);
    }

    return result;
}

bool MModelContainers::insertRows(int position, int count, const QModelIndex &parent){
    if(count > 0){
        MModelItem *parentItem = getItem(parent);

        beginInsertRows(parent, position, position + count - 1);
        bool success = parentItem->insertChildren(position, count, m_rootItem->columnCount());
        endInsertRows();

        return success;
    }

    return false;
}
bool MModelContainers::insertColumns(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginInsertColumns(parent, position, position + count - 1);
        bool success = m_rootItem->insertColumns(position, count);
        endInsertColumns();

        return success;
    }

    return false;
}

bool MModelContainers::removeRows(int position, int count, const QModelIndex &parent){
    if(count > 0){
        MModelItem *parentItem = getItem(parent);

        beginRemoveRows(parent, position, position + count - 1);
        bool success = parentItem->removeChildren(position, count);
        endRemoveRows();

        return success;
    }

    return false;
}
bool MModelContainers::removeColumns(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginRemoveColumns(parent, position, position + count - 1);
        bool success = m_rootItem->removeColumns(position, count);
        endRemoveColumns();

        if (m_rootItem->columnCount() == 0){
            removeRows(0, rowCount());
        }

        return success;
    }

    return false;
}

QModelIndex MModelContainers::getIndex(const QString &pid) const{
    QModelIndexList pIndexes = persistentIndexList();
    for(int i = 0; i < pIndexes.size(); i++){
        QModelIndex idx = pIndexes.at(i);
        if(getPID(idx) == pid){
            return idx;
        }
    }

    return QModelIndex();
}
MModelItem *MModelContainers::getItem(const QModelIndex &index) const{
    if(index.isValid()){
        MModelItem *item = static_cast<MModelItem*>(index.internalPointer());
        if (item){
            return item;
        }
    }

    return m_rootItem;
}
MModelItem *MModelContainers::getItem(const QString &pid) const{
    return !m_libItems.keys().contains(pid) ? m_rootItem : m_libItems[pid];
}
QString MModelContainers::getPID(const QModelIndex &index) const{
    return getPID(getItem(index));
}
QString MModelContainers::getPID(MModelItem *item) const{
    if(!item || !m_libItems.values().contains(item)){
        return QString();
    }

    return m_libItems.key(item);
}


void MModelContainers::setLibrary(MMediaLibrary *library){
    beginResetModel();

    if(m_mediaLibrary){
        disconnect(m_mediaLibrary.data(), &MMediaLibrary::elementModified, this, &MModelContainers::elementModified);
        disconnect(m_mediaLibrary.data(), &MMediaLibrary::elementInserted, this, &MModelContainers::elementInserted);
        disconnect(m_mediaLibrary.data(), &MMediaLibrary::elementRemoved, this, &MModelContainers::elementRemoved);
        disconnect(m_mediaLibrary.data(), &MMediaLibrary::elementMoved, this, &MModelContainers::elementMoved);
        disconnect(m_mediaLibrary.data(), &MMediaLibrary::elementDeleted, this, &MModelContainers::elementDeleted);
    }

    m_mediaLibrary.take();
    m_mediaLibrary.reset(library);

    if(m_mediaLibrary){
        connect(m_mediaLibrary.data(), &MMediaLibrary::elementModified, this, &MModelContainers::elementModified);
        connect(m_mediaLibrary.data(), &MMediaLibrary::elementInserted, this, &MModelContainers::elementInserted);
        connect(m_mediaLibrary.data(), &MMediaLibrary::elementRemoved, this, &MModelContainers::elementRemoved);
        connect(m_mediaLibrary.data(), &MMediaLibrary::elementMoved, this, &MModelContainers::elementMoved);
        connect(m_mediaLibrary.data(), &MMediaLibrary::elementDeleted, this, &MModelContainers::elementDeleted);
    }

    removeRows(0, rowCount());
    m_libItems.clear();
    populate();

    endResetModel();
}

MFolder* MModelContainers::getParentFolderAt(const QModelIndex &index) const{
    MFolder *parentFolder = nullptr;

    if(index.isValid()){
        MMediaElement *element = m_mediaLibrary->getElement(getPID(index));

        if(!element){
            parentFolder = nullptr;
        }
        else if(element->type() == Mpi3::FolderElement){
            parentFolder = static_cast<MFolder*>(element);
        }
        else if(element->type() == Mpi3::PlaylistElement){
            parentFolder = static_cast<MPlaylist*>(element)->parent;
        }
    }

    return parentFolder;
}

void MModelContainers::populate(MFolder *parentFolder, MModelItem *parentItem){
    QVector<MFolder*> childFolders = parentFolder ? parentFolder->folders : m_mediaLibrary->childFolders();
    QVector<MPlaylist*> childPlaylists = parentFolder ? parentFolder->playlists : m_mediaLibrary->childPlaylists();

    if(!parentItem){
        parentItem = m_rootItem;
    }

    parentItem->insertChildren(0, childFolders.size() + childPlaylists.size(), columnCount());
    for(int i = 0; i < childFolders.size(); i++){
        MFolder *childFolder = childFolders.at(i);
        MModelItem *childItem = parentItem->child(i);

        childItem->setData(0, childFolder->name());
        childItem->setIcon(m_icnFolder);

        m_libItems[childFolder->pid()] = childItem;

        populate(childFolder, childItem);
    }

    for(int i = 0; i < childPlaylists.size(); i++){
        MPlaylist *childPlaylist = childPlaylists.at(i);
        MModelItem *childItem = parentItem->child(i + childFolders.size());

        childItem->setData(0, childPlaylist->name());
        childItem->setIcon(m_icnPlaylist);

        m_libItems[childPlaylist->pid()] = childItem;
    }
}

void MModelContainers::playlistInserted(MPlaylist *childPlaylist, MMediaElement *parentElement){
    if(!parentElement && childPlaylist->parent){
        return;
    }

    QModelIndex parentIndex;
    MModelItem *parentItem = m_rootItem;
    int position = m_mediaLibrary->childPlaylists().size() - 1;
    position += m_mediaLibrary->childFolders().size();

    if(parentElement){
        if(parentElement->type() == Mpi3::FolderElement){
            MFolder *parentFolder = static_cast<MFolder*>(parentElement);
            parentIndex = getIndex(parentFolder->pid());
            parentItem = getItem(parentFolder->pid());
            position = parentFolder->playlists.indexOf(childPlaylist);
            position += parentFolder->folders.size();
        }
    }

    insertRows(position, 1, parentIndex);

    MModelItem *childItem = parentItem->child(position);
    childItem->setData(0, childPlaylist->name());
    childItem->setIcon(m_icnPlaylist);

    m_libItems[childPlaylist->pid()] = childItem;
}
void MModelContainers::folderInserted(MFolder *childFolder, MMediaElement *parentElement){
    if(!parentElement && childFolder->parent){
        return;
    }

    QModelIndex parentIndex;
    MModelItem *parentItem = m_rootItem;
    int position = m_mediaLibrary->childFolders().size() - 1;

    if(parentElement){
        if(parentElement->type() == Mpi3::FolderElement){
            MFolder *parentFolder = static_cast<MFolder*>(parentElement);

            parentIndex = getIndex(parentFolder->pid());
            parentItem = getItem(parentFolder->pid());
            position = parentFolder->folders.indexOf(childFolder);
        }
    }

    insertRows(position, 1, parentIndex);

    MModelItem *childItem = parentItem->child(position);
    childItem->setData(0, childFolder->name());
    childItem->setIcon(m_icnFolder);

    m_libItems[childFolder->pid()] = childItem;

    populate(childFolder, childItem);
}

void MModelContainers::elementModified(MMediaElement *elemModified){
    if(m_libItems.keys().contains(elemModified->pid())){
        m_libItems[elemModified->pid()]->setData(0, elemModified->name());

        QModelIndex changedIndex = getIndex(elemModified->pid());
        emit dataChanged(changedIndex, changedIndex);
    }
}
void MModelContainers::elementInserted(MMediaElement *elemInserted, MMediaElement *elemParent){
    if(elemInserted->type() == Mpi3::PlaylistElement){
        MPlaylist *sc_insertedPlaylist = static_cast<MPlaylist*>(elemInserted);
        playlistInserted(sc_insertedPlaylist, elemParent);
    }
    else if(elemInserted->type() == Mpi3::FolderElement){
        MFolder *sc_insertedFolder = static_cast<MFolder*>(elemInserted);
        folderInserted(sc_insertedFolder, elemParent);
    }
}
void MModelContainers::elementRemoved(MMediaElement *elemRemoved, MMediaElement *elemParent){
    if(elemRemoved->type() == Mpi3::FolderElement || elemRemoved->type() == Mpi3::PlaylistElement) {
        QModelIndex parentIndex = getIndex(elemParent->pid());
        QModelIndex childIndex = getIndex(elemRemoved->pid());

        if(childIndex.isValid()){
            removeRows(childIndex.row(), 1, parentIndex);
            m_libItems.remove(elemRemoved->pid());
        }
    }
}
void MModelContainers::elementMoved(MMediaElement *elemMoved, MMediaElement *elemParent){
    QModelIndex moveIndex = getIndex(elemMoved->pid());
    QModelIndex sourceIndex = moveIndex.parent();
    removeRows(moveIndex.row(), 1, sourceIndex);

    if(elemMoved->type() == Mpi3::PlaylistElement){
        MPlaylist *sc_movedPlaylist = static_cast<MPlaylist*>(elemMoved);
        playlistInserted(sc_movedPlaylist, elemParent);
    }
    else if(elemMoved->type() == Mpi3::FolderElement){
        MFolder *sc_movedFolder = static_cast<MFolder*>(elemMoved);
        folderInserted(sc_movedFolder, elemParent);
    }
}
void MModelContainers::elementDeleted(const QString &pidDeleted, Mpi3::ElementType elemType, QVector<QString> pidChildren){
    if(elemType == Mpi3::FolderElement || elemType == Mpi3::PlaylistElement){
        QModelIndex childIndex = getIndex(pidDeleted);
        QModelIndex parentIndex = getIndex(getPID(getItem(pidDeleted)->parent()));
        removeRows(childIndex.row(), 1, parentIndex);

        m_libItems.remove(pidDeleted);
        if(elemType == Mpi3::FolderElement){
            for(int i = 0; i < pidChildren.size(); i++){
                m_libItems.remove(pidChildren.at(i));
            }
        }
    }
}


MModelSonglist::MModelSonglist(QObject *parent) : QAbstractItemModel(parent){
    m_headers << "Name" << "Artist" << "Path";

    for(int i = 0; i < columnCount(); i++){
        columnVisibility[i] = false;
    }

//    qDebug() << qMetaTypeId<Mpi3ElementList>();
//    qDebug() << QMetaType::typeName(qMetaTypeId<Mpi3ElementList>());
//    qDebug() << "";
//    qDebug() << qMetaTypeId<QStringList>();
//    qDebug() << QMetaType::typeName(qMetaTypeId<QStringList>());
//    qDebug() << "";
}
MModelSonglist::~MModelSonglist(){}

Qt::ItemFlags MModelSonglist::flags(const QModelIndex &index) const{
    if(index.isValid()){
        if(m_currentDisplay == MModelSonglist::DisplayPlaylist){
            return Qt::ItemIsEditable
                    | Qt::ItemIsDragEnabled
                    | Qt::ItemIsDropEnabled
                    | Qt::ItemIsSelectable
                    | Qt::ItemIsEnabled
                    | Qt::ItemNeverHasChildren;
        }
        else{
            return Qt::ItemIsEditable
                    | Qt::ItemIsDropEnabled
                    | Qt::ItemIsSelectable
                    | Qt::ItemIsEnabled
                    | Qt::ItemNeverHasChildren;
        }
    }

    return Qt::ItemIsDropEnabled;
}
Qt::DropActions MModelSonglist::supportedDragActions() const{
    return Qt::CopyAction;
}
Qt::DropActions MModelSonglist::supportedDropActions() const{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList MModelSonglist::mimeTypes() const {
    QStringList mTypes;
    mTypes << QMetaType::typeName(qMetaTypeId<QStringList>());
    return mTypes;
}
QMimeData* MModelSonglist::mimeData(const QModelIndexList &indexes) const{
    QMimeData *mData = new QMimeData();

    QVector<MSong*> selectedSongs;
    for(int i = 0; i < indexes.size(); i++){
        MSong *song = getSongAt(indexes.at(i));
        if(song && !selectedSongs.contains(song)){
            selectedSongs.append(song);
        }
    }

    QByteArray pidBytes = m_mediaLibrary->songsToData(selectedSongs);
    QList<QUrl> songUrls = m_mediaLibrary->songsToPaths(selectedSongs);

    mData->setData(QMetaType::typeName(qMetaTypeId<QStringList>()), pidBytes);
    mData->setUrls(songUrls);

    return mData;
}

bool MModelSonglist::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const {
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

    bool displayIsPlaylist = m_currentDisplay == MModelSonglist::DisplayPlaylist;
    bool displayIsAllSongs = m_currentDisplay == MModelSonglist::DisplayAllSongs;

    if(displayIsPlaylist || displayIsAllSongs){
        bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
        bool dataIsValidMediaFiles = data->hasUrls() ? m_mediaLibrary->validMediaFiles(data->urls()) : false;
        bool actionIsCopyAction = action == Qt::CopyAction;

        return dataIsSonglist || (dataIsValidMediaFiles && actionIsCopyAction);
    }

    return false;
}
bool MModelSonglist::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent){
    Q_UNUSED(column);
    Q_UNUSED(parent);

    bool displayIsPlaylist = m_currentDisplay == MModelSonglist::DisplayPlaylist;
    bool displayIsAllSongs = m_currentDisplay == MModelSonglist::DisplayAllSongs;

    if(displayIsPlaylist || displayIsAllSongs){
        bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
        bool dataIsValidMediaFiles = data->hasUrls() ? m_mediaLibrary->validMediaFiles(data->urls()) : false;
        bool actionIsCopyAction = action == Qt::CopyAction;
        bool actionIsMoveAction = action == Qt::MoveAction;

        if(dataIsSonglist){
            QByteArray pidBytes = data->data(QMetaType::typeName(qMetaTypeId<QStringList>()));
            QVector<MSong*> droppedSongs = m_mediaLibrary->songsFromData(pidBytes);
            MPlaylist *parentPlaylist = m_mediaLibrary->getPlaylist(m_currentContainer);

            if(actionIsCopyAction){
                for(int i = 0; i < droppedSongs.size(); i++){
                    m_mediaLibrary->insert(droppedSongs.at(i), parentPlaylist, row);
                }
                return true;
            }
            else if(actionIsMoveAction && displayIsPlaylist){
                for(int i = 0; i < droppedSongs.size(); i++){
                    m_mediaLibrary->move(droppedSongs.at(i), parentPlaylist, row);
                }
                return true;
            }
        }
        else if(dataIsValidMediaFiles && actionIsCopyAction){
            MPlaylist *parentPlaylist = m_mediaLibrary->getPlaylist(m_currentContainer);

            for(int i = 0; i < data->urls().size(); i++){
                MSong *droppedSong = m_mediaLibrary->newSong(data->urls().at(i).toString());
                m_mediaLibrary->insert(droppedSong, parentPlaylist, row);
            }
            return true;
        }
    }

    return false;
}

int MModelSonglist::rowCount(const QModelIndex &) const{
    return m_currentSonglist.size();
}
int MModelSonglist::columnCount(const QModelIndex &) const{
    return m_headers.size();
}

QModelIndex MModelSonglist::index(int row, int column, const QModelIndex &) const{
    return createIndex(row, column);
}
QModelIndex MModelSonglist::parent(const QModelIndex &) const{
    return QModelIndex();
}

QVariant MModelSonglist::data(const QModelIndex &index, int role) const{
//    Qt::FontRole
//    Qt::TextAlignmentRole
//    Qt::BackgroundRole
//    Qt::ForegroundRole
//    Qt::CheckStateRole

    if(index.isValid()){
        if(role == Qt::DisplayRole || role == Qt::EditRole){
            MSong *song = m_currentSonglist.at(index.row());
            switch(index.column()){
                case 0: return song->name();
                case 1: return song->artist();
                case 2: return song->path();
            }
        }
    }

    return QVariant();
}
QVariant MModelSonglist::headerData(int section, Qt::Orientation orientation, int role) const{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return m_headers.at(section);
    }
    return QVariant();
}

bool MModelSonglist::setData(const QModelIndex &index, const QVariant &value, int role){
    if(index.isValid() && role == Qt::EditRole){
        MSong *song = m_currentSonglist.at(index.row());
        if(song && index.column() == 0){
             m_mediaLibrary->modify(song, value.toString(), MSong::SongName);

             emit dataChanged(index, index);
             return true;
        }
    }
    return false;
}
bool MModelSonglist::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role){
    if (role == Qt::EditRole && orientation == Qt::Horizontal){
        m_headers[section] = value.toString();
        emit headerDataChanged(orientation, section, section);
        return true;
    }

    return false;
}

bool MModelSonglist::insertRows(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginInsertRows(parent, position, position + count - 1);
        endInsertRows();
        return true;
    }
    return false;
}
bool MModelSonglist::insertColumns(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginInsertColumns(parent, position, position + count - 1);
        endInsertColumns();
        return true;
    }
    return false;
}

bool MModelSonglist::removeRows(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginRemoveRows(parent, position, position + count - 1);
        endRemoveRows();
        return true;
    }
    return false;
}
bool MModelSonglist::removeColumns(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginRemoveColumns(parent, position, position + count - 1);
        endRemoveColumns();
        return true;
    }
    return false;
}

void MModelSonglist::setLibrary(MMediaLibrary *library){
    beginResetModel();

    if(m_mediaLibrary){
        disconnect(m_mediaLibrary.data(), &MMediaLibrary::elementModified, this, &MModelSonglist::elementModified);
        disconnect(m_mediaLibrary.data(), &MMediaLibrary::elementInserted, this, &MModelSonglist::elementInserted);
        disconnect(m_mediaLibrary.data(), &MMediaLibrary::elementRemoved, this, &MModelSonglist::elementRemoved);
        disconnect(m_mediaLibrary.data(), &MMediaLibrary::elementMoved, this, &MModelSonglist::elementMoved);
        disconnect(m_mediaLibrary.data(), &MMediaLibrary::elementDeleted, this, &MModelSonglist::elementDeleted);
    }

    m_mediaLibrary.take();
    m_mediaLibrary.reset(library);

    if(m_mediaLibrary){
        connect(m_mediaLibrary.data(), &MMediaLibrary::elementModified, this, &MModelSonglist::elementModified);
        connect(m_mediaLibrary.data(), &MMediaLibrary::elementInserted, this, &MModelSonglist::elementInserted);
        connect(m_mediaLibrary.data(), &MMediaLibrary::elementRemoved, this, &MModelSonglist::elementRemoved);
        connect(m_mediaLibrary.data(), &MMediaLibrary::elementMoved, this, &MModelSonglist::elementMoved);
        connect(m_mediaLibrary.data(), &MMediaLibrary::elementDeleted, this, &MModelSonglist::elementDeleted);
    }

    removeRows(0, rowCount());

    m_currentSonglist.clear();
    m_currentContainer = QString();
    m_currentDisplay = MModelSonglist::DisplayNone;

    endResetModel();
}

QVector<MSong*> MModelSonglist::currentSonglist() const {
    return m_currentSonglist;
}
MSong* MModelSonglist::getSongAt(const QModelIndex &index) const{
    if(index.row() > -1 && index.row() < m_currentSonglist.size()){
        return m_currentSonglist.at(index.row());
    }

    return nullptr;
}

QString MModelSonglist::currentContainer() const {
    return m_currentContainer;
}
MModelSonglist::Display MModelSonglist::currentDisplay() const{
    return m_currentDisplay;
}
void MModelSonglist::setDisplay(MModelSonglist::Display display){
    m_currentDisplay = display;

    beginResetModel();
    removeRows(0, rowCount());
    m_currentSonglist.clear();

    switch(m_currentDisplay){
        case MModelSonglist::DisplayNone: {
            m_currentContainer = QString();
            break;
        }
        case MModelSonglist::DisplayAllSongs: {
            m_currentContainer = m_mediaLibrary->pid();
            m_currentSonglist = *m_mediaLibrary->libSongs;
            break;
        }
        case MModelSonglist::DisplayArtists: {
            m_currentContainer = m_mediaLibrary->pid();
            break;
        }
        case MModelSonglist::DisplayAlbums: {
            m_currentContainer = m_mediaLibrary->pid();
            break;
        }
        case MModelSonglist::DisplayPlaylist: {
            MPlaylist *playlist = m_mediaLibrary->getPlaylist(m_currentContainer);
            if(playlist){
                m_currentSonglist = playlist->songs;
            }
            break;
        }
        case MModelSonglist::DisplayFolder: {
            MFolder *folder = m_mediaLibrary->getFolder(m_currentContainer);
            if(folder){
                m_currentSonglist = m_mediaLibrary->allChildSongs(folder);
            }
            break;
        };
    }

    insertRows(0, m_currentSonglist.size());
    endResetModel();
}
void MModelSonglist::setDisplay(MMediaElement *container){
    if(container->type() == Mpi3::PlaylistElement){
        m_currentContainer = container->pid();
        setDisplay(MModelSonglist::DisplayPlaylist);
    }
    else if(container->type() == Mpi3::FolderElement){
        m_currentContainer = container->pid();
        setDisplay(MModelSonglist::DisplayFolder);
    }
}

void MModelSonglist::elementModified(MMediaElement *elemModified){
    if(elemModified->type() == Mpi3::SongElement){
        MSong *sc_songModified = static_cast<MSong*>(elemModified);

        if(m_currentSonglist.contains(sc_songModified)){
            int position = m_currentSonglist.indexOf(sc_songModified);
            emit dataChanged(index(position, 0), index(position, columnCount()));
        }
    }
}
void MModelSonglist::elementInserted(MMediaElement *elemInserted, MMediaElement *elemParent){
    if(elemInserted->type() == Mpi3::SongElement){
        MSong *sc_songInserted = static_cast<MSong*>(elemInserted);

        if(!elemParent){
            if(m_currentDisplay == MModelSonglist::DisplayAllSongs){
                m_currentSonglist.push_back(sc_songInserted);
                insertRows(rowCount(), 1);
            }
        }
        else if(elemParent->type() == Mpi3::PlaylistElement && elemParent->pid() == m_currentContainer){
            MPlaylist *sc_currentPlaylist = static_cast<MPlaylist*>(elemParent);

            int position = sc_currentPlaylist->songs.indexOf(sc_songInserted);
            m_currentSonglist.insert(position, sc_songInserted);
            insertRows(position, 1);
        }
    }
}
void MModelSonglist::elementRemoved(MMediaElement *elemRemoved, MMediaElement *elemParent){
    if(elemRemoved->type() == Mpi3::SongElement){
        MSong *sc_songRemoved = static_cast<MSong*>(elemRemoved);

        if(elemParent->type() == Mpi3::PlaylistElement && elemParent->pid() == m_currentContainer){
            removeRows(m_currentSonglist.indexOf(sc_songRemoved), 1);
            m_currentSonglist.removeAll(sc_songRemoved);
        }
    }
}
void MModelSonglist::elementMoved(MMediaElement *elemMoved, MMediaElement *elemParent){
    if(elemMoved->type() == Mpi3::SongElement && elemParent->type() == Mpi3::PlaylistElement){
        if(elemParent->pid() == m_currentContainer){
            MSong *sc_songMoved = static_cast<MSong*>(elemMoved);
            MPlaylist *sc_parentPlaylist = static_cast<MPlaylist*>(elemParent);

            int fromPosition = m_currentSonglist.indexOf(sc_songMoved);
            int toPosition = sc_parentPlaylist->songs.indexOf(sc_songMoved);

            m_currentSonglist.move(fromPosition, toPosition);
        }
    }
}
void MModelSonglist::elementDeleted(const QString &pidDeleted, Mpi3::ElementType elemType, QVector<QString> pidChildren){
    if(elemType == Mpi3::SongElement){
        for(int i = 0; i < m_currentSonglist.size(); i++){
            MSong *songDeleted = m_currentSonglist.at(i);

            if(songDeleted->pid() == pidDeleted){
                removeRows(i, 1);
                m_currentSonglist.removeAll(songDeleted);
            }
        }
    }
    else if(pidDeleted == m_currentContainer || pidChildren.contains(m_currentContainer)) {
        setDisplay(MModelSonglist::DisplayNone);
    }
}
