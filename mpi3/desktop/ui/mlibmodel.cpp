#include "mlibmodel.h"
#include "mlibitem.h"
#include "util/medialib.h"

#include <QMimeData>
#include <QPixmap>
//#include <QFont>
#include <QUrl>

#include <QDebug>


LibraryModel::LibraryModel(QObject *parent) : QAbstractItemModel(parent){
    rootItem = new LibraryItem();

    insertColumns(0, 1);
    rootItem->setData(0, "Playlists");

    QPixmap pix_folder(":/icons/treeview/folder.png");
    QPixmap pix_playlist(":/icons/treeview/playlist.png");

    icn_folder.addPixmap(pix_folder);
    icn_playlist.addPixmap(pix_playlist);
}
LibraryModel::~LibraryModel(){
    delete rootItem;
}

Qt::ItemFlags LibraryModel::flags(const QModelIndex &index) const{
    if (index.isValid()){
        Mpi3Folder *folder = m_library->getFolder(getPID(index));
        if(folder){
            return Qt::ItemIsEditable
                    | Qt::ItemIsDragEnabled
                    | Qt::ItemIsDropEnabled
                    | QAbstractItemModel::flags(index);
        }
        else {
            return Qt::ItemIsEditable
                    | Qt::ItemIsDragEnabled
                    | QAbstractItemModel::flags(index);
        }
    }
    return Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
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

int LibraryModel::rowCount(const QModelIndex &parent) const{
    LibraryItem *parentItem = getItem(parent);
    return parentItem->childCount();
}
int LibraryModel::columnCount(const QModelIndex &) const{
    return rootItem->columnCount();
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
    if (parentItem == rootItem){
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
        return rootItem->data(section);
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

        if (result){
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

    bool result = rootItem->setData(section, value);

    if (result){
        emit headerDataChanged(orientation, section, section);
    }

    return result;
}

bool LibraryModel::insertRows(int position, int count, const QModelIndex &parent){
    if(count > 0){
        LibraryItem *parentItem = getItem(parent);

        beginInsertRows(parent, position, position + count - 1);
        bool success = parentItem->insertChildren(position, count, rootItem->columnCount());
        endInsertRows();

        return success;
    }
    return false;
}
bool LibraryModel::insertColumns(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginInsertColumns(parent, position, position + count - 1);
        bool success = rootItem->insertColumns(position, count);
        endInsertColumns();

        return success;
    }
    return false;
}

bool LibraryModel::removeRows(int position, int count, const QModelIndex &parent){
    if(count > 0){
        LibraryItem *parentItem = getItem(parent);
        bool success = true;

        beginRemoveRows(parent, position, position + count - 1);
        success = parentItem->removeChildren(position, count);
        endRemoveRows();

        return success;
    }
    return false;
}
bool LibraryModel::removeColumns(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginRemoveColumns(parent, position, position + count - 1);
        bool success = rootItem->removeColumns(position, count);
        endRemoveColumns();

        if (rootItem->columnCount() == 0){
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
    return rootItem;
}
LibraryItem *LibraryModel::getItem(const QString &pid) const{
    return libItems[pid];
}

QString LibraryModel::getPID(const QModelIndex &index) const{
    return getPID(getItem(index));
}
QString LibraryModel::getPID(LibraryItem *item) const{
    return libItems.key(item);
}

QModelIndex LibraryModel::currentIndex(){
    return m_currentIndex;
}
void LibraryModel::setCurrentIndex(QModelIndex &index){
    m_currentIndex = index;
}

Mpi3Library *LibraryModel::library() const{
    return m_library.data();
}
void LibraryModel::setLibrary(Mpi3Library *library){
    beginResetModel();

    if(m_library){
        disconnect(m_library.data(), &Mpi3Library::playlistModified, this, &LibraryModel::playlistModified);
        disconnect(m_library.data(), &Mpi3Library::playlistInserted, this, &LibraryModel::playlistInserted);
        disconnect(m_library.data(), &Mpi3Library::playlistRemoved, this, &LibraryModel::playlistRemoved);

        disconnect(m_library.data(), &Mpi3Library::folderModified, this, &LibraryModel::folderModified);
        disconnect(m_library.data(), &Mpi3Library::folderInserted, this, &LibraryModel::folderInserted);
        disconnect(m_library.data(), &Mpi3Library::folderRemoved, this, &LibraryModel::folderRemoved);
    }

    m_library.take();
    m_library.reset(library);

    if(m_library){
        connect(m_library.data(), &Mpi3Library::playlistModified, this, &LibraryModel::playlistModified);
        connect(m_library.data(), &Mpi3Library::playlistInserted, this, &LibraryModel::playlistInserted);
        connect(m_library.data(), &Mpi3Library::playlistRemoved, this, &LibraryModel::playlistRemoved);

        connect(m_library.data(), &Mpi3Library::folderModified, this, &LibraryModel::folderModified);
        connect(m_library.data(), &Mpi3Library::folderInserted, this, &LibraryModel::folderInserted);
        connect(m_library.data(), &Mpi3Library::folderRemoved, this, &LibraryModel::folderRemoved);
    }

    removeRows(0, rowCount());

    endResetModel();
}

void LibraryModel::playlistModified(Mpi3Playlist *playlist){
    LibraryItem *item = getItem(playlist->pid());
    item->setData(0, playlist->name());
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
        item = rootItem;
        position = rowCount();
    }

    insertRows(position, 1, idx);

    LibraryItem *child = item->child(position);
    child->setData(0, playlist->name());
    child->setIcon(icn_playlist);

    libItems[playlist->pid()] = child;
}
void LibraryModel::playlistRemoved(Mpi3Playlist *playlist, Mpi3Folder *folder){
    Q_UNUSED(playlist);
    Q_UNUSED(folder);
}

void LibraryModel::folderModified(Mpi3Folder *folder){
    LibraryItem *item = getItem(folder->pid());
    item->setData(0, folder->name());
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
        item = rootItem;
        position = rowCount();
    }

    insertRows(position, 1, idx);

    LibraryItem *child = item->child(position);
    child->setData(0, folder->name());
    child->setIcon(icn_folder);

    libItems[folder->pid()] = child;
}
void LibraryModel::folderRemoved(Mpi3Folder *folder, Mpi3Folder *parent){
    Q_UNUSED(folder);
    Q_UNUSED(parent);
}




void LibraryModel::insertFolder(){
    if(m_currentIndex.isValid()){
        QString pid = getPID(m_currentIndex);
        Mpi3Playlist *currentPlaylist = m_library->getPlaylist(pid);

        if(currentPlaylist){
            m_currentIndex = m_currentIndex.parent();
        }
    }

    Mpi3Folder *parent = m_library->getFolder(getPID(m_currentIndex));
    Mpi3Folder *folder = m_library->newFolder(true);
    int position = parent ? parent->folders.size() : -1;

    m_library->insert(folder, parent, position);
}
void LibraryModel::insertPlaylist(){
    if(m_currentIndex.isValid()){
        QString pid = getPID(m_currentIndex);
        Mpi3Playlist *currentPlaylist = m_library->getPlaylist(pid);

        if(currentPlaylist){
            m_currentIndex = m_currentIndex.parent();
        }
    }

    Mpi3Folder *parent = m_library->getFolder(getPID(m_currentIndex));
    Mpi3Playlist *playlist = m_library->newPlaylist(true);
    int position = parent ? parent->playlists.size() : -1;

    m_library->insert(playlist, parent, position);
}

void LibraryModel::recurseFolder(LibraryItem *item, Mpi3Folder *folder){
    QList<Mpi3Folder*> folders = m_library->childFolders(folder);
    for(int i = 0; i < folders.size(); i++){

        Mpi3Folder *f = folders.at(i);
        item->insertChildren(item->childCount(), 1, columnCount());

        LibraryItem *child = item->child(item->childCount() - 1);
        libItems[f->pid()] = child;

        child->setData(0, f->name());
        child->setIcon(icn_folder);
    }

    QList<Mpi3Playlist*> playlists = m_library->childPlaylists(folder);
    for(int i = 0; i < playlists.size(); i++){

        Mpi3Playlist *p = playlists.at(i);
        item->insertChildren(item->childCount(), 1, columnCount());

        LibraryItem *child = item->child(item->childCount() - 1);
        libItems[p->pid()] = child;

        child->setData(0, p->name());
        child->setIcon(icn_playlist);
    }
}


SonglistModel::SonglistModel(QObject *parent) : QAbstractItemModel(parent){
    m_headers << "Name" << "Artist" << "Path";

    for(int i = 0; i < this->columnCount(); i++){
        columnVisibility[i] = false;
    }

}
SonglistModel::~SonglistModel(){

}

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

int SonglistModel::rowCount(const QModelIndex &) const{
    return m_songlist.count();
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
    if (!index.isValid()){
        return QVariant();
    }
//    Qt::FontRole
//    Qt::TextAlignmentRole
//    Qt::BackgroundRole
//    Qt::ForegroundRole
//    Qt::CheckStateRole

    if (role == Qt::DisplayRole || role == Qt::EditRole){
        Mpi3Song *song = m_songlist.at(index.row());
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
    if (role == Qt::EditRole){
        Mpi3Song *song = m_songlist.at(index.row());
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
//        LibraryItem *parentItem = getItem(parent);
//        bool success = true;

        beginRemoveRows(parent, position, position + count - 1);
//        success = parentItem->removeChildren(position, count);
        endRemoveRows();

        return true;
    }
    return false;
}
bool SonglistModel::removeColumns(int position, int count, const QModelIndex &parent){
    if(count > 0){
        beginRemoveColumns(parent, position, position + count - 1);
//        bool success = rootItem->removeColumns(position, count);
        endRemoveColumns();

//        if (rootItem->columnCount() == 0){
//            removeRows(0, rowCount());
//        }

        return true;
    }
    return false;
}

Mpi3Library *SonglistModel::library() const{
    return m_library.data();
}
void SonglistModel::setLibrary(Mpi3Library *library){
    m_library.take();
    m_library.reset(library);
}

QString SonglistModel::getPID(const QModelIndex &index) const{
    return m_songlist.at(index.row())->pid();
}

void SonglistModel::setSonglist(QVector<Mpi3Song*> songlist){
    m_songlist = songlist;
}

void SonglistModel::dropExternalFiles(QModelIndex index, QList<QUrl> urls){
    int position = index.row() == -1 ? 0 : index.row();
    insertRows(position, urls.size());

    for(int i = 0; i < urls.size(); i++){
        Mpi3Song *song = m_library->newSong();
//        song->name = urls.at(i).fileName();
//        song->path = urls.at(i).toString();
        m_songlist.insert(i + position, song);
    }
}



























