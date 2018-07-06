#include "mlibmodel.h"
#include "mlibitem.h"
#include "util/medialib.h"

#include <QPixmap>

#include <QDebug>


LibraryModel::LibraryModel(QObject *parent) : QAbstractItemModel(parent){
    rootItem = new LibraryItem();

    QPixmap pix_folder(":/desktop/icons/folder.png");
    QPixmap pix_playlist(":/desktop/icons/playlist.png");

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
//    Qt::FontRole
//    Qt::TextAlignmentRole
//    Qt::BackgroundRole
//    Qt::ForegroundRole
//    Qt::CheckStateRole

    if(role == Qt::DecorationRole && m_currentView == LibraryModel::ContainersView && index.column() == 0){
        LibraryItem *item = getItem(index);
        return item->icon();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole){
        LibraryItem *item = getItem(index);
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
        qDebug() << value;
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
    if(count == 0){
        return false;
    }

    LibraryItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + count - 1);
    success = parentItem->removeChildren(position, count);
    endRemoveRows();

    return success;
}
bool LibraryModel::removeColumns(int position, int count, const QModelIndex &parent){
    if(count == 0){
        return false;
    }

    bool success;

    beginRemoveColumns(parent, position, position + count - 1);
    success = rootItem->removeColumns(position, count);
    endRemoveColumns();

    if (rootItem->columnCount() == 0){
        removeRows(0, rowCount());
    }

    return success;
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
QString LibraryModel::getPID(LibraryItem *item) const{
    return libItems.key(item);
}

LibraryModel::View LibraryModel::currentView(){
    return m_currentView;
}
bool LibraryModel::setView(LibraryModel::View view){
    if(m_currentView == view){
        return false;
    } else {
        m_currentView = view;
        libItems.clear();

        removeItems(0, rowCount());
        removeColumns(0, columnCount());

        QVector<QString> headers;
        if(view == LibraryModel::LibraryView || view == LibraryModel::PlaylistView){
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

    m_currentView = LibraryModel::PlaylistView;
    this->viewLibrarySonglist();
}

void LibraryModel::viewLibrarySonglist(){
    if(!setView(LibraryModel::LibraryView)){
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
    if(!setView(LibraryModel::ContainersView)){
        return;
    }

    for(int i = 0; i < m_library->libFolders->size(); i++){
        Mpi3Folder *f = m_library->libFolders->at(i);

        if(!f->parent){
            insertItems(rootItem->childCount(), 1);
            LibraryItem *item = rootItem->child(rootItem->childCount() - 1);
            libItems[f->pid] = item;
            insertFolder(item, f);
            item->setData(0, f->name);
            item->setIcon(icn_folder);
        }
    }

    for(int i = 0; i < m_library->libPlaylists->size(); i++){
        Mpi3Playlist *p = m_library->libPlaylists->at(i);

        if(!p->parent){
            insertItems(rootItem->childCount(), 1);
            LibraryItem *item = rootItem->child(rootItem->childCount() - 1);
            libItems[p->pid] = item;
            item->setData(0, p->name);
            item->setIcon(icn_playlist);
        }
    }
}
void LibraryModel::viewLibraryArtists(){
    if(!setView(LibraryModel::ArtistsView)){
        return;
    }
}
void LibraryModel::viewPlaylist(Mpi3Playlist *playlist){
    Q_UNUSED(playlist);
}

void LibraryModel::insertFolder(LibraryItem *item, Mpi3Folder *folder){
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

void LibraryModel::insertItems(int position, int count){
    beginInsertRows(QModelIndex(), position, position + count - 1);
    rootItem->insertChildren(position, count, rootItem->columnCount());
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




















