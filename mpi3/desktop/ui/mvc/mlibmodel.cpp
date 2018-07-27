#include "mlibmodel.h"
#include "mlibitem.h"
#include "util/mpi3library.h"

#include <QMimeData>
#include <QPixmap>
#include <QUrl>

#include <QDebug>


bool validMediaFiles(const QMimeData *data){
    if(!data->hasUrls()){
        return false;
    }
    for(int i = 0; i < data->urls().size(); i++){
        QUrl file = data->urls().at(i);
        if(!file.toString().endsWith(".mp3") && !file.toString().endsWith(".wav")){
            return false;
        }
    }

    return true;
}


LibraryModel::LibraryModel(QObject *parent) : QAbstractItemModel(parent){
    m_rootItem = new LibraryItem();

    insertColumns(0, 1);
    m_rootItem->setData(0, "Playlists");

    QPixmap pix_folder(":/icons/treeview/folder.png");
    QPixmap pix_playlist(":/icons/treeview/playlist.png");

    icn_folder.addPixmap(pix_folder);
    icn_playlist.addPixmap(pix_playlist);
}
LibraryModel::~LibraryModel(){
    delete m_rootItem;
}

Qt::ItemFlags LibraryModel::flags(const QModelIndex &index) const{
    if(!index.isValid()){
        return Qt::ItemIsDropEnabled;
    }

    return Qt::ItemIsEditable
            | Qt::ItemIsDragEnabled
            | Qt::ItemIsDropEnabled
            | QAbstractItemModel::flags(index);
}
Qt::DropActions LibraryModel::supportedDragActions() const{
    return Qt::MoveAction;
}
Qt::DropActions LibraryModel::supportedDropActions() const{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList LibraryModel::mimeTypes() const {
    QStringList mTypes;
    mTypes << "text/plain";
    return mTypes;
}
QMimeData* LibraryModel::mimeData(const QModelIndexList &indexes) const{
    QMimeData *mimeData = new QMimeData();
    if(indexes.size() == 1){
        QString pid = getPID(indexes.at(0));
        mimeData->setText(pid);
    }
    return mimeData;
}

bool LibraryModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const {

    if(action == Qt::CopyAction && validMediaFiles(data)){

//        qDebug() << row << column << parent.row() << parent.column();

//        if(row == 0 && column == 0 && parent.row() == -1 && parent.column() == -1){
//            return true;
//        }

        if(row == -1 && column == -1 && parent.row() == -1 && parent.column() == -1){
            return true;
        }

        LibraryItem *parentItem = getItem(parent);
        LibraryItem *childItem = nullptr;

//        if(row == -1 && column == -1){
//            childItem = m_rootItem->child(parent.row());
//        }
//        else{
//            childItem = parentItem->child(row);
//        }

        if(row == -1 && column == -1){
            childItem = m_rootItem->child(parent.row());
        }
        else if(parent.row() == -1 && parent.column() == -1){
            childItem = m_rootItem->child(row);
        }
        else{
            childItem = parentItem->child(row);
        }

        Mpi3Playlist *dropPlaylist = m_library->getPlaylist(getPID(childItem));

        Mpi3Folder *dropFolder = m_library->getFolder(getPID(childItem));
        if(dropFolder){
//            qDebug() << dropFolder->name();
        }

        if(dropPlaylist){
//            qDebug() << dropPlaylist->name();
            return true;
        }
        else {
            return false;
        }
    }

    return false;
}
bool LibraryModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent){
    Q_UNUSED(data);
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

    return true;
}

int LibraryModel::rowCount(const QModelIndex &parent) const{
    LibraryItem *parentItem = getItem(parent);
    return parentItem->childCount();
}
int LibraryModel::columnCount(const QModelIndex &) const{
    return m_rootItem->columnCount();
}

QModelIndex LibraryModel::index(int row, int column, const QModelIndex &parent) const{
    if (parent.isValid() && parent.column() != 0){
        return QModelIndex();
    }

    LibraryItem *parentItem = getItem(parent);
    LibraryItem *childItem = parentItem->child(row);
    if (childItem){
        return createIndex(row, column, childItem);
    }
    else{
        return QModelIndex();
    }
}
QModelIndex LibraryModel::parent(const QModelIndex &index) const{
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

QVariant LibraryModel::data(const QModelIndex &index, int role) const{
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
QVariant LibraryModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return m_rootItem->data(section);
    }

    return QVariant();
}

bool LibraryModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if (role == Qt::EditRole){
        LibraryItem *item = getItem(index);
        bool result = item->setData(index.column(), value);

        QString pid = getPID(item);
        Mpi3Folder *folder = m_library->getFolder(pid);
        Mpi3Playlist *playlist = m_library->getPlaylist(pid);

        if(folder){
            m_library->modify(folder, Mpi3Folder::Name, value.toString());
        }
        else if (playlist){
            m_library->modify(playlist, Mpi3Playlist::Name, value.toString());
        }

        if(result){
            emit dataChanged(index, index);
        }
        return result;
    }

    return false;
}
bool LibraryModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role){
    if (role != Qt::EditRole || orientation != Qt::Horizontal){
        return false;
    }

    bool result = m_rootItem->setData(section, value);

    if (result){
        emit headerDataChanged(orientation, section, section);
    }

    return result;
}

bool LibraryModel::insertRows(int position, int count, const QModelIndex &parent){
    if(count > 0){
        LibraryItem *parentItem = getItem(parent);

        beginInsertRows(parent, position, position + count - 1);
        bool success = parentItem->insertChildren(position, count, m_rootItem->columnCount());
        endInsertRows();

        return success;
    }

    return false;
}
bool LibraryModel::insertColumns(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginInsertColumns(parent, position, position + count - 1);
        bool success = m_rootItem->insertColumns(position, count);
        endInsertColumns();

        return success;
    }

    return false;
}

bool LibraryModel::removeRows(int position, int count, const QModelIndex &parent){
    if(count > 0){
        LibraryItem *parentItem = getItem(parent);

        beginRemoveRows(parent, position, position + count - 1);
        bool success = parentItem->removeChildren(position, count);
        endRemoveRows();

        return success;
    }

    return false;
}
bool LibraryModel::removeColumns(int position, int count, const QModelIndex &parent){
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

QModelIndex LibraryModel::getIndex(const QString &pid, QModelIndex parent){
    LibraryItem *item = getItem(parent);
    for(int i = 0; i < item->childCount(); i++){
        QModelIndex idx = index(i, 0, parent);
        if(getPID(idx) == pid){
            return idx;
        }
        else {
            QModelIndex child = getIndex(pid, idx);
            if(child.isValid() && getPID(child) == pid){
                return child;
            }
        }
    }

    return QModelIndex();
}

LibraryItem *LibraryModel::getItem(const QModelIndex &index) const{
    if(index.isValid()){
        LibraryItem *item = static_cast<LibraryItem*>(index.internalPointer());
        if (item){
            return item;
        }
    }

    return m_rootItem;
}
LibraryItem *LibraryModel::getItem(const QString &pid) const{
    return pid.isNull() ? m_rootItem : m_libItems[pid];
}

QString LibraryModel::getPID(const QModelIndex &index) const{
    return getPID(getItem(index));
}
QString LibraryModel::getPID(LibraryItem *item) const{
    return m_libItems.key(item);
}

void LibraryModel::setLibrary(Mpi3Library *library){
    beginResetModel();

    if(m_library){
        disconnect(m_library.data(), &Mpi3Library::elementModified, this, &LibraryModel::elementModified);
        disconnect(m_library.data(), &Mpi3Library::elementInserted, this, &LibraryModel::elementInserted);
        disconnect(m_library.data(), &Mpi3Library::elementRemoved, this, &LibraryModel::elementRemoved);
    }

    m_library.take();
    m_library.reset(library);

    if(m_library){
        connect(m_library.data(), &Mpi3Library::elementModified, this, &LibraryModel::elementModified);
        connect(m_library.data(), &Mpi3Library::elementInserted, this, &LibraryModel::elementInserted);
        connect(m_library.data(), &Mpi3Library::elementRemoved, this, &LibraryModel::elementRemoved);
    }

    removeRows(0, rowCount());

    endResetModel();
}

void LibraryModel::populate(Mpi3Folder *parentFolder, QModelIndex parentIndex){
    QList<Mpi3Folder*> childFolders = m_library->childFolders(parentFolder);
    QList<Mpi3Playlist*> childPlaylists = m_library->childPlaylists(parentFolder);

    insertRows(0, childFolders.size() + childPlaylists.size(), parentIndex);

    for(int i = 0; i < childFolders.size(); i++){
        Mpi3Folder *folder = childFolders.at(i);
        LibraryItem *child = getItem(parentIndex)->child(i);
        child->setData(0, folder->name());
        child->setIcon(icn_folder);
    }
    for(int i = 0; i < childPlaylists.size(); i++){
        Mpi3Playlist *playlist = childPlaylists.at(i);
        LibraryItem *child = getItem(parentIndex)->child(i + childFolders.size());
        child->setData(0, playlist->name());
        child->setIcon(icn_playlist);
    }
}

void LibraryModel::playlistInserted(Mpi3Playlist *playlist, Mpi3Folder *folder){
    QModelIndex idx;
    LibraryItem *item = nullptr;
    int position = 0;

    if(folder){
        idx = getIndex(folder->pid());
        item = getItem(folder->pid());
        position = folder->playlists.indexOf(playlist);
    }
    else {
        item = m_rootItem;
        position = rowCount();
    }

    insertRows(position, 1, idx);

    LibraryItem *child = item->child(position);
    child->setData(0, playlist->name());
    child->setIcon(icn_playlist);

    m_libItems[playlist->pid()] = child;
}
void LibraryModel::folderInserted(Mpi3Folder *folder, Mpi3Folder *parent){
    QModelIndex idx;
    LibraryItem *item = nullptr;
    int position = 0;

    if(parent){
        idx = getIndex(parent->pid());
        item = getItem(parent->pid());
        position = parent->folders.indexOf(folder);
    }
    else {
        item = m_rootItem;
        position = rowCount();
    }

    insertRows(position, 1, idx);

    LibraryItem *child = item->child(position);
    child->setData(0, folder->name());
    child->setIcon(icn_folder);

    m_libItems[folder->pid()] = child;
}

void LibraryModel::elementModified(const QString &pidModified){
    Mpi3Folder *childFolder = m_library->getFolder(pidModified);
    Mpi3Playlist *childPlaylist = m_library->getPlaylist(pidModified);
    LibraryItem *item = getItem(pidModified);
    if(childFolder){
        item->setData(0, childFolder->name());
    }
    else if(childPlaylist){
        item->setData(0, childPlaylist->name());
    }
}
void LibraryModel::elementInserted(const QString &pidInserted, const QString &pidParent){
    Mpi3Folder *parentFolder = m_library->getFolder(pidParent);
    Mpi3Folder *childFolder = m_library->getFolder(pidInserted);
    Mpi3Playlist *childPlaylist = m_library->getPlaylist(pidInserted);

    if(childFolder){
        folderInserted(childFolder, parentFolder);
    }
    else if(childPlaylist){
        playlistInserted(childPlaylist, parentFolder);
    }
}
void LibraryModel::elementRemoved(const QString &pidRemoved, const QString &pidParent){
    QModelIndex parent = getIndex(pidParent);
    QModelIndex child = getIndex(pidRemoved);

    if(child.isValid()){
        removeRows(child.row(), 1, parent);
    }
}


SonglistModel::SonglistModel(QObject *parent) : QAbstractItemModel(parent){
    m_headers << "Name" << "Artist" << "Path";

    for(int i = 0; i < this->columnCount(); i++){
        columnVisibility[i] = false;
    }

}
SonglistModel::~SonglistModel(){}

Qt::ItemFlags SonglistModel::flags(const QModelIndex &index) const{
    if(index.isValid()){
        return Qt::ItemIsEditable
                | Qt::ItemIsDragEnabled
                | Qt::ItemIsDropEnabled
                | QAbstractItemModel::flags(index);
    }
    return Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
}
Qt::DropActions SonglistModel::supportedDragActions() const{
    return Qt::CopyAction;
}
Qt::DropActions SonglistModel::supportedDropActions() const{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList SonglistModel::mimeTypes() const {
    QStringList mTypes;
    mTypes << "text/plain";
    return mTypes;
}
QMimeData* SonglistModel::mimeData(const QModelIndexList &indexes) const{
    QMimeData *mimeData = new QMimeData();

    QString pids;
    for(int i = 0; i < indexes.size(); i++){
        QModelIndex index = indexes.at(i);
        pids += ";" + getPID(index);
    }

    mimeData->setText(pids);
    return mimeData;
}

bool SonglistModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const {
    Q_UNUSED(data);
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

    return true;
//    if(data->hasUrls()){
//        return validMediaFiles(data->urls());
//    }

//    return false;
}
bool SonglistModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent){
    //    Q_UNUSED(data);
        Q_UNUSED(action);
    //    Q_UNUSED(row);
        Q_UNUSED(column);
        Q_UNUSED(parent);

    if(!m_playlist){
        return false;
    }

    if(data->hasUrls()){
        for(int i = 0; i < data->urls().size(); i++){
            QUrl file = data->urls().at(i);
            Mpi3Song *song = m_library->newSong();
            m_library->insert(song, m_playlist, qMax(0, row));
            song->path = file.toString();
            m_library->modify(song, Mpi3Song::Name, file.fileName());
        }

        return true;
    }

    return false;
}

int SonglistModel::rowCount(const QModelIndex &) const{
    return m_playlist ? m_playlist->songs.size() : 0;
}
int SonglistModel::columnCount(const QModelIndex &) const{
    return m_headers.count();
}

QModelIndex SonglistModel::index(int row, int column, const QModelIndex &parent) const{
    Q_UNUSED(parent);
    return createIndex(row, column);
}
QModelIndex SonglistModel::parent(const QModelIndex &index) const{
    Q_UNUSED(index);
    return QModelIndex();
}

QVariant SonglistModel::data(const QModelIndex &index, int role) const{
    if(!m_playlist){
        return QVariant();
    }

    if (!index.isValid()){
        return QVariant();
    }
//    Qt::FontRole
//    Qt::TextAlignmentRole
//    Qt::BackgroundRole
//    Qt::ForegroundRole
//    Qt::CheckStateRole

    if (role == Qt::DisplayRole || role == Qt::EditRole){
        Mpi3Song *song = m_playlist->songs.at(index.row());
        switch(index.column()){
            case 0: return song->name();
            case 1: return song->artist;
            case 2: return song->path;
        }
    }

    return QVariant();
}
QVariant SonglistModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return m_headers.at(section);
    }
    return QVariant();
}

bool SonglistModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(!m_playlist){
        return false;
    }

    if (role == Qt::EditRole){
        Mpi3Song *song = m_playlist->songs.at(index.row());
        if(index.column() == 0){
            m_library->modify(song, Mpi3Song::Name, value.toString());
        }

        emit dataChanged(index, index);
        return true;
    }
    return false;
}
bool SonglistModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role){
    if (role != Qt::EditRole || orientation != Qt::Horizontal){
        return false;
    }

    m_headers[section] = value.toString();
    emit headerDataChanged(orientation, section, section);

    return true;
}

bool SonglistModel::insertRows(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginInsertRows(parent, position, position + count - 1);
        endInsertRows();
        return true;
    }
    return false;
}
bool SonglistModel::insertColumns(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginInsertColumns(parent, position, position + count - 1);
        endInsertColumns();
        return true;
    }
    return false;
}

bool SonglistModel::removeRows(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginRemoveRows(parent, position, position + count - 1);
        endRemoveRows();
        return true;
    }
    return false;
}
bool SonglistModel::removeColumns(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginRemoveColumns(parent, position, position + count - 1);
        endRemoveColumns();
        return true;
    }
    return false;
}

QString SonglistModel::getPID(const QModelIndex &index) const{
    return m_playlist->songs.at(index.row())->pid();
}

void SonglistModel::setLibrary(Mpi3Library *library){
    beginResetModel();

    if(m_library){
        disconnect(m_library.data(), &Mpi3Library::elementModified, this, &SonglistModel::elementModified);
        disconnect(m_library.data(), &Mpi3Library::elementInserted, this, &SonglistModel::elementInserted);
        disconnect(m_library.data(), &Mpi3Library::elementRemoved, this, &SonglistModel::elementRemoved);
    }

    m_library.take();
    m_library.reset(library);

    if(m_library){
        connect(m_library.data(), &Mpi3Library::elementModified, this, &SonglistModel::elementModified);
        connect(m_library.data(), &Mpi3Library::elementInserted, this, &SonglistModel::elementInserted);
        connect(m_library.data(), &Mpi3Library::elementRemoved, this, &SonglistModel::elementRemoved);
    }

    removeRows(0, rowCount());

    endResetModel();
}
void SonglistModel::setPlaylist(Mpi3Playlist *playlist){
    m_playlist = playlist;
}

void SonglistModel::elementModified(const QString &pidModified){
    Q_UNUSED(pidModified);
}
void SonglistModel::elementInserted(const QString &pidInserted, const QString &pidParent){
    if(!m_playlist){
        return;
    }

    if(m_playlist->pid() == pidParent){
        Mpi3Song *song = m_playlist->getSong(pidInserted);
//        qDebug() << m_playlist->songs.indexOf(song);
        insertRows(m_playlist->songs.indexOf(song), 1);
    }
}
void SonglistModel::elementRemoved(const QString &pidRemoved, const QString &pidParent){
    Q_UNUSED(pidRemoved);
    Q_UNUSED(pidParent);
}
