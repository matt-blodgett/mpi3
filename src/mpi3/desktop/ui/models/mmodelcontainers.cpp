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

    QModelIndex targetIndex = index(row, column, parent);
    MMediaElement *targetElement = m_mediaLibrary->getElement(getPID(targetIndex));

    if(!targetElement) {
//        return actionIsCopyAction && dataIsValidMediaFiles;
        return dataIsContainer || dataIsValidMediaFiles;
    }
    else if(targetElement->type() == Mpi3::PlaylistElement) {
        return actionIsCopyAction && (dataIsSonglist || dataIsValidMediaFiles);
    }
    else if(targetElement->type() == Mpi3::FolderElement) {

        if(actionIsMoveAction && dataIsContainer){
            QString pid = data->data(QMetaType::typeName(qMetaTypeId<QString>()));

            MFolder *dropFolder = m_mediaLibrary->getFolder(pid);
            MPlaylist *dropPlaylist = m_mediaLibrary->getPlaylist(pid);

            MFolder *targetFolder = static_cast<MFolder*>(targetElement);
            MPlaylist *targetPlaylist = static_cast<MPlaylist*>(targetElement);

            if(dropFolder && !targetFolder) {
                return dropFolder->parentFolder() || !targetPlaylist;
            }
            else if(dropPlaylist && !targetFolder) {
                return dropPlaylist->parentFolder() || !targetPlaylist;
            }
            else if(dropFolder && targetFolder) {
                return dropFolder != targetFolder && !dropFolder->childContainers(true).contains(targetFolder);
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
    bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
    bool dataIsContainer = data->hasFormat(QMetaType::typeName(qMetaTypeId<QString>()));
    bool dataIsValidMediaFiles = data->hasUrls() ? m_mediaLibrary->validMediaFiles(data->urls()) : false;
    bool actionIsCopyAction = action == Qt::CopyAction;
    bool actionIsMoveAction = action == Qt::MoveAction;

    QModelIndex targetIndex = index(row, column, parent);
    MMediaElement *targetElement = m_mediaLibrary->getElement(getPID(targetIndex));

    if(!targetElement) {

        if(actionIsMoveAction && dataIsContainer) {
            QString pid = data->data(QMetaType::typeName(qMetaTypeId<QString>()));
            MContainer *dropContainer = m_mediaLibrary->getContainer(pid);

            if(dropContainer){
                dropContainer->setParentFolder(nullptr);
                return true;
            }
        }
        else if(actionIsCopyAction && dataIsValidMediaFiles) {
            for(QUrl url : data->urls()) {
                m_mediaLibrary->newSong(url.toString());
            }

            return true;
        }
    }
    else if(targetElement->type() == Mpi3::PlaylistElement) {
        MPlaylist *targetPlaylist = static_cast<MPlaylist*>(targetElement);

        if(actionIsCopyAction && dataIsSonglist) {
            QByteArray pidBytes = data->data(QMetaType::typeName(qMetaTypeId<QStringList>()));
            for(MSong *song : m_mediaLibrary->songsFromBytes(pidBytes)) {
                targetPlaylist->append(song);
            }

            return true;
        }
        else if(actionIsCopyAction && dataIsValidMediaFiles) {
            for(QUrl url : data->urls()) {
                MSong *song = m_mediaLibrary->newSong(url.toString());
                targetPlaylist->append(song);
            }

            return true;
        }
    }
    else if(targetElement->type() == Mpi3::FolderElement) {
        MFolder *targetFolder = static_cast<MFolder*>(targetElement);

        if(actionIsMoveAction && dataIsContainer) {
            QString pid = data->data(QMetaType::typeName(qMetaTypeId<QString>()));
            MContainer *dropContainer = m_mediaLibrary->getContainer(pid);

            if(dropContainer){
                dropContainer->setParentFolder(targetFolder);
                return true;
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

QVariant MModelContainers::data(const QModelIndex &index, int role) const
{
//    Qt::FontRole
//    Qt::TextAlignmentRole
//    Qt::BackgroundRole
//    Qt::ForegroundRole
//    Qt::CheckStateRole

    if(index.isValid()) {
        MModelItem *item = getItem(index);
        if(role == Qt::DecorationRole && index.column() == 0) {
            return item->icon();
        }

        if(role == Qt::DisplayRole || role == Qt::EditRole) {
            return item->data(index.column());
        }
    }

    return QVariant();
}
QVariant MModelContainers::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return m_rootItem->data(section);
    }

    return QVariant();
}

bool MModelContainers::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole) {
        MModelItem *item = getItem(index);
        bool result = item->setData(index.column(), value);

        if(result) {
            MContainer *c = m_mediaLibrary->getContainer(getPID(item));
            c->setName(value.toString());
            emit dataChanged(index, index);
        }

        return result;
    }

    return false;
}
bool MModelContainers::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if(role == Qt::EditRole || orientation == Qt::Horizontal) {
        bool result = m_rootItem->setData(section, value);

        if(result) {
            emit headerDataChanged(orientation, section, section);
        }

        return result;
    }

    return false;
}

bool MModelContainers::insertRows(int position, int count, const QModelIndex &parent)
{
    if(count > 0) {
        MModelItem *parentItem = getItem(parent);

        beginInsertRows(parent, position, position + count - 1);
        bool result = parentItem->insertChildren(position, count, m_rootItem->columnCount());
        endInsertRows();

        return result;
    }

    return false;
}
bool MModelContainers::insertColumns(int position, int count, const QModelIndex &parent)
{
    if(count > 0) {
        beginInsertColumns(parent, position, position + count - 1);
        bool result = m_rootItem->insertColumns(position, count);
        endInsertColumns();

        return result;
    }

    return false;
}

bool MModelContainers::removeRows(int position, int count, const QModelIndex &parent)
{
    if(count > 0) {
        MModelItem *parentItem = getItem(parent);

        beginRemoveRows(parent, position, position + count - 1);
        bool result = parentItem->removeChildren(position, count);
        endRemoveRows();

        return result;
    }

    return false;
}
bool MModelContainers::removeColumns(int position, int count, const QModelIndex &parent)
{
    if(count > 0) {
        beginRemoveColumns(parent, position, position + count - 1);
        bool result = m_rootItem->removeColumns(position, count);
        endRemoveColumns();

        if(m_rootItem->columnCount() == 0) {
            removeRows(0, rowCount());
        }

        return result;
    }

    return false;
}

QModelIndex MModelContainers::getIndex(const QString &pid) const
{
    for(QModelIndex idx : persistentIndexList()) {
        if(getPID(idx) == pid) {
            return idx;
        }
    }

    return QModelIndex();
}
QModelIndex MModelContainers::getIndex(MModelItem *item) const
{
    return getIndex(getPID(item));
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
    if(item && m_libItems.values().contains(item)) {
        return m_libItems.key(item);
    }

    return QString();
}

MContainer *MModelContainers::getContainer(const QModelIndex &index) const
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
        connect(m_mediaLibrary, &MMediaLibrary::folderCreated, this, &MModelContainers::folderCreated);
        connect(m_mediaLibrary, &MMediaLibrary::playlistCreated, this, &MModelContainers::playlistCreated);
        connect(m_mediaLibrary, &MMediaLibrary::folderDeleted, this, &MModelContainers::folderDeleted);
        connect(m_mediaLibrary, &MMediaLibrary::playlistDeleted, this, &MModelContainers::playlistDeleted);
        connect(m_mediaLibrary, &MMediaLibrary::folderPropertyChanged, this, &MModelContainers::folderPropertyChanged);
        connect(m_mediaLibrary, &MMediaLibrary::playlistPropertyChanged, this, &MModelContainers::playlistPropertyChanged);
        connect(m_mediaLibrary, &MMediaLibrary::parentFolderChanged, this, &MModelContainers::parentFolderChanged);
        connect(m_mediaLibrary, &MMediaLibrary::libraryLoaded, this, [this](){setLibrary(m_mediaLibrary);});
        connect(m_mediaLibrary, &MMediaLibrary::libraryReset, this, [this](){setLibrary(m_mediaLibrary);});
    }

    removeRows(0, rowCount());
    m_libItems.clear();
    populate();

    endResetModel();
}

void MModelContainers::populate(MFolder *parentFolder, MModelItem *parentItem)
{
    QVector<MFolder*> folders = parentFolder ? parentFolder->childFolders() : m_mediaLibrary->rootFolders();
    QVector<MPlaylist*> playlists = parentFolder ? parentFolder->childPlaylists() : m_mediaLibrary->rootPlaylists();

    if(!parentItem) {
        parentItem = m_rootItem;
    }

    parentItem->insertChildren(0, folders.size() + playlists.size(), columnCount());

    for(int i = 0; i < folders.size(); i++) {
        MFolder *childFolder = folders.at(i);
        MModelItem *childItem = parentItem->child(i);

        childItem->setData(0, childFolder->name());
        childItem->setIcon(iconFolder);

        m_libItems[childFolder->pid()] = childItem;

        populate(childFolder, childItem);
    }

    for(int i = 0; i < playlists.size(); i++) {
        MPlaylist *childPlaylist = playlists.at(i);
        MModelItem *childItem = parentItem->child(i + folders.size());

        childItem->setData(0, childPlaylist->name());
        childItem->setIcon(iconPlaylist);

        m_libItems[childPlaylist->pid()] = childItem;
    }
}

void MModelContainers::containerCreated(MContainer *c)
{
    MModelItem *parentItem = m_rootItem;
    QModelIndex parentIndex = QModelIndex();

    if(c->parentFolder()){
        QString pid = c->parentFolder()->pid();
        parentItem = getItem(pid);
        parentIndex = getIndex(pid);
    }

    int folderCount = 0;
    int playlistCount = 0;
    if(c->parentFolder()){
        folderCount = c->parentFolder()->childFolders().size();
        playlistCount = c->parentFolder()->childPlaylists().size();
    }
    else {
        folderCount = m_mediaLibrary->rootFolders().size();
        playlistCount = m_mediaLibrary->rootPlaylists().size();
    }

    QIcon icon;
    int position = 0;
    if(c->type() == Mpi3::FolderElement) {
        position = qMax(0, folderCount - 1);
        icon = iconFolder;
    }
    else {
        position = qMax(0, folderCount + playlistCount - 1);
        icon = iconPlaylist;
    }

    insertRows(position, 1, parentIndex);

    MModelItem *childItem = parentItem->child(position);
    childItem->setData(0, c->name());
    childItem->setIcon(icon);

    m_libItems[c->pid()] = childItem;
}
void MModelContainers::containerDeleted(MContainer *c)
{
    QModelIndex idx = getIndex(c->pid());
    if(idx.isValid()){
        removeRows(idx.row(), 1, idx.parent());
    }

    m_libItems.remove(c->pid());
}

void MModelContainers::folderCreated(MFolder *f)
{
    containerCreated(f);
}
void MModelContainers::playlistCreated(MPlaylist *p)
{
    containerCreated(p);
}

void MModelContainers::folderDeleted(MFolder *f)
{
    containerDeleted(f);
}
void MModelContainers::playlistDeleted(MPlaylist *p)
{
    containerDeleted(p);
}

void MModelContainers::folderPropertyChanged(
    MFolder *childFolder,
    const QString &propertyName,
    const QVariant &oldPropertyValue,
    const QVariant &newPropertyValue)
{
    Q_UNUSED(propertyName);
    Q_UNUSED(oldPropertyValue);

    QString pid = childFolder->pid();
    QModelIndex idx = getIndex(pid);

    m_libItems[pid]->setData(0, newPropertyValue);
    emit dataChanged(idx, idx);
}

void MModelContainers::playlistPropertyChanged(
    MPlaylist *childPlaylist,
    const QString &propertyName,
    const QVariant &oldPropertyValue,
    const QVariant &newPropertyValue)
{
    Q_UNUSED(propertyName);
    Q_UNUSED(oldPropertyValue);

    QString pid = childPlaylist->pid();
    QModelIndex idx = getIndex(pid);

    m_libItems[pid]->setData(0, newPropertyValue);
    emit dataChanged(idx, idx);
}

void MModelContainers::parentFolderChanged(
    MContainer *childContainer,
    MFolder *oldParentFolder,
    MFolder *newParentFolder)
{
    Q_UNUSED(oldParentFolder);
    Q_UNUSED(newParentFolder);

    MContainer *c = childContainer;
    containerDeleted(c);

    if(c->type() == Mpi3::FolderElement){
        containerCreated(c);

        MFolder *f = static_cast<MFolder*>(c);

        for(MContainer *child : f->childContainers(true)){
            m_libItems.remove(child->pid());
        }

        MModelItem *item = nullptr;
        if(f->parentFolder()){
            item = getItem(f->parentFolder()->pid());
        }

        populate(f, item);
    }
    else if(c->type() == Mpi3::PlaylistElement){
        containerCreated(c);
    }
}
