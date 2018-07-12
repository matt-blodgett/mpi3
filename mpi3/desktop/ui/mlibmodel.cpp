#include "mlibmodel.h"
#include "mlibitem.h"
#include "util/medialib.h"

#include <QPixmap>
//#include <QFont>


#include <QDebug>


LibraryModel::LibraryModel(QObject *parent) : QAbstractItemModel(parent){
    rootItem = new LibraryItem();

    QPixmap pix_folder(":/icons/treeview/folder.png");
    QPixmap pix_playlist(":/icons/treeview/playlist.png");

    icn_folder.addPixmap(pix_folder);
    icn_playlist.addPixmap(pix_playlist);
}
LibraryModel::~LibraryModel(){
    delete rootItem;
}

Qt::ItemFlags LibraryModel::flags(const QModelIndex &index) const{
    if (!index.isValid()){
        return 0;
    }

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
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
    if(role == Qt::DecorationRole && m_currentView == LibraryModel::ViewContainers && index.column() == 0){
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
    if (role != Qt::EditRole){
        return false;
    }

    LibraryItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result){
        emit dataChanged(index, index);
    }

    return result;
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

LibraryModel::View LibraryModel::currentView(){
    return m_currentView;
}
bool LibraryModel::setView(LibraryModel::View view){
    if(view == m_currentView && view != LibraryModel::ViewPlaylists){
        return false;
    }
    else {
        m_currentView = view;
        m_playlist.take();

        libItems.clear();

        removeRows(0, rowCount());
        removeColumns(0, columnCount());

        QVector<QString> headers;
        if(view == LibraryModel::ViewLibrary || view == LibraryModel::ViewPlaylists){
            headers << "Name" << "Artist" << "Path";
        } else {
            headers << "Name";
        }

        insertColumns(0, headers.count());
        for(int i = 0; i < headers.size(); i++){
            rootItem->setData(i, headers[i]);
        }

        return true;
    }
}

void LibraryModel::setCurrentIndex(QModelIndex &index){
    m_currentIndex = index;
}
QModelIndex LibraryModel::currentIndex(){
    return m_currentIndex;
}

Mpi3Library *LibraryModel::library() const{
    return m_library.data();
}
void LibraryModel::setLibrary(Mpi3Library *library){
    beginResetModel();

    m_library.take();
    m_library.reset(library);

    removeRows(0, rowCount());

    for(int i = 0; i < this->columnCount(); i++){
        columnVisibility[i] = false;
    }

    m_currentView = LibraryModel::ViewNone;
    viewLibrarySonglist();

    endResetModel();
}

void LibraryModel::viewLibrarySonglist(){
    if(!setView(LibraryModel::ViewLibrary)){
        return;
    }

    insertRows(0, m_library->libSongs->size());
    for(int i = 0; i < m_library->libSongs->size(); i++){
        Mpi3Song *s = m_library->libSongs->at(i);
        LibraryItem *item = rootItem->child(i);

        item->setData(0, s->name);
        item->setData(1, s->artist);
        item->setData(2, s->path);
        libItems[s->pid] = item;
    }
}
void LibraryModel::viewLibraryContainers(){
    if(!setView(LibraryModel::ViewContainers)){
        return;
    }

    for(int i = 0; i < m_library->libFolders->size(); i++){
        Mpi3Folder *f = m_library->libFolders->at(i);

        if(!f->parent){
            insertRows(rootItem->childCount(), 1);
            LibraryItem *item = rootItem->child(rootItem->childCount() - 1);
            libItems[f->pid] = item;
            recurseFolder(item, f);
            item->setData(0, f->name);
            item->setIcon(icn_folder);
        }
    }

    for(int i = 0; i < m_library->libPlaylists->size(); i++){
        Mpi3Playlist *p = m_library->libPlaylists->at(i);

        if(!p->parent){
            insertRows(rootItem->childCount(), 1);
            LibraryItem *item = rootItem->child(rootItem->childCount() - 1);
            libItems[p->pid] = item;
            item->setData(0, p->name);
            item->setIcon(icn_playlist);
        }
    }
}
void LibraryModel::viewLibraryArtists(){
    if(!setView(LibraryModel::ViewArtists)){
        return;
    }
}
void LibraryModel::viewPlaylist(Mpi3Playlist *playlist){
    if(!setView(LibraryModel::ViewPlaylists)){
        return;
    }

    m_playlist.reset(playlist);

    insertRows(0, playlist->songs.size());
    for(int i = 0; i < playlist->songs.size(); i++){
        Mpi3Song *s = playlist->songs.at(i);
        rootItem->child(i)->setData(0, s->name);
        rootItem->child(i)->setData(1, s->artist);
        rootItem->child(i)->setData(2, s->path);
    }
}

void LibraryModel::recurseFolder(LibraryItem *item, Mpi3Folder *folder){
    QList<Mpi3Folder*> folders = m_library->childFolders(folder);
    for(int i = 0; i < folders.size(); i++){

        Mpi3Folder *f = folders.at(i);
        item->insertChildren(item->childCount(), 1, columnCount());

        LibraryItem *child = item->child(item->childCount() - 1);
        libItems[f->pid] = child;

        child->setData(0, f->name);
        child->setIcon(icn_folder);
    }

    QList<Mpi3Playlist*> playlists = m_library->childPlaylists(folder);
    for(int i = 0; i < playlists.size(); i++){

        Mpi3Playlist *p = playlists.at(i);
        item->insertChildren(item->childCount(), 1, columnCount());

        LibraryItem *child = item->child(item->childCount() - 1);
        libItems[p->pid] = child;

        child->setData(0, p->name);
        child->setIcon(icn_playlist);
    }
}

void LibraryModel::insertFolder(){
    if(m_currentView != LibraryModel::ViewContainers){
        return;
    }

    Mpi3Folder *folder = new Mpi3Folder(true);

    m_library->libFolders->push_back(folder);
    folder->name = "New Folder";

    if(m_currentIndex.isValid()){
        QString pid = getPID(m_currentIndex);
        Mpi3Playlist *currentPlaylist = m_library->getPlaylist(pid);

        if(currentPlaylist){
            m_currentIndex = m_currentIndex.parent();
        }
    }

    Mpi3Folder *parentFolder = m_library->getFolder(getPID(m_currentIndex));

    if(parentFolder){
        parentFolder->folders.push_back(folder);
        folder->parent = parentFolder;
    }

    LibraryItem *item = getItem(m_currentIndex);

    int index = parentFolder ? parentFolder->folders.size() - 1 : m_library->rootFolders().size() - 1;

    qDebug() << index;

    insertRows(index, 1, m_currentIndex);

    LibraryItem *child = item->child(index);
    child->setData(0, folder->name);
    child->setIcon(icn_folder);

    libItems[folder->pid] = child;
}


void LibraryModel::insertPlaylist(){
    if(m_currentView != LibraryModel::ViewContainers){
        return;
    }

    Mpi3Playlist *playlist = new Mpi3Playlist(true);

    m_library->libPlaylists->push_back(playlist);
    playlist->name = "New Playlist";

    if(m_currentIndex.isValid()){
        QString pid = getPID(m_currentIndex);
        Mpi3Playlist *currentPlaylist = m_library->getPlaylist(pid);

        if(currentPlaylist){
            m_currentIndex = m_currentIndex.parent();
        }
    }

    Mpi3Folder *parentFolder = m_library->getFolder(getPID(m_currentIndex));
    if(parentFolder){
        parentFolder->playlists.push_back(playlist);
        playlist->parent = parentFolder;
    }

    LibraryItem *item = getItem(m_currentIndex);
    insertRows(item->childCount(), 1, m_currentIndex);

    LibraryItem *child = item->child(item->childCount() - 1);
    child->setData(0, playlist->name);
    child->setIcon(icn_playlist);
    libItems[playlist->pid] = child;
}
