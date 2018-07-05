#include "mlibmodel.h"
#include "mlibitem.h"
#include "util/medialib.h"

#include <QDebug>


LibraryModel::LibraryModel(QObject *parent) : QAbstractItemModel(parent){
    rootItem = new LibraryItem();
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
    } else{
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

    if (role != Qt::DisplayRole && role != Qt::EditRole){
        return QVariant();
    }

    LibraryItem *item = getItem(index);
    return item->data(index.column());
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
    LibraryItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + count - 1);
    success = parentItem->insertChildren(position, count, rootItem->columnCount());
    endInsertRows();

    return success;
}
bool LibraryModel::insertColumns(int position, int count, const QModelIndex &parent){
    bool success;

    beginInsertColumns(parent, position, position + count - 1);
    success = rootItem->insertColumns(position, count);
    endInsertColumns();

    return success;
}

bool LibraryModel::removeRows(int position, int count, const QModelIndex &parent){
    LibraryItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + count - 1);
    success = parentItem->removeChildren(position, count);
    endRemoveRows();

    return success;
}
bool LibraryModel::removeColumns(int position, int count, const QModelIndex &parent){
    bool success;

    beginRemoveColumns(parent, position, position + count - 1);
    success = rootItem->removeColumns(position, count);
    endRemoveColumns();

    if (rootItem->columnCount() == 0){
        removeRows(0, rowCount());
    }

    return success;
}

LibraryModel::View LibraryModel::currentView(){
    return m_currentView;
}

Mpi3Library *LibraryModel::library() const{
    return m_library.data();
}
void LibraryModel::setLibrary(Mpi3Library *library){
    if (m_library) {
        disconnect(m_library.data(), &Mpi3Library::mediaInserted, this, &LibraryModel::insertItems);
        disconnect(m_library.data(), &Mpi3Library::mediaChanged, this, &LibraryModel::changeItems);
    }

    beginResetModel();

    m_library.take();
    m_library.reset(library);

    if (m_library) {
        connect(m_library.data(), &Mpi3Library::mediaInserted, this, &LibraryModel::insertItems);
        connect(m_library.data(), &Mpi3Library::mediaChanged, this, &LibraryModel::changeItems);
    }

    endResetModel();

    for(int i = 0; i < this->columnCount(); i++){
        columnVisibility[i] = false;
    }

    m_currentView = LibraryModel::Playlist;
    this->viewLibrarySonglist();
}

bool LibraryModel::setView(LibraryModel::View view){
    if(m_currentView == view){
        return false;
    } else {
        m_currentView = view;
        libItems.clear();

        removeItems(0, rowCount());
        rootItem->removeColumns(0, columnCount());

        QVector<QString> headers;
        if(view == LibraryModel::Library || view == LibraryModel::Playlist){
            headers << "Name" << "Artist" << "Path";
        } else {
            headers << "Name";
        }

        insertColumns(0, headers.size());
        for(int i = 0; i < headers.size(); i++){
            rootItem->setData(i, headers[i]);
        }

        return true;
    }
}

void LibraryModel::viewLibrarySonglist(){
    if(!setView(LibraryModel::Library)){
        return;
    }

    insertItems(0, m_library->libSongs->size());
    for(int i = 0; i < m_library->libSongs->size(); i++){
        Mpi3Song *s = m_library->libSongs->at(i);
        LibraryItem *item = rootItem->child(i);

        libItems[s->pid] = item;
        changeItems(i, s);
    }
}
void LibraryModel::viewLibraryContainers(){
    if(!setView(LibraryModel::Containers)){
        return;
    }

    for(int i = 0; i < m_library->libFolders->size(); i++){
        Mpi3Folder *f = m_library->libFolders->at(i);

        if(!f->parent){
            insertItems(rowCount(), 1);
            LibraryItem *item = rootItem->child(i);
            insertFolder(item, f);
            item->setData(0, f->name);
        }
    }
}
void LibraryModel::viewLibraryArtists(){
    if(!setView(LibraryModel::Artists)){
        return;
    }
}
void LibraryModel::insertFolder(LibraryItem *item, Mpi3Folder *folder){
    QList<Mpi3Folder*> folders = m_library->childFolders(folder);
    for(int i = 0; i < folders.size(); i++){
        Mpi3Folder *f = folders.at(i);
        item->insertChildren(item->childCount(), 1, columnCount());
        item->child(item->childCount()-1)->setData(0, f->name);
    }

    QList<Mpi3Playlist*> playlists = m_library->childPlaylists(folder);
    for(int i = 0; i < playlists.size(); i++){
        Mpi3Playlist *p = playlists.at(i);
        item->insertChildren(item->childCount(), 1, columnCount());
        item->child(item->childCount()-1)->setData(0, p->name);
    }
}

void LibraryModel::viewPlaylist(Mpi3Playlist *playlist){
    Q_UNUSED(playlist);
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




void LibraryModel::insertItems(int position, int count){
    beginInsertRows(QModelIndex(), position, position + count - 1);
    rootItem->insertChildren(position, count, columnCount());
    endInsertRows();
}
void LibraryModel::changeItems(int position, Mpi3Song *s){
    rootItem->child(position)->setData(0, s->name);
    rootItem->child(position)->setData(1, s->artist);
    rootItem->child(position)->setData(2, s->path);
}

void LibraryModel::removeItems(int position, int count){
    if(rowCount() <= 0){
        return;
    }
    beginRemoveRows(QModelIndex(), position, position + count - 1);
    rootItem->removeChildren(position, count);
    endRemoveRows();
}




















