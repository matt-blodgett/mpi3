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
QMimeData *MModelContainers::mimeData(const QModelIndexList &indexes) const{
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
    bool dataIsValidMediaFiles = data->hasUrls() ? MMediaLibrary::validMediaFiles(data->urls()) : false;
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
            return moveFolder->parentFolder() || !dropPlaylist;
        }
        else if(movePlaylist && !dropFolder){
            return movePlaylist->parentFolder() || !dropPlaylist;
        }
        else if(moveFolder && dropFolder){
            return moveFolder != dropFolder && !moveFolder->childContainers().contains(dropFolder);
        }
        else if(movePlaylist && dropFolder){
            return movePlaylist->parentFolder() != dropFolder;
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
            foreach(QUrl url, data->urls()){
                MSong *droppedSong = m_mediaLibrary->newSong(url.toString());
                m_mediaLibrary->insert(droppedSong);
            }
            return true;
        }
    }
    else if(dropElement->type() == Mpi3::PlaylistElement){
        MPlaylist *parentPlaylist = static_cast<MPlaylist*>(dropElement);

        if(actionIsCopyAction && dataIsSonglist){
            QByteArray pidBytes = data->data(QMetaType::typeName(qMetaTypeId<QStringList>()));
            foreach(MSong *song, m_mediaLibrary->songsFromBytes(pidBytes)){
                m_mediaLibrary->insert(song, parentPlaylist);
            }
            return true;
        }
        else if(actionIsCopyAction && dataIsValidMediaFiles){
            foreach(QUrl url, data->urls()){
                MSong *droppedSong = m_mediaLibrary->newSong(url.toString());
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
    foreach(QModelIndex pidx, persistentIndexList()){
        if(getPID(pidx) == pid){
            return pidx;
        }
    }

    return QModelIndex();
}
MModelItem *MModelContainers::getItem(const QModelIndex &index) const{
    if(index.isValid()){
        MModelItem *item = static_cast<MModelItem*>(index.internalPointer());
        if(item){
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
        disconnect(m_mediaLibrary, &MMediaLibrary::elementModified, this, &MModelContainers::elementModified);
        disconnect(m_mediaLibrary, &MMediaLibrary::elementInserted, this, &MModelContainers::elementInserted);
        disconnect(m_mediaLibrary, &MMediaLibrary::elementRemoved, this, &MModelContainers::elementRemoved);
        disconnect(m_mediaLibrary, &MMediaLibrary::elementMoved, this, &MModelContainers::elementMoved);
        disconnect(m_mediaLibrary, &MMediaLibrary::elementDeleted, this, &MModelContainers::elementDeleted);
    }

    m_mediaLibrary = library;

    if(m_mediaLibrary){
        connect(m_mediaLibrary, &MMediaLibrary::elementModified, this, &MModelContainers::elementModified);
        connect(m_mediaLibrary, &MMediaLibrary::elementInserted, this, &MModelContainers::elementInserted);
        connect(m_mediaLibrary, &MMediaLibrary::elementRemoved, this, &MModelContainers::elementRemoved);
        connect(m_mediaLibrary, &MMediaLibrary::elementMoved, this, &MModelContainers::elementMoved);
        connect(m_mediaLibrary, &MMediaLibrary::elementDeleted, this, &MModelContainers::elementDeleted);
    }

    removeRows(0, rowCount());
    m_libItems.clear();
    populate();

    endResetModel();
}

MFolder *MModelContainers::parentFolderAt(const QModelIndex &index) const{

    if(index.isValid()){
        MMediaContainer *container = m_mediaLibrary->getContainer(getPID(index));
        if(container->type() == Mpi3::FolderElement){
            return static_cast<MFolder*>(container);
        }
        else {
            return container->parentFolder();
        }
    }

    return nullptr;
}

void MModelContainers::populate(MFolder *parentFolder, MModelItem *parentItem){

    QVector<MFolder*> childFolders = parentFolder ? parentFolder->folders() : m_mediaLibrary->rootFolders();
    QVector<MPlaylist*> childPlaylists = parentFolder ? parentFolder->playlists() : m_mediaLibrary->rootPlaylists();

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

    parentElement = parentElement->type() == Mpi3::LibraryElement ? nullptr : parentElement;
    if(!parentElement && childPlaylist->parentFolder()){
        return;
    }

    QModelIndex parentIndex;
    MModelItem *parentItem = m_rootItem;
    int position = m_mediaLibrary->rootPlaylists().size() - 1;
    position += m_mediaLibrary->rootFolders().size();

    if(parentElement){
        if(parentElement->type() == Mpi3::FolderElement){
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
    childItem->setIcon(m_icnPlaylist);

    m_libItems[childPlaylist->pid()] = childItem;
}
void MModelContainers::folderInserted(MFolder *childFolder, MMediaElement *parentElement){

    parentElement = parentElement->type() == Mpi3::LibraryElement ? nullptr : parentElement;
    if(!parentElement && childFolder->parentFolder()){
        return;
    }

    QModelIndex parentIndex;
    MModelItem *parentItem = m_rootItem;
    int position = m_mediaLibrary->rootFolders().size() - 1;

    if(parentElement){
        if(parentElement->type() == Mpi3::FolderElement){
            MFolder *parentFolder = static_cast<MFolder*>(parentElement);
            parentIndex = getIndex(parentFolder->pid());
            parentItem = getItem(parentFolder->pid());
            position = parentFolder->folders().indexOf(childFolder);
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
void MModelContainers::elementDeleted(MMediaElement *elemDeleted){

    if(elemDeleted->type() == Mpi3::FolderElement || elemDeleted->type() == Mpi3::PlaylistElement){
        QModelIndex childIndex = getIndex(elemDeleted->pid());
        QModelIndex parentIndex = childIndex.parent();
        removeRows(childIndex.row(), 1, parentIndex);

        m_libItems.remove(elemDeleted->pid());

        if(elemDeleted->type() == Mpi3::FolderElement){
            MFolder *folder = static_cast<MFolder*>(elemDeleted);
            foreach(MMediaContainer *childContainer, folder->childContainers()){
                m_libItems.remove(childContainer->pid());
            }
        }
    }
}


MModelSonglist::MModelSonglist(QObject *parent) : QAbstractItemModel(parent){
    m_headers << "Name";
    m_headers << "Artist";
    m_headers << "Album";
    m_headers << "Time";
    m_headers << "Size";
    m_headers << "Kind";
    m_headers << "Path";
    m_headers << "Bit Rate";
    m_headers << "Sample Rate";
    m_headers << "Date Added";
    m_headers << "Date Modified";
    m_headers << "Plays";
    m_headers << "Skips";
    m_headers << "Last Played";
    m_headers << "Last Skipped";
}
MModelSonglist::~MModelSonglist(){}

Qt::ItemFlags MModelSonglist::flags(const QModelIndex &index) const{

    Qt::ItemFlags idxFlags = Qt::ItemIsDropEnabled;

    if(index.isValid()){

        idxFlags |= Qt::ItemIsEnabled;
        idxFlags |= Qt::ItemIsSelectable;
        idxFlags |= Qt::ItemNeverHasChildren;

        if(m_container->type() == Mpi3::PlaylistElement){
            idxFlags |= Qt::ItemIsDragEnabled;
        }

        if(index.column() >= 0 && index.column() <= 2){
            idxFlags |= Qt::ItemIsEditable;
        }

    }

    return idxFlags;
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
QMimeData *MModelSonglist::mimeData(const QModelIndexList &indexes) const{
    QMimeData *mData = new QMimeData();

    QVector<MSong*> selectedSongs;
    foreach(QModelIndex idx, indexes){
        MSong *song = songAt(idx);
        if(song && !selectedSongs.contains(song)){
            selectedSongs.append(song);
        }
    }

    QByteArray pidBytes = MMediaLibrary::songsToBytes(selectedSongs);
    QList<QUrl> songUrls = MMediaLibrary::songsToPaths(selectedSongs);

    mData->setData(QMetaType::typeName(qMetaTypeId<QStringList>()), pidBytes);
    mData->setUrls(songUrls);

    return mData;
}

bool MModelSonglist::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const {
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

    Mpi3::ElementType elemType = m_container->type();
    if(elemType == Mpi3::PlaylistElement || elemType == Mpi3::LibraryElement){

        bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
        bool dataIsValidMediaFiles = data->hasUrls() ? m_library->validMediaFiles(data->urls()) : false;
        bool actionIsCopyAction = action == Qt::CopyAction;

        return dataIsSonglist || (dataIsValidMediaFiles && actionIsCopyAction);
    }

    return false;
}
bool MModelSonglist::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent){
    Q_UNUSED(column);
    Q_UNUSED(parent);

    Mpi3::ElementType elemType = m_container->type();
    if(elemType == Mpi3::PlaylistElement || elemType == Mpi3::LibraryElement){

        bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
        bool dataIsValidMediaFiles = data->hasUrls() ? m_library->validMediaFiles(data->urls()) : false;
        bool actionIsCopyAction = action == Qt::CopyAction;
        bool actionIsMoveAction = action == Qt::MoveAction;

        MPlaylist *parentPlaylist = nullptr;
        if(elemType == Mpi3::PlaylistElement){
            parentPlaylist = static_cast<MPlaylist*>(m_container);
        }

        if(dataIsSonglist){
            QByteArray pidBytes = data->data(QMetaType::typeName(qMetaTypeId<QStringList>()));
            QVector<MSong*> droppedSongs = m_library->songsFromBytes(pidBytes);

            if(actionIsCopyAction){
                foreach(MSong *song, droppedSongs){
                    m_library->insert(song, parentPlaylist, row);
                }
                return true;
            }
            else if(actionIsMoveAction && elemType == Mpi3::PlaylistElement){
                foreach(MSong *song, droppedSongs){
                    m_library->move(song, parentPlaylist, row);
                }
                return true;
            }
        }
        else if(dataIsValidMediaFiles && actionIsCopyAction){
            foreach(QUrl url, data->urls()){
                MSong *droppedSong = m_library->newSong(url.toString());
                m_library->insert(droppedSong, parentPlaylist, row);
            }
            return true;
        }
    }

    return false;
}

void MModelSonglist::sort(int column, Qt::SortOrder order){
    qDebug() << column << order;
}

int MModelSonglist::rowCount(const QModelIndex &) const{
    return m_songlist.size();
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
        MSong *song = m_songlist.at(index.row());

        if(role == Qt::DisplayRole){

            switch(index.column()){
                case 0: return song->name();
                case 1: return song->artist();
                case 2: return song->album();
                case 3: return song->time_str();
                case 4: return song->size_str();
                case 5: return song->kind();
                case 6: return song->path();
                case 7: return song->bitRate();
                case 8: return song->sampleRate();
            }
        }

        else if(role == Qt::EditRole){

            switch(index.column()){
                case 0: return song->name();
                case 1: return song->artist();
                case 2: return song->album();
            }
        }
    }

    return QVariant();
}
QVariant MModelSonglist::headerData(int section, Qt::Orientation orientation, int role) const{

//    if(orientation == Qt::Horizontal && role == Qt::DecorationRole){
//        return QIcon(":/icons/treeview/folder_open_light.png");
//    }
//http://doc.qt.io/qt-5/qtwidgets-itemviews-customsortfiltermodel-example.html

    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return m_headers.at(section);
    }

    return QVariant();
}

bool MModelSonglist::setData(const QModelIndex &index, const QVariant &value, int role){

    if(index.isValid() && role == Qt::EditRole){
        MSong *song = m_songlist.at(index.row());

        if(!song || (index.column() >= 0 && index.column() <= 2)){
            return false;
        }

        MSong::MutableProperty attrib = MSong::SongName;
        if(index.column() == 0){
            attrib  = MSong::SongName;
        }
        else if(index.column() == 1){
            attrib  = MSong::SongArtist;
        }
        else if(index.column() == 2){
            attrib  = MSong::SongAlbum;
        }

        m_library->modify(song, value.toString(), attrib);
        emit dataChanged(index, index);
        return true;
    }
    return false;
}
bool MModelSonglist::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role){
    if(role == Qt::EditRole && orientation == Qt::Horizontal){
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

    if(m_library){
        disconnect(m_library, &MMediaLibrary::elementModified, this, &MModelSonglist::elementModified);
        disconnect(m_library, &MMediaLibrary::elementInserted, this, &MModelSonglist::elementInserted);
        disconnect(m_library, &MMediaLibrary::elementRemoved, this, &MModelSonglist::elementRemoved);
        disconnect(m_library, &MMediaLibrary::elementMoved, this, &MModelSonglist::elementMoved);
        disconnect(m_library, &MMediaLibrary::elementDeleted, this, &MModelSonglist::elementDeleted);
    }

    m_songlist.clear();
    m_library = library;
    m_container = nullptr;

    if(m_library){
        connect(m_library, &MMediaLibrary::elementModified, this, &MModelSonglist::elementModified);
        connect(m_library, &MMediaLibrary::elementInserted, this, &MModelSonglist::elementInserted);
        connect(m_library, &MMediaLibrary::elementRemoved, this, &MModelSonglist::elementRemoved);
        connect(m_library, &MMediaLibrary::elementMoved, this, &MModelSonglist::elementMoved);
        connect(m_library, &MMediaLibrary::elementDeleted, this, &MModelSonglist::elementDeleted);
    }

    removeRows(0, rowCount());

    endResetModel();
}
void MModelSonglist::setContainer(MMediaContainer *container){
    removeRows(0, rowCount());

    m_container = container;
    m_songlist = container->songs();

    insertRows(0, rowCount());
}

MMediaLibrary *MModelSonglist::library() const {
    return m_library;
}
MMediaContainer *MModelSonglist::container() const {
    return m_container;
}
QVector<MSong*> MModelSonglist::songlist() const {
    return m_songlist;
}
MSong *MModelSonglist::songAt(const QModelIndex &index) const{
    if(index.row() > -1 && index.row() < m_songlist.size()){
        return m_songlist.at(index.row());
    }

    return nullptr;
}

void MModelSonglist::elementModified(MMediaElement *elemModified){

    if(elemModified->type() == Mpi3::SongElement){
        MSong *sc_song = static_cast<MSong*>(elemModified);

        if(m_songlist.contains(sc_song)){
            int pos = m_songlist.indexOf(sc_song);
            emit dataChanged(index(pos, 0), index(pos, columnCount()));
        }
    }
}
void MModelSonglist::elementInserted(MMediaElement *elemInserted, MMediaContainer *elemParent){
    Q_UNUSED(elemParent)

    if(elemInserted->type() == Mpi3::SongElement && elemParent == m_container){
        MSong *sc_song = static_cast<MSong*>(elemInserted);

        if(!m_songlist.contains(sc_song) && m_container->songs().contains(sc_song)){
            int pos = m_container->songs().indexOf(sc_song);
            m_songlist.insert(pos, sc_song);
            insertRows(pos, 1);
        }
    }
}
void MModelSonglist::elementRemoved(MMediaElement *elemRemoved, MMediaContainer *elemParent){
    Q_UNUSED(elemParent)

    if(elemRemoved->type() == Mpi3::SongElement && elemParent == m_container){
        MSong *sc_song = static_cast<MSong*>(elemRemoved);

        if(m_songlist.contains(sc_song) && !m_container->songs().contains(sc_song)){
            removeRows(m_songlist.indexOf(sc_song), 1);
            m_songlist.removeAll(sc_song);
        }
    }
}
void MModelSonglist::elementMoved(MMediaElement *elemMoved, MMediaContainer *elemParent){

    if(elemMoved->type() == Mpi3::SongElement && elemParent == m_container){
        MSong *sc_song = static_cast<MSong*>(elemMoved);

        int fromPosition = m_songlist.indexOf(sc_song);
        int toPosition = elemParent->songs().indexOf(sc_song);

        m_songlist.move(fromPosition, toPosition);
    }
}
void MModelSonglist::elementDeleted(MMediaElement *elemDeleted){

    if(elemDeleted->type() == Mpi3::SongElement){
        MSong *sc_song = static_cast<MSong*>(elemDeleted);
        if(m_songlist.contains(sc_song)){
            removeRows(m_songlist.indexOf(sc_song), 1);
            m_songlist.removeAll(sc_song);
        }
    }
}
