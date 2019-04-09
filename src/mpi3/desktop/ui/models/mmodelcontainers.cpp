#include "mmodelcontainers.h"
#include "mmodelcontainersitem.h"

#include "mmedialibrary.h"

#include <QMimeData>
#include <QPixmap>


#include <QDebug>


typedef MModelContainersItem MModelItem;


MModelContainers::MModelContainers(QObject *parent) : QAbstractItemModel(parent)
{
    m_rootItem = new MModelItem();
    m_rootItem->setLabel("Playlists");

    beginInsertColumns(QModelIndex(), 0, 1);
    endInsertColumns();
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
    Q_UNUSED(indexes);
    QMimeData *mData = new QMimeData();

//    if(indexes.size() == 1) {
//        QByteArray pidBytes;
//        pidBytes.append(getPID(indexes.at(0)));
//        mData->setData(QMetaType::typeName(qMetaTypeId<QString>()), pidBytes);
//    }

    return mData;
}

bool MModelContainers::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(data);
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

//    bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
//    bool dataIsContainer = data->hasFormat(QMetaType::typeName(qMetaTypeId<QString>()));
//    bool dataIsValidMediaFiles = data->hasUrls() ? MMediaLibrary::validMediaFiles(data->urls()) : false;
//    bool actionIsCopyAction = action == Qt::CopyAction;
//    bool actionIsMoveAction = action == Qt::MoveAction;

//    QModelIndex targetIndex = index(row, column, parent);
//    MMediaElement *targetElement = m_mediaLibrary->getElement(getPID(targetIndex));

//    if(!targetElement) {
////        return actionIsCopyAction && dataIsValidMediaFiles;
//        return dataIsContainer || dataIsValidMediaFiles;
//    }
//    else if(targetElement->type() == Mpi3::PlaylistElement) {
//        return actionIsCopyAction && (dataIsSonglist || dataIsValidMediaFiles);
//    }
//    else if(targetElement->type() == Mpi3::FolderElement) {

//        if(actionIsMoveAction && dataIsContainer){
//            QString pid = data->data(QMetaType::typeName(qMetaTypeId<QString>()));

//            MFolder *dropFolder = m_mediaLibrary->getFolder(pid);
//            MPlaylist *dropPlaylist = m_mediaLibrary->getPlaylist(pid);

//            MFolder *targetFolder = static_cast<MFolder*>(targetElement);
//            MPlaylist *targetPlaylist = static_cast<MPlaylist*>(targetElement);

//            if(dropFolder && !targetFolder) {
//                return dropFolder->parentFolder() || !targetPlaylist;
//            }
//            else if(dropPlaylist && !targetFolder) {
//                return dropPlaylist->parentFolder() || !targetPlaylist;
//            }
//            else if(dropFolder && targetFolder) {
//                return dropFolder != targetFolder && !dropFolder->childContainers(true).contains(targetFolder);
//            }
//            else if(dropPlaylist && targetFolder) {
//                return dropPlaylist->parentFolder() != targetFolder;
//            }
//        }
//    }

    return false;
}
bool MModelContainers::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(data);
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

//    bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
//    bool dataIsContainer = data->hasFormat(QMetaType::typeName(qMetaTypeId<QString>()));
//    bool dataIsValidMediaFiles = data->hasUrls() ? m_mediaLibrary->validMediaFiles(data->urls()) : false;
//    bool actionIsCopyAction = action == Qt::CopyAction;
//    bool actionIsMoveAction = action == Qt::MoveAction;

//    QModelIndex targetIndex = index(row, column, parent);
//    MMediaElement *targetElement = m_mediaLibrary->getElement(getPID(targetIndex));

//    if(!targetElement) {

//        if(actionIsMoveAction && dataIsContainer) {
//            QString pid = data->data(QMetaType::typeName(qMetaTypeId<QString>()));
//            MContainer *dropContainer = m_mediaLibrary->getContainer(pid);

//            if(dropContainer){
//                dropContainer->setParentFolder(nullptr);
//                return true;
//            }
//        }
//        else if(actionIsCopyAction && dataIsValidMediaFiles) {
//            for(QUrl url : data->urls()) {
//                m_mediaLibrary->newSong(url.toString());
//            }

//            return true;
//        }
//    }
//    else if(targetElement->type() == Mpi3::PlaylistElement) {
//        MPlaylist *targetPlaylist = static_cast<MPlaylist*>(targetElement);

//        if(actionIsCopyAction && dataIsSonglist) {
//            QByteArray pidBytes = data->data(QMetaType::typeName(qMetaTypeId<QStringList>()));
//            for(MSong *song : m_mediaLibrary->songsFromBytes(pidBytes)) {
//                targetPlaylist->append(song);
//            }

//            return true;
//        }
//        else if(actionIsCopyAction && dataIsValidMediaFiles) {
//            for(QUrl url : data->urls()) {
//                MSong *song = m_mediaLibrary->newSong(url.toString());
//                targetPlaylist->append(song);
//            }

//            return true;
//        }
//    }
//    else if(targetElement->type() == Mpi3::FolderElement) {
//        MFolder *targetFolder = static_cast<MFolder*>(targetElement);

//        if(actionIsMoveAction && dataIsContainer) {
//            QString pid = data->data(QMetaType::typeName(qMetaTypeId<QString>()));
//            MContainer *dropContainer = m_mediaLibrary->getContainer(pid);

//            if(dropContainer){
//                dropContainer->setParentFolder(targetFolder);
//                return true;
//            }
//        }
//    }

    return false;
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

int MModelContainers::rowCount(const QModelIndex &parent) const
{
    MModelItem *parentItem = getItem(parent);
    return parentItem->childCount();
}
int MModelContainers::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant MModelContainers::data(const QModelIndex &index, int role) const
{
    if(index.isValid() && index.column() == 0) {
        MModelItem *item = getItem(index);
        if(role == Qt::DecorationRole) {
            return item->icon();
        }
        else if(role == Qt::DisplayRole || role == Qt::EditRole) {
            return item->label();
        }
    }

    return QVariant();
}
bool MModelContainers::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::EditRole) {
        MModelItem *item = getItem(index);
        item->setLabel(value.toString());

//        MContainer *c = m_mediaLibrary->getContainer(getPID(item));
//        c->setName(value.toString());
        emit dataChanged(index, index);

        return true;
    }

    return false;
}

QVariant MModelContainers::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole && section == 0) {
        return m_rootItem->label();
    }

    return QVariant();
}


bool MModelContainers::insertRows(int position, int count, const QModelIndex &parent)
{
    if(count > 0) {
        MModelItem *parentItem = getItem(parent);

        beginInsertRows(parent, position, position + count - 1);
        bool result = parentItem->insertChildren(position, count);
        endInsertRows();

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




QList<MModelItem*> MModelContainers::allItems(MModelItem *parentItem) const
{
    if(!parentItem){
        parentItem = m_rootItem;
    }

    QList<MModelItem*> childItems;
    for(int i = 0; i < parentItem->childCount(); i++){
        MModelItem *childItem = parentItem->child(i);
        childItems.append(childItem);
        for(MModelItem *c : allItems(childItem)){
            childItems.append(c);
        }
    }

    return childItems;
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
    for(MModelItem *item : allItems()){
        if(item->pid() == pid){
            return item;
        }
    }

    return m_rootItem;
}
QModelIndex MModelContainers::getIndex(const QString &pid) const
{
    for(QModelIndex idx : persistentIndexList()) {
        if(getItem(idx)->pid() == pid) {
            return idx;
        }
    }

    return QModelIndex();
}
QString MModelContainers::getPID(const QModelIndex &index) const
{
    return getItem(index)->pid();
}

bool MModelContainers::itemIsPlaylist(const QModelIndex &index) const
{
    return getItem(index)->type() == MModelItem::PlaylistItem;
}
bool MModelContainers::itemIsFolder(const QModelIndex &index) const
{
    return getItem(index)->type() == MModelItem::FolderItem;
}

MMediaLibrary* MModelContainers::library() const
{
    return m_mediaLibrary;
}
void MModelContainers::setLibrary(MMediaLibrary *library)
{
    beginResetModel();
    m_rootItem->removeChildren(0, rowCount());

    if(m_mediaLibrary) {
        disconnect(m_mediaLibrary, nullptr, this, nullptr);
    }

    m_mediaLibrary = library;
    connect(m_mediaLibrary, &MMediaLibrary::folderCreated, this, &MModelContainers::folderCreated);
    connect(m_mediaLibrary, &MMediaLibrary::playlistCreated, this, &MModelContainers::playlistCreated);
    connect(m_mediaLibrary, &MMediaLibrary::folderDeleted, this, &MModelContainers::folderDeleted);
    connect(m_mediaLibrary, &MMediaLibrary::playlistDeleted, this, &MModelContainers::playlistDeleted);
    connect(m_mediaLibrary, &MMediaLibrary::folderChanged, this, &MModelContainers::folderChanged);
    connect(m_mediaLibrary, &MMediaLibrary::playlistChanged, this, &MModelContainers::playlistChanged);
//    connect(m_mediaLibrary, &MMediaLibrary::parentFolderChanged, this, &MModelContainers::parentFolderChanged);
    connect(m_mediaLibrary, &MMediaLibrary::libraryReset, this, [this](){
        beginResetModel(); m_rootItem->removeChildren(0, rowCount()); endResetModel();});

    MFolderList parentFolders;
    MFolderList childFolders;
    for(MFolder *f : m_mediaLibrary->folders()){
        if(f->parentFolder()){
            childFolders.append(f);
        }
        else {
            parentFolders.append(f);
        }
    }

    QList<MModelContainersItem*> modelItems;

    for(MFolder *f : parentFolders){
        int position = m_rootItem->childCount();
        m_rootItem->insertChildren(position, 1);

        MModelItem *child = m_rootItem->child(position);
        child->setType(MModelItem::FolderItem);
        child->setIcon(iconFolder);
        child->setPID(f->pid());
        child->setLabel(f->name());

        modelItems.append(child);
    }

    while(modelItems.size() != m_mediaLibrary->folders().size()){

        for(MFolder *f : childFolders){

            MModelItem *item = nullptr;
            for(MModelItem *i : modelItems){
                if(i->pid() == f->parentFolder()->pid()){
                    item = i;
                    break;
                }
            }

            if(item){
                int position = item->childCount();
                item->insertChildren(position, 1);

                MModelItem *child = item->child(position);
                child->setType(MModelItem::FolderItem);
                child->setIcon(iconFolder);
                child->setPID(f->pid());
                child->setLabel(f->name());

                modelItems.append(child);
            }
        }
    }

    for(MPlaylist *p : m_mediaLibrary->playlists()){
        MModelItem *item = m_rootItem;

        if(p->parentFolder()){
            for(MModelItem *i : modelItems){
                if(i->pid() == p->parentFolder()->pid()){
                    item = i;
                    break;
                }
            }
        }

        int position = item->childCount();
        item->insertChildren(position, 1);

        MModelItem *child = item->child(position);
        child->setType(MModelItem::PlaylistItem);
        child->setIcon(iconPlaylist);
        child->setPID(p->pid());
        child->setLabel(p->name());
    }

    endResetModel();
}

void MModelContainers::folderCreated(MFolder *f)
{

    qDebug() << persistentIndexList().size();

    MModelItem *parentItem = m_rootItem;
    if(f->parentFolder()){
        parentItem = getItem(f->parentFolder()->pid());
    }

    QModelIndex idx = QModelIndex();
    if(f->parentFolder()){
        idx = getIndex(f->parentFolder()->pid());
    }

//    int row = rowCount(idx);
    int row = parentItem->childCount();
//    MModelItem *parentItem = getItem(idx);
    for(int i = 0; i < parentItem->childCount(); i++){
        if(parentItem->child(i)->type() == MModelItem::PlaylistItem){
            row = i;
            break;
        }
    }

//    insertRows(row, 1, idx);
    beginInsertRows(idx, row, row);
    parentItem->insertChildren(row, 1);
    endInsertRows();

    MModelItem *childItem = parentItem->child(row);
    childItem->setType(MModelItem::FolderItem);
    childItem->setIcon(iconFolder);
    childItem->setPID(f->pid());
    childItem->setLabel(f->name());


//    emit dataChanged(idx, idx);
    qDebug() << persistentIndexList().size();
}
void MModelContainers::playlistCreated(MPlaylist *p)
{
    qDebug() << persistentIndexList().size();

    MModelItem *parentItem = m_rootItem;
    if(p->parentFolder()){
        parentItem = getItem(p->parentFolder()->pid());
    }

    QModelIndex idx = QModelIndex();
    if(p->parentFolder()){
        idx = getIndex(p->parentFolder()->pid());
    }

//    int row = rowCount(idx);
    int row = parentItem->childCount();
//    insertRows(row, 1, idx);

//    MModelItem *parentItem = getItem(idx);
    beginInsertRows(idx, row, row);
    parentItem->insertChildren(row, 1);
    endInsertRows();

    MModelItem *childItem = parentItem->child(row);
    childItem->setType(MModelItem::PlaylistItem);
    childItem->setIcon(iconPlaylist);
    childItem->setPID(p->pid());
    childItem->setLabel(p->name());

//    persistentIndexList().append(createIndex(row, 0, childItem));


    qDebug() << persistentIndexList().size();

//    emit dataChanged(idx, idx);
}

void MModelContainers::folderDeleted(MFolder *f)
{
    QModelIndex idx = getIndex(f->pid());
    beginRemoveRows(idx.parent(), idx.row(), idx.row());
    getItem(idx.parent())->removeChildren(idx.row(), 1);
    endRemoveRows();
}
void MModelContainers::playlistDeleted(MPlaylist *p)
{
    QModelIndex idx = getIndex(p->pid());
    beginRemoveRows(idx.parent(), idx.row(), idx.row());
    getItem(idx.parent())->removeChildren(idx.row(), 1);
    endRemoveRows();
}

void MModelContainers::folderChanged(MFolder *f)
{
    QModelIndex idx = getIndex(f->pid());
    getItem(idx)->setLabel(f->name());
    emit dataChanged(idx, idx);
}
void MModelContainers::playlistChanged(MPlaylist *p)
{
    QModelIndex idx = getIndex(p->pid());
    getItem(idx)->setLabel(p->name());
    emit dataChanged(idx, idx);
}

void MModelContainers::parentFolderChanged(MContainer *c)
{
    QModelIndex sourceIdx = getIndex(c->pid());
    QModelIndex parentIdx = c->parentFolder() ? getIndex(c->parentFolder()->pid()) : QModelIndex();

    MModelItem *sourceItem = getItem(sourceIdx);
    MModelItem *parentItem = getItem(parentIdx);

    int row = rowCount(parentIdx);
    for(int i = 0; i < parentItem->childCount(); i++){
        if(parentItem->child(i)->type() == MModelItem::PlaylistItem){
            row = i;
            break;
        }
    }

    qDebug() << sourceIdx.row();
    qDebug() << parentIdx.row();
    qDebug() << row;

    beginMoveRows(sourceIdx, sourceIdx.row(), sourceIdx.row(), parentIdx, row);
    sourceItem->move(parentItem, row);
    endMoveRows();
}

//void MModelContainers::setLibrary(MMediaLibrary *library)
//{
//    beginResetModel();

//    if(m_mediaLibrary) {
//        disconnect(m_mediaLibrary, nullptr, this, nullptr);
//    }

//    m_mediaLibrary = library;
//    if(m_mediaLibrary) {
//        connect(m_mediaLibrary, &MMediaLibrary::folderCreated, this, &MModelContainers::folderCreated);
//        connect(m_mediaLibrary, &MMediaLibrary::playlistCreated, this, &MModelContainers::playlistCreated);
//        connect(m_mediaLibrary, &MMediaLibrary::folderDeleted, this, &MModelContainers::folderDeleted);
//        connect(m_mediaLibrary, &MMediaLibrary::playlistDeleted, this, &MModelContainers::playlistDeleted);
//        connect(m_mediaLibrary, &MMediaLibrary::folderPropertyChanged, this, &MModelContainers::folderPropertyChanged);
//        connect(m_mediaLibrary, &MMediaLibrary::playlistPropertyChanged, this, &MModelContainers::playlistPropertyChanged);
//        connect(m_mediaLibrary, &MMediaLibrary::parentFolderChanged, this, &MModelContainers::parentFolderChanged);
//        connect(m_mediaLibrary, &MMediaLibrary::libraryLoaded, this, [this](){setLibrary(m_mediaLibrary);});
//        connect(m_mediaLibrary, &MMediaLibrary::libraryReset, this, [this](){setLibrary(m_mediaLibrary);});
//    }

//    removeRows(0, rowCount());
//    m_modelItems.clear();
//    populate();

//    endResetModel();
//}

//void MModelContainers::populate(MFolder *parentFolder, MModelItem *parentItem)
//{
//    QVector<MFolder*> folders = parentFolder ? parentFolder->childFolders() : m_mediaLibrary->rootFolders();
//    QVector<MPlaylist*> playlists = parentFolder ? parentFolder->childPlaylists() : m_mediaLibrary->rootPlaylists();

//    if(!parentItem) {
//        parentItem = m_rootItem;
//    }

//    parentItem->insertChildren(0, folders.size() + playlists.size(), columnCount());

//    for(int i = 0; i < folders.size(); i++) {
//        MFolder *childFolder = folders.at(i);
//        MModelItem *childItem = parentItem->child(i);
//        childItem->setData(0, childFolder->name());
//        childItem->setIcon(iconFolder);

//        m_modelItems[childFolder->pid()] = childItem;

//        populate(childFolder, childItem);
//    }

//    for(int i = 0; i < playlists.size(); i++) {
//        MPlaylist *childPlaylist = playlists.at(i);
//        MModelItem *childItem = parentItem->child(i + folders.size());
//        childItem->setData(0, childPlaylist->name());
//        childItem->setIcon(iconPlaylist);

//        m_modelItems[childPlaylist->pid()] = childItem;
//    }
//}

//void MModelContainers::containerCreated(MContainer *c)
//{
//    MModelItem *parentItem = m_rootItem;
//    QModelIndex parentIndex = QModelIndex();

//    if(c->parentFolder()){
//        QString pid = c->parentFolder()->pid();
//        parentItem = getItem(pid);
//        parentIndex = getIndex(pid);
//    }

//    int folderCount = 0;
//    int playlistCount = 0;
//    if(c->parentFolder()){
//        folderCount = c->parentFolder()->childFolders().size();
//        playlistCount = c->parentFolder()->childPlaylists().size();
//    }
//    else {
//        folderCount = m_mediaLibrary->rootFolders().size();
//        playlistCount = m_mediaLibrary->rootPlaylists().size();
//    }

//    QIcon icon;
//    int position = 0;
//    if(c->type() == Mpi3::FolderElement) {
//        position = qMax(0, folderCount - 1);
//        icon = iconFolder;
//    }
//    else {
//        position = qMax(0, folderCount + playlistCount - 1);
//        icon = iconPlaylist;
//    }

//    insertRows(position, 1, parentIndex);

//    MModelItem *childItem = parentItem->child(position);
//    childItem->setData(0, c->name());
//    childItem->setIcon(icon);

//    m_modelItems[c->pid()] = childItem;
//}
//void MModelContainers::containerDeleted(MContainer *c)
//{
//    QModelIndex idx = getIndex(c->pid());
//    if(idx.isValid()){
//        removeRows(idx.row(), 1, idx.parent());
//    }

//    m_modelItems.remove(c->pid());
//}

//void MModelContainers::folderCreated(MFolder *f)
//{
//    containerCreated(f);
//}
//void MModelContainers::playlistCreated(MPlaylist *p)
//{
//    containerCreated(p);
//}

//void MModelContainers::folderDeleted(MFolder *f)
//{
//    containerDeleted(f);
//}
//void MModelContainers::playlistDeleted(MPlaylist *p)
//{
//    containerDeleted(p);
//}

//void MModelContainers::folderPropertyChanged(
//    MFolder *childFolder,
//    const QString &propertyName,
//    const QVariant &oldPropertyValue,
//    const QVariant &newPropertyValue)
//{
//    Q_UNUSED(propertyName);
//    Q_UNUSED(oldPropertyValue);

//    QString pid = childFolder->pid();
//    QModelIndex idx = getIndex(pid);

//    m_modelItems[pid]->setData(0, newPropertyValue);
//    emit dataChanged(idx, idx);
//}

//void MModelContainers::playlistPropertyChanged(
//    MPlaylist *childPlaylist,
//    const QString &propertyName,
//    const QVariant &oldPropertyValue,
//    const QVariant &newPropertyValue)
//{
//    Q_UNUSED(propertyName);
//    Q_UNUSED(oldPropertyValue);

//    QString pid = childPlaylist->pid();
//    QModelIndex idx = getIndex(pid);

//    m_modelItems[pid]->setData(0, newPropertyValue);
//    emit dataChanged(idx, idx);
//}

//void MModelContainers::parentFolderChanged(
//    MContainer *childContainer,
//    MFolder *oldParentFolder,
//    MFolder *newParentFolder)
//{
//    Q_UNUSED(oldParentFolder);
//    Q_UNUSED(newParentFolder);

//    MContainer *c = childContainer;
//    containerDeleted(c);

//    if(c->type() == Mpi3::FolderElement){
//        MFolder *f = static_cast<MFolder*>(c);

//        for(MContainer *child : f->childContainers(true)){
//            m_modelItems.remove(child->pid());
//        }

//        MModelItem *item = nullptr;
//        if(f->parentFolder()){
//            item = getItem(f->parentFolder()->pid());
//        }

//        qDebug() << "beginResetModel";
//        beginResetModel();
//        qDebug() << "removeRows";
//        removeRows(0, rowCount());
//        qDebug() << "m_libItems";
//        m_modelItems.clear();
//        qDebug() << "populate";
//        populate();
//        qDebug() << "endResetModel";
//        endResetModel();
//    }
//    else if(c->type() == Mpi3::PlaylistElement){
//        containerCreated(c);
//    }
//}
