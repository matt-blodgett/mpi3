#include "mlibmodel.h"
#include "mlibitem.h"

#include "util/mlibrary.h"

#include <QMimeData>
#include <QPixmap>
#include <QUrl>

#include <QDebug>


Mpi3ModelContainers::Mpi3ModelContainers(QObject *parent) : QAbstractItemModel(parent){
    m_rootItem = new LibraryItem();

    m_rootItem->setData(0, "Playlists");
    insertColumns(0, 1);

    m_icnFolder.addPixmap(QPixmap(":/icons/treeview/folder.png"));
    m_icnPlaylist.addPixmap(QPixmap(":/icons/treeview/playlist.png"));
}
Mpi3ModelContainers::~Mpi3ModelContainers(){
    delete m_rootItem;
}

Qt::ItemFlags Mpi3ModelContainers::flags(const QModelIndex &index) const{
    if(index.isValid()){
        return Qt::ItemIsEditable
                | Qt::ItemIsDragEnabled
                | Qt::ItemIsDropEnabled
                | Qt::ItemIsSelectable
                | Qt::ItemIsEnabled;
    }

    return Qt::ItemIsDropEnabled;
}
Qt::DropActions Mpi3ModelContainers::supportedDragActions() const{
    return Qt::MoveAction;
}
Qt::DropActions Mpi3ModelContainers::supportedDropActions() const{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList Mpi3ModelContainers::mimeTypes() const {
    QStringList mTypes;
    mTypes << QMetaType::typeName(qMetaTypeId<QString>());
    return mTypes;
}
QMimeData* Mpi3ModelContainers::mimeData(const QModelIndexList &indexes) const{
    QMimeData *mData = new QMimeData();
    if(indexes.size() == 1){
        QByteArray pidBytes;
        pidBytes.append(getPID(indexes.at(0)));
        mData->setData(QMetaType::typeName(qMetaTypeId<QString>()), pidBytes);
    }

    return mData;
}

bool Mpi3ModelContainers::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const {

    bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
    bool dataIsContainer = data->hasFormat(QMetaType::typeName(qMetaTypeId<QString>()));
    bool dataIsValidMediaFiles = data->hasUrls() ? m_mediaLibrary->validMediaFiles(data->urls()) : false;
    bool actionIsCopyAction = action == Qt::CopyAction;
    bool actionIsMoveAction = action == Qt::MoveAction;

    QModelIndex dropIndex = index(row, column, parent);

    if(actionIsMoveAction && dataIsContainer){
        QString pid = data->data(QMetaType::typeName(qMetaTypeId<QString>()));

        Mpi3Folder *moveFolder = m_mediaLibrary->getFolder(pid);
        Mpi3Playlist *movePlaylist = m_mediaLibrary->getPlaylist(pid);

        Mpi3Folder *dropFolder = m_mediaLibrary->getFolder(getPID(dropIndex));
        Mpi3Playlist *dropPlaylist = m_mediaLibrary->getPlaylist(getPID(dropIndex));

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

    Mpi3Element *dropElement = m_mediaLibrary->getElement(getPID(dropIndex));

    if(!dropElement){
        return (actionIsCopyAction && dataIsValidMediaFiles);
    }
    else if(dropElement->type() == Mpi3Element::PlaylistElement){
        return actionIsCopyAction && (dataIsSonglist || dataIsValidMediaFiles);
    }

    return false;
}
bool Mpi3ModelContainers::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent){

    bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
    bool dataIsContainer = data->hasFormat(QMetaType::typeName(qMetaTypeId<QString>()));
    bool dataIsValidMediaFiles = data->hasUrls() ? m_mediaLibrary->validMediaFiles(data->urls()) : false;
    bool actionIsCopyAction = action == Qt::CopyAction;
    bool actionIsMoveAction = action == Qt::MoveAction;

    QModelIndex dropIndex = index(row, column, parent);
    Mpi3Element *dropElement = m_mediaLibrary->getElement(getPID(dropIndex));

    if(!dropElement){
        if(actionIsMoveAction && dataIsContainer){
            QString pid = data->data(QMetaType::typeName(qMetaTypeId<QString>()));
            Mpi3Element *moveElement = m_mediaLibrary->getElement(pid);

            if(!moveElement){
                return false;
            }
            else if(moveElement->type() == Mpi3Element::PlaylistElement){
                Mpi3Playlist *movePlaylist = static_cast<Mpi3Playlist*>(moveElement);
                m_mediaLibrary->move(movePlaylist, nullptr);
                return true;
            }
            else if(moveElement->type() == Mpi3Element::FolderElement){
                Mpi3Folder *moveFolder = static_cast<Mpi3Folder*>(moveElement);
                m_mediaLibrary->move(moveFolder, nullptr);
                return true;
            }
            else {
                return false;
            }
        }
        else if(actionIsCopyAction && dataIsValidMediaFiles){
            for(int i = 0; i < data->urls().size(); i++){
                Mpi3Song *droppedSong = m_mediaLibrary->newSong(data->urls().at(i).toString());
                m_mediaLibrary->insert(droppedSong);
            }
            return true;
        }
    }
    else if(dropElement->type() == Mpi3Element::PlaylistElement){
        Mpi3Playlist *parentPlaylist = static_cast<Mpi3Playlist*>(dropElement);

        if(actionIsCopyAction && dataIsSonglist){
            QByteArray pidBytes = data->data(QMetaType::typeName(qMetaTypeId<QStringList>()));
            QVector<Mpi3Song*> droppedSongs = m_mediaLibrary->songsFromData(pidBytes);

            for(int i = 0; i < droppedSongs.size(); i++){
                m_mediaLibrary->insert(droppedSongs.at(i), parentPlaylist);
            }

            return true;
        }
        else if(actionIsCopyAction && dataIsValidMediaFiles){
            for(int i = 0; i < data->urls().size(); i++){
                Mpi3Song *droppedSong = m_mediaLibrary->newSong(data->urls().at(i).toString());
                m_mediaLibrary->insert(droppedSong, parentPlaylist);
            }
            return true;
        }
    }
    else if(dropElement->type() == Mpi3Element::FolderElement){
        if(actionIsMoveAction && dataIsContainer){
            Mpi3Folder *parentFolder = static_cast<Mpi3Folder*>(dropElement);

            QString pid = data->data(QMetaType::typeName(qMetaTypeId<QString>()));
            Mpi3Element *moveElement = m_mediaLibrary->getElement(pid);

            if(!moveElement){
                return false;
            }
            else if(moveElement->type() == Mpi3Element::PlaylistElement){
                Mpi3Playlist *movePlaylist = static_cast<Mpi3Playlist*>(moveElement);
                m_mediaLibrary->move(movePlaylist, parentFolder);
                return true;
            }
            else if(moveElement->type() == Mpi3Element::FolderElement){
                Mpi3Folder *moveFolder = static_cast<Mpi3Folder*>(moveElement);
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

int Mpi3ModelContainers::rowCount(const QModelIndex &parent) const{
    LibraryItem *parentItem = getItem(parent);
    return parentItem->childCount();
}
int Mpi3ModelContainers::columnCount(const QModelIndex &) const{
    return m_rootItem->columnCount();
}

QModelIndex Mpi3ModelContainers::index(int row, int column, const QModelIndex &parent) const{
    LibraryItem *parentItem = getItem(parent);
    LibraryItem *childItem = parentItem->child(row);
    if (childItem){
        return createIndex(row, column, childItem);
    }
    else{
        return QModelIndex();
    }
}
QModelIndex Mpi3ModelContainers::parent(const QModelIndex &index) const{
    if (!index.isValid()){
        return QModelIndex();
    }

    LibraryItem *childItem = getItem(index);
    LibraryItem *parentItem = childItem->parent();
    if (parentItem == m_rootItem){
        return QModelIndex();
    }

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

QVariant Mpi3ModelContainers::data(const QModelIndex &index, int role) const{
    if (!index.isValid()){
        return QVariant();
    }

//    Qt::FontRole
//    Qt::TextAlignmentRole
//    Qt::BackgroundRole
//    Qt::ForegroundRole
//    Qt::CheckStateRole

    LibraryItem *item = getItem(index);
    if(role == Qt::DecorationRole && index.column() == 0){
        return item->icon();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole){
        return item->data(index.column());
    }

    return QVariant();
}
QVariant Mpi3ModelContainers::headerData(int section, Qt::Orientation orientation, int role) const{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return m_rootItem->data(section);
    }

    return QVariant();
}

bool Mpi3ModelContainers::setData(const QModelIndex &index, const QVariant &value, int role){
    if (role == Qt::EditRole){
        LibraryItem *item = getItem(index);
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
bool Mpi3ModelContainers::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role){
    if (role != Qt::EditRole || orientation != Qt::Horizontal){
        return false;
    }

    bool result = m_rootItem->setData(section, value);

    if (result){
        emit headerDataChanged(orientation, section, section);
    }

    return result;
}

bool Mpi3ModelContainers::insertRows(int position, int count, const QModelIndex &parent){
    if(count > 0){
        LibraryItem *parentItem = getItem(parent);

        beginInsertRows(parent, position, position + count - 1);
        bool success = parentItem->insertChildren(position, count, m_rootItem->columnCount());
        endInsertRows();

        return success;
    }

    return false;
}
bool Mpi3ModelContainers::insertColumns(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginInsertColumns(parent, position, position + count - 1);
        bool success = m_rootItem->insertColumns(position, count);
        endInsertColumns();

        return success;
    }

    return false;
}

bool Mpi3ModelContainers::removeRows(int position, int count, const QModelIndex &parent){
    if(count > 0){
        LibraryItem *parentItem = getItem(parent);

        beginRemoveRows(parent, position, position + count - 1);
        bool success = parentItem->removeChildren(position, count);
        endRemoveRows();

        return success;
    }

    return false;
}
bool Mpi3ModelContainers::removeColumns(int position, int count, const QModelIndex &parent){
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

QModelIndex Mpi3ModelContainers::getIndex(const QString &pid) const{
    QModelIndexList pIndexes = persistentIndexList();
    for(int i = 0; i < pIndexes.size(); i++){
        QModelIndex idx = pIndexes.at(i);
        if(getPID(idx) == pid){
            return idx;
        }
    }

    return QModelIndex();
}
LibraryItem *Mpi3ModelContainers::getItem(const QModelIndex &index) const{
    if(index.isValid()){
        LibraryItem *item = static_cast<LibraryItem*>(index.internalPointer());
        if (item){
            return item;
        }
    }

    return m_rootItem;
}
LibraryItem *Mpi3ModelContainers::getItem(const QString &pid) const{
    return !m_libItems.keys().contains(pid) ? m_rootItem : m_libItems[pid];
}
QString Mpi3ModelContainers::getPID(const QModelIndex &index) const{
    return getPID(getItem(index));
}
QString Mpi3ModelContainers::getPID(LibraryItem *item) const{
    if(!item || !m_libItems.values().contains(item)){
        return QString();
    }

    return m_libItems.key(item);
}

Mpi3Folder* Mpi3ModelContainers::getParentFolderAt(const QModelIndex &index) const{
    Mpi3Folder *parentFolder = nullptr;

    if(index.isValid()){
        Mpi3Element *element = m_mediaLibrary->getElement(getPID(index));

        if(!element){
            parentFolder = nullptr;
        }
        else if(element->type() == Mpi3Element::FolderElement){
            parentFolder = static_cast<Mpi3Folder*>(element);
        }
        else if(element->type() == Mpi3Element::PlaylistElement){
            parentFolder = static_cast<Mpi3Playlist*>(element)->parent;
        }
    }

    return parentFolder;
}

void Mpi3ModelContainers::setLibrary(Mpi3Library *library){
    beginResetModel();

    if(m_mediaLibrary){
        disconnect(m_mediaLibrary.data(), &Mpi3Library::elementModified, this, &Mpi3ModelContainers::elementModified);
        disconnect(m_mediaLibrary.data(), &Mpi3Library::elementInserted, this, &Mpi3ModelContainers::elementInserted);
        disconnect(m_mediaLibrary.data(), &Mpi3Library::elementRemoved, this, &Mpi3ModelContainers::elementRemoved);
        disconnect(m_mediaLibrary.data(), &Mpi3Library::elementMoved, this, &Mpi3ModelContainers::elementMoved);
        disconnect(m_mediaLibrary.data(), &Mpi3Library::elementDeleted, this, &Mpi3ModelContainers::elementDeleted);
    }

    m_mediaLibrary.take();
    m_mediaLibrary.reset(library);

    if(m_mediaLibrary){
        connect(m_mediaLibrary.data(), &Mpi3Library::elementModified, this, &Mpi3ModelContainers::elementModified);
        connect(m_mediaLibrary.data(), &Mpi3Library::elementInserted, this, &Mpi3ModelContainers::elementInserted);
        connect(m_mediaLibrary.data(), &Mpi3Library::elementRemoved, this, &Mpi3ModelContainers::elementRemoved);
        connect(m_mediaLibrary.data(), &Mpi3Library::elementMoved, this, &Mpi3ModelContainers::elementMoved);
        connect(m_mediaLibrary.data(), &Mpi3Library::elementDeleted, this, &Mpi3ModelContainers::elementDeleted);
    }

    removeRows(0, rowCount());
    m_libItems.clear();
    populate();

    endResetModel();
}

void Mpi3ModelContainers::populate(Mpi3Folder *parentFolder, LibraryItem *parentItem){
    QVector<Mpi3Folder*> childFolders = parentFolder ? parentFolder->folders : m_mediaLibrary->childFolders();
    QVector<Mpi3Playlist*> childPlaylists = parentFolder ? parentFolder->playlists : m_mediaLibrary->childPlaylists();

    if(!parentItem){
        parentItem = m_rootItem;
    }

    parentItem->insertChildren(0, childFolders.size() + childPlaylists.size(), columnCount());
    for(int i = 0; i < childFolders.size(); i++){
        Mpi3Folder *childFolder = childFolders.at(i);
        LibraryItem *childItem = parentItem->child(i);

        childItem->setData(0, childFolder->name());
        childItem->setIcon(m_icnFolder);

        m_libItems[childFolder->pid()] = childItem;

        populate(childFolder, childItem);
    }

    for(int i = 0; i < childPlaylists.size(); i++){
        Mpi3Playlist *childPlaylist = childPlaylists.at(i);
        LibraryItem *childItem = parentItem->child(i + childFolders.size());

        childItem->setData(0, childPlaylist->name());
        childItem->setIcon(m_icnPlaylist);

        m_libItems[childPlaylist->pid()] = childItem;
    }
}

void Mpi3ModelContainers::playlistInserted(Mpi3Playlist *childPlaylist, Mpi3Element *parentElement){
    if(!parentElement && childPlaylist->parent){
        return;
    }

    QModelIndex parentIndex;
    LibraryItem *parentItem = m_rootItem;
    int position = m_mediaLibrary->childPlaylists().size() - 1;
    position += m_mediaLibrary->childFolders().size();

    if(parentElement){
        if(parentElement->type() == Mpi3Element::FolderElement){
            Mpi3Folder *parentFolder = static_cast<Mpi3Folder*>(parentElement);
            parentIndex = getIndex(parentFolder->pid());
            parentItem = getItem(parentFolder->pid());
            position = parentFolder->playlists.indexOf(childPlaylist);
            position += parentFolder->folders.size();
        }
    }

    insertRows(position, 1, parentIndex);

    LibraryItem *childItem = parentItem->child(position);
    childItem->setData(0, childPlaylist->name());
    childItem->setIcon(m_icnPlaylist);

    m_libItems[childPlaylist->pid()] = childItem;
}
void Mpi3ModelContainers::folderInserted(Mpi3Folder *childFolder, Mpi3Element *parentElement){
    if(!parentElement && childFolder->parent){
        return;
    }

    QModelIndex parentIndex;
    LibraryItem *parentItem = m_rootItem;
    int position = m_mediaLibrary->childFolders().size() - 1;

    if(parentElement){
        if(parentElement->type() == Mpi3Element::FolderElement){
            Mpi3Folder *parentFolder = static_cast<Mpi3Folder*>(parentElement);

            parentIndex = getIndex(parentFolder->pid());
            parentItem = getItem(parentFolder->pid());
            position = parentFolder->folders.indexOf(childFolder);
        }
    }

    insertRows(position, 1, parentIndex);

    LibraryItem *childItem = parentItem->child(position);
    childItem->setData(0, childFolder->name());
    childItem->setIcon(m_icnFolder);

    m_libItems[childFolder->pid()] = childItem;

    populate(childFolder, childItem);
}

void Mpi3ModelContainers::elementModified(Mpi3Element *elemModified){
    if(m_libItems.keys().contains(elemModified->pid())){
        m_libItems[elemModified->pid()]->setData(0, elemModified->name());

        QModelIndex changedIndex = getIndex(elemModified->pid());
        emit dataChanged(changedIndex, changedIndex);
    }
}
void Mpi3ModelContainers::elementInserted(Mpi3Element *elemInserted, Mpi3Element *elemParent){
    if(elemInserted->type() == Mpi3Element::PlaylistElement){
        Mpi3Playlist *sc_insertedPlaylist = static_cast<Mpi3Playlist*>(elemInserted);
        playlistInserted(sc_insertedPlaylist, elemParent);
    }
    else if(elemInserted->type() == Mpi3Element::FolderElement){
        Mpi3Folder *sc_insertedFolder = static_cast<Mpi3Folder*>(elemInserted);
        folderInserted(sc_insertedFolder, elemParent);
    }
}
void Mpi3ModelContainers::elementRemoved(Mpi3Element *elemRemoved, Mpi3Element *elemParent){
    if(elemRemoved->type() == Mpi3Element::FolderElement || elemRemoved->type() == Mpi3Element::PlaylistElement) {
        QModelIndex parentIndex = getIndex(elemParent->pid());
        QModelIndex childIndex = getIndex(elemRemoved->pid());

        if(childIndex.isValid()){
            removeRows(childIndex.row(), 1, parentIndex);
            m_libItems.remove(elemRemoved->pid());
        }
    }
}
void Mpi3ModelContainers::elementMoved(Mpi3Element *elemMoved, Mpi3Element *elemParent){
    QModelIndex moveIndex = getIndex(elemMoved->pid());
    QModelIndex sourceIndex = moveIndex.parent();
    removeRows(moveIndex.row(), 1, sourceIndex);

    if(elemMoved->type() == Mpi3Element::PlaylistElement){
        Mpi3Playlist *sc_movedPlaylist = static_cast<Mpi3Playlist*>(elemMoved);
        playlistInserted(sc_movedPlaylist, elemParent);
    }
    else if(elemMoved->type() == Mpi3Element::FolderElement){
        Mpi3Folder *sc_movedFolder = static_cast<Mpi3Folder*>(elemMoved);
        folderInserted(sc_movedFolder, elemParent);
    }
}
void Mpi3ModelContainers::elementDeleted(const QString &pidDeleted, int elemType, QVector<QString> pidChildren){
    if(elemType == Mpi3Element::FolderElement || elemType == Mpi3Element::PlaylistElement){
        QModelIndex childIndex = getIndex(pidDeleted);
        QModelIndex parentIndex = getIndex(getPID(getItem(pidDeleted)->parent()));
        removeRows(childIndex.row(), 1, parentIndex);

        m_libItems.remove(pidDeleted);
        if(elemType == Mpi3Element::FolderElement){
            for(int i = 0; i < pidChildren.size(); i++){
                m_libItems.remove(pidChildren.at(i));
            }
        }
    }
}


Mpi3ModelSonglist::Mpi3ModelSonglist(QObject *parent) : QAbstractItemModel(parent){
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
Mpi3ModelSonglist::~Mpi3ModelSonglist(){}

Qt::ItemFlags Mpi3ModelSonglist::flags(const QModelIndex &index) const{
    if(index.isValid()){
        if(m_currentDisplay == Mpi3ModelSonglist::DisplayPlaylist){
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
Qt::DropActions Mpi3ModelSonglist::supportedDragActions() const{
    return Qt::CopyAction;
}
Qt::DropActions Mpi3ModelSonglist::supportedDropActions() const{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList Mpi3ModelSonglist::mimeTypes() const {
    QStringList mTypes;
    mTypes << QMetaType::typeName(qMetaTypeId<QStringList>());
    return mTypes;
}
QMimeData* Mpi3ModelSonglist::mimeData(const QModelIndexList &indexes) const{
    QMimeData *mData = new QMimeData();

    QVector<Mpi3Song*> selectedSongs;
    for(int i = 0; i < indexes.size(); i++){
        Mpi3Song *song = getSongAt(indexes.at(i));
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

bool Mpi3ModelSonglist::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const {
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

    bool displayIsPlaylist = m_currentDisplay == Mpi3ModelSonglist::DisplayPlaylist;
    bool displayIsAllSongs = m_currentDisplay == Mpi3ModelSonglist::DisplayAllSongs;

    if(displayIsPlaylist || displayIsAllSongs){
        bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
        bool dataIsValidMediaFiles = data->hasUrls() ? m_mediaLibrary->validMediaFiles(data->urls()) : false;
        bool actionIsCopyAction = action == Qt::CopyAction;

        return dataIsSonglist || (dataIsValidMediaFiles && actionIsCopyAction);
    }

    return false;
}
bool Mpi3ModelSonglist::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent){
    Q_UNUSED(column);
    Q_UNUSED(parent);

    bool displayIsPlaylist = m_currentDisplay == Mpi3ModelSonglist::DisplayPlaylist;
    bool displayIsAllSongs = m_currentDisplay == Mpi3ModelSonglist::DisplayAllSongs;

    if(displayIsPlaylist || displayIsAllSongs){
        bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
        bool dataIsValidMediaFiles = data->hasUrls() ? m_mediaLibrary->validMediaFiles(data->urls()) : false;
        bool actionIsCopyAction = action == Qt::CopyAction;
        bool actionIsMoveAction = action == Qt::MoveAction;

        if(dataIsSonglist){
            QByteArray pidBytes = data->data(QMetaType::typeName(qMetaTypeId<QStringList>()));
            QVector<Mpi3Song*> droppedSongs = m_mediaLibrary->songsFromData(pidBytes);
            Mpi3Playlist *parentPlaylist = m_mediaLibrary->getPlaylist(m_currentContainer);

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
            Mpi3Playlist *parentPlaylist = m_mediaLibrary->getPlaylist(m_currentContainer);

            for(int i = 0; i < data->urls().size(); i++){
                Mpi3Song *droppedSong = m_mediaLibrary->newSong(data->urls().at(i).toString());
                m_mediaLibrary->insert(droppedSong, parentPlaylist, row);
            }
            return true;
        }
    }

    return false;
}

int Mpi3ModelSonglist::rowCount(const QModelIndex &) const{
    return m_currentSonglist.size();
}
int Mpi3ModelSonglist::columnCount(const QModelIndex &) const{
    return m_headers.size();
}

QModelIndex Mpi3ModelSonglist::index(int row, int column, const QModelIndex &) const{
    return createIndex(row, column);
}
QModelIndex Mpi3ModelSonglist::parent(const QModelIndex &) const{
    return QModelIndex();
}

QVariant Mpi3ModelSonglist::data(const QModelIndex &index, int role) const{
//    Qt::FontRole
//    Qt::TextAlignmentRole
//    Qt::BackgroundRole
//    Qt::ForegroundRole
//    Qt::CheckStateRole

    if(index.isValid()){
        if(role == Qt::DisplayRole || role == Qt::EditRole){
            Mpi3Song *song = m_currentSonglist.at(index.row());
            switch(index.column()){
                case 0: return song->name();
                case 1: return song->artist();
                case 2: return song->path();
            }
        }
    }

    return QVariant();
}
QVariant Mpi3ModelSonglist::headerData(int section, Qt::Orientation orientation, int role) const{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return m_headers.at(section);
    }
    return QVariant();
}

bool Mpi3ModelSonglist::setData(const QModelIndex &index, const QVariant &value, int role){
    if(index.isValid() && role == Qt::EditRole){
        Mpi3Song *song = m_currentSonglist.at(index.row());
        if(song && index.column() == 0){
             m_mediaLibrary->modify(song, value.toString(), Mpi3Song::SongName);

             emit dataChanged(index, index);
             return true;
        }
    }
    return false;
}
bool Mpi3ModelSonglist::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role){
    if (role == Qt::EditRole && orientation == Qt::Horizontal){
        m_headers[section] = value.toString();
        emit headerDataChanged(orientation, section, section);
        return true;
    }

    return false;
}

bool Mpi3ModelSonglist::insertRows(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginInsertRows(parent, position, position + count - 1);
        endInsertRows();
        return true;
    }
    return false;
}
bool Mpi3ModelSonglist::insertColumns(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginInsertColumns(parent, position, position + count - 1);
        endInsertColumns();
        return true;
    }
    return false;
}

bool Mpi3ModelSonglist::removeRows(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginRemoveRows(parent, position, position + count - 1);
        endRemoveRows();
        return true;
    }
    return false;
}
bool Mpi3ModelSonglist::removeColumns(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginRemoveColumns(parent, position, position + count - 1);
        endRemoveColumns();
        return true;
    }
    return false;
}

Mpi3Song* Mpi3ModelSonglist::getSongAt(const QModelIndex &index) const{
    if(index.row() > -1 && index.row() < m_currentSonglist.size()){
        return m_currentSonglist.at(index.row());
    }

    return nullptr;
}

void Mpi3ModelSonglist::setLibrary(Mpi3Library *library){
    beginResetModel();

    if(m_mediaLibrary){
        disconnect(m_mediaLibrary.data(), &Mpi3Library::elementModified, this, &Mpi3ModelSonglist::elementModified);
        disconnect(m_mediaLibrary.data(), &Mpi3Library::elementInserted, this, &Mpi3ModelSonglist::elementInserted);
        disconnect(m_mediaLibrary.data(), &Mpi3Library::elementRemoved, this, &Mpi3ModelSonglist::elementRemoved);
        disconnect(m_mediaLibrary.data(), &Mpi3Library::elementMoved, this, &Mpi3ModelSonglist::elementMoved);
        disconnect(m_mediaLibrary.data(), &Mpi3Library::elementDeleted, this, &Mpi3ModelSonglist::elementDeleted);
    }

    m_mediaLibrary.take();
    m_mediaLibrary.reset(library);

    if(m_mediaLibrary){
        connect(m_mediaLibrary.data(), &Mpi3Library::elementModified, this, &Mpi3ModelSonglist::elementModified);
        connect(m_mediaLibrary.data(), &Mpi3Library::elementInserted, this, &Mpi3ModelSonglist::elementInserted);
        connect(m_mediaLibrary.data(), &Mpi3Library::elementRemoved, this, &Mpi3ModelSonglist::elementRemoved);
        connect(m_mediaLibrary.data(), &Mpi3Library::elementMoved, this, &Mpi3ModelSonglist::elementMoved);
        connect(m_mediaLibrary.data(), &Mpi3Library::elementDeleted, this, &Mpi3ModelSonglist::elementDeleted);
    }

    removeRows(0, rowCount());

    m_currentSonglist.clear();
    m_currentContainer = QString();
    m_currentDisplay = Mpi3ModelSonglist::DisplayNone;

    endResetModel();
}

QString Mpi3ModelSonglist::currentContainer() const {
    return m_currentContainer;
}
Mpi3ModelSonglist::Display Mpi3ModelSonglist::currentDisplay() const{
    return m_currentDisplay;
}
void Mpi3ModelSonglist::setDisplay(Mpi3ModelSonglist::Display display){
    m_currentDisplay = display;

    beginResetModel();
    removeRows(0, rowCount());
    m_currentSonglist.clear();

    switch(m_currentDisplay){
        case Mpi3ModelSonglist::DisplayNone: {
            m_currentContainer = QString();
            break;
        }
        case Mpi3ModelSonglist::DisplayAllSongs: {
            m_currentContainer = m_mediaLibrary->pid();
            m_currentSonglist = *m_mediaLibrary->libSongs;
            break;
        }
        case Mpi3ModelSonglist::DisplayArtists: {
            m_currentContainer = m_mediaLibrary->pid();
            break;
        }
        case Mpi3ModelSonglist::DisplayAlbums: {
            m_currentContainer = m_mediaLibrary->pid();
            break;
        }
        case Mpi3ModelSonglist::DisplayPlaylist: {
            Mpi3Playlist *playlist = m_mediaLibrary->getPlaylist(m_currentContainer);
            if(playlist){
                m_currentSonglist = playlist->songs;
            }
            break;
        }
        case Mpi3ModelSonglist::DisplayFolder: {
            Mpi3Folder *folder = m_mediaLibrary->getFolder(m_currentContainer);
            if(folder){
                m_currentSonglist = m_mediaLibrary->allChildSongs(folder);
            }
            break;
        };
    }

    insertRows(0, m_currentSonglist.size());
    endResetModel();
}
void Mpi3ModelSonglist::setDisplay(Mpi3Element *container){
    if(container->type() == Mpi3Element::PlaylistElement){
        m_currentContainer = container->pid();
        setDisplay(Mpi3ModelSonglist::DisplayPlaylist);
    }
    else if(container->type() == Mpi3Element::FolderElement){
        m_currentContainer = container->pid();
        setDisplay(Mpi3ModelSonglist::DisplayFolder);
    }
}

void Mpi3ModelSonglist::elementModified(Mpi3Element *elemModified){
    if(elemModified->type() == Mpi3Element::SongElement){
        Mpi3Song *sc_songModified = static_cast<Mpi3Song*>(elemModified);

        if(m_currentSonglist.contains(sc_songModified)){
            int position = m_currentSonglist.indexOf(sc_songModified);
            emit dataChanged(index(position, 0), index(position, columnCount()));
        }
    }
}
void Mpi3ModelSonglist::elementInserted(Mpi3Element *elemInserted, Mpi3Element *elemParent){
    if(elemInserted->type() == Mpi3Element::SongElement){
        Mpi3Song *sc_songInserted = static_cast<Mpi3Song*>(elemInserted);

        if(!elemParent){
            if(m_currentDisplay == Mpi3ModelSonglist::DisplayAllSongs){
                m_currentSonglist.push_back(sc_songInserted);
                insertRows(rowCount(), 1);
            }
        }
        else if(elemParent->type() == Mpi3Element::PlaylistElement && elemParent->pid() == m_currentContainer){
            Mpi3Playlist *sc_currentPlaylist = static_cast<Mpi3Playlist*>(elemParent);

            int position = sc_currentPlaylist->songs.indexOf(sc_songInserted);
            m_currentSonglist.insert(position, sc_songInserted);
            insertRows(position, 1);
        }
    }
}
void Mpi3ModelSonglist::elementRemoved(Mpi3Element *elemRemoved, Mpi3Element *elemParent){
    if(elemRemoved->type() == Mpi3Element::SongElement){
        Mpi3Song *sc_songRemoved = static_cast<Mpi3Song*>(elemRemoved);

        if(elemParent->type() == Mpi3Element::PlaylistElement && elemParent->pid() == m_currentContainer){
            removeRows(m_currentSonglist.indexOf(sc_songRemoved), 1);
            m_currentSonglist.removeAll(sc_songRemoved);
        }
    }
}
void Mpi3ModelSonglist::elementMoved(Mpi3Element *elemMoved, Mpi3Element *elemParent){
    if(elemMoved->type() == Mpi3Element::SongElement && elemParent->type() == Mpi3Element::PlaylistElement){
        if(elemParent->pid() == m_currentContainer){
            Mpi3Song *sc_songMoved = static_cast<Mpi3Song*>(elemMoved);
            Mpi3Playlist *sc_parentPlaylist = static_cast<Mpi3Playlist*>(elemParent);

            int fromPosition = m_currentSonglist.indexOf(sc_songMoved);
            int toPosition = sc_parentPlaylist->songs.indexOf(sc_songMoved);

            m_currentSonglist.move(fromPosition, toPosition);
        }
    }
}
void Mpi3ModelSonglist::elementDeleted(const QString &pidDeleted, int elemType, QVector<QString> pidChildren){
    if(elemType == Mpi3Element::SongElement){
        for(int i = 0; i < m_currentSonglist.size(); i++){
            Mpi3Song *songDeleted = m_currentSonglist.at(i);

            if(songDeleted->pid() == pidDeleted){
                removeRows(i, 1);
                m_currentSonglist.removeAll(songDeleted);
            }
        }
    }
    else if(pidDeleted == m_currentContainer || pidChildren.contains(m_currentContainer)) {
        setDisplay(Mpi3ModelSonglist::DisplayNone);
    }
}
