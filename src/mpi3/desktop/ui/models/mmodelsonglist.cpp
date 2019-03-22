#include "mmodelsonglist.h"
#include "mmedialibrary.h"

#include <QMimeData>
#include <QUrl>


#include <QDebug>


MModelSonglist::MModelSonglist(QObject *parent) : QAbstractItemModel(parent)
{
    m_headers << "#";
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
MModelSonglist::~MModelSonglist()
{

}

Qt::ItemFlags MModelSonglist::flags(const QModelIndex &index) const
{
    Qt::ItemFlags idxFlags = Qt::ItemIsDropEnabled;

    if(index.isValid()) {
        idxFlags |= Qt::ItemIsEnabled;
        idxFlags |= Qt::ItemIsSelectable;
        idxFlags |= Qt::ItemNeverHasChildren;
        idxFlags |= Qt::ItemIsDragEnabled;

        if(index.column() >= 1 && index.column() <= 3) {
            idxFlags |= Qt::ItemIsEditable;
        }
    }

    return idxFlags;
}
Qt::DropActions MModelSonglist::supportedDragActions() const
{
    return Qt::CopyAction;
}
Qt::DropActions MModelSonglist::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList MModelSonglist::mimeTypes() const
{
    QStringList mTypes;
    mTypes << QMetaType::typeName(qMetaTypeId<QStringList>());

    return mTypes;
}
QMimeData *MModelSonglist::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mData = new QMimeData();

    QVector<MSong*> selectedSongs;
    for(QModelIndex idx : indexes) {
        MSong *song = getSong(idx);
        if(song && !selectedSongs.contains(song)) {
            selectedSongs.prepend(song);
        }
    }

    QByteArray pidBytes = MMediaLibrary::songsToBytes(selectedSongs);
    QList<QUrl> songUrls = MMediaLibrary::songsToPaths(selectedSongs);

    mData->setData(QMetaType::typeName(qMetaTypeId<QStringList>()), pidBytes);
    mData->setUrls(songUrls);

    return mData;
}

bool MModelSonglist::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

    if(!m_container || m_container->type() == Mpi3::PlaylistElement) {
        bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
        bool dataIsValidMediaFiles = data->hasUrls() ? MMediaLibrary::validMediaFiles(data->urls()) : false;
        bool containerIsPlaylist = m_container && m_container->type() == Mpi3::PlaylistElement;
        bool actionIsCopyAction = action == Qt::CopyAction;

        return (dataIsSonglist && containerIsPlaylist) || (dataIsValidMediaFiles && actionIsCopyAction);
    }

    return false;
}
bool MModelSonglist::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(column);
    Q_UNUSED(parent);

    if(!m_container || m_container->type() == Mpi3::PlaylistElement) {
        bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
        bool dataIsValidMediaFiles = data->hasUrls() ? MMediaLibrary::validMediaFiles(data->urls()) : false;
        bool actionIsCopyAction = action == Qt::CopyAction;
        bool actionIsMoveAction = action == Qt::MoveAction;

        row = row < 0 ? rowCount() : row;
        MPlaylist *playlist = static_cast<MPlaylist*>(m_container);

        if(dataIsSonglist && playlist) {
            QByteArray pidBytes = data->data(QMetaType::typeName(qMetaTypeId<QStringList>()));
            QVector<MSong*> droppedSongs = m_mediaLibrary->songsFromBytes(pidBytes);

            if(actionIsCopyAction) {
                playlist->insert(row, droppedSongs);
            }
            else if(actionIsMoveAction && playlist) {
                QList<int> indexes;
                for(MSong *s : droppedSongs){
                    indexes.append(m_songlist.indexOf(s));
                }

                playlist->move(indexes, row);
            }

            return true;
        }
        else if(dataIsValidMediaFiles && actionIsCopyAction) {
            MSongList copySongs;

            for(QUrl url : data->urls()) {
                MSong *s = m_mediaLibrary->newSong(url.toLocalFile());
                copySongs.append(s);
            }

            if(playlist){
                playlist->insert(row, copySongs);
            }

            return true;
        }
    }

    return false;
}

int MModelSonglist::rowCount(const QModelIndex &) const
{
    return m_songlist.size();
}
int MModelSonglist::columnCount(const QModelIndex &) const
{
    return m_headers.size();
}

QModelIndex MModelSonglist::index(int row, int column, const QModelIndex &) const
{
    return createIndex(row, column);
}
QModelIndex MModelSonglist::parent(const QModelIndex &) const
{
    return QModelIndex();
}

QVariant MModelSonglist::data(const QModelIndex &index, int role) const
{
//    Qt::FontRole
//    Qt::TextAlignmentRole
//    Qt::BackgroundRole
//    Qt::ForegroundRole
//    Qt::CheckStateRole

    if(index.isValid()) {
        MSong *song = m_songlist.at(index.row());

        if(role == Qt::DisplayRole) {
            switch(index.column()) {
                case 0: {return index.row() + 1;}
                case 1: {return song->name();}
                case 2: {return song->artist();}
                case 3: {return song->album();}
                case 4: {return song->timeString();}
                case 5: {return song->sizeString();}
                case 6: {return song->kind();}
                case 7: {return song->path();}
                case 8: {return song->bitRate();}
                case 9: {return song->sampleRate();}
            }
        }
        else if(role == Qt::EditRole) {
            switch(index.column()) {
                case 1: {return song->name();}
                case 2: {return song->artist();}
                case 3: {return song->album();}
            }
        }
    }

    return QVariant();
}
QVariant MModelSonglist::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return m_headers.at(section);
    }

    return QVariant();
}

bool MModelSonglist::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid() && (index.column() >= 1 && index.column() <= 3)) {
        MSong *song = m_songlist.at(index.row());

        if(role == Qt::EditRole) {
            switch(index.column()) {
                case 1: {
                    song->setName(value.toString());
                    break;
                }
                case 2: {
                    song->setArtist(value.toString());
                    break;
                }
                case 3: {
                    song->setAlbum(value.toString());
                    break;
                }
            }
        }

        emit dataChanged(index, index);
        return true;
    }

    return false;
}
bool MModelSonglist::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if(role == Qt::EditRole && orientation == Qt::Horizontal) {
        m_headers[section] = value.toString();
        emit headerDataChanged(orientation, section, section);
        return true;
    }

    return false;
}

bool MModelSonglist::insertRows(int position, int count, const QModelIndex &parent)
{
    if(count > 0) {
        beginInsertRows(parent, position, position + count - 1);
        endInsertRows();
        return true;
    }

    return false;
}
bool MModelSonglist::insertColumns(int position, int count, const QModelIndex &parent)
{
    if(count > 0) {
        beginInsertColumns(parent, position, position + count - 1);
        endInsertColumns();
        return true;
    }

    return false;
}

bool MModelSonglist::removeRows(int position, int count, const QModelIndex &parent)
{
    if(count > 0) {
        beginRemoveRows(parent, position, position + count - 1);
        endRemoveRows();
        return true;
    }

    return false;
}
bool MModelSonglist::removeColumns(int position, int count, const QModelIndex &parent)
{
    if(count > 0) {
        beginRemoveColumns(parent, position, position + count - 1);
        endRemoveColumns();
        return true;
    }

    return false;
}

MSong *MModelSonglist::getSong(const QModelIndex &index) const
{
    if(index.row() > -1 && index.row() < m_songlist.size()) {
        return m_songlist.at(index.row());
    }

    return nullptr;
}

MContainer *MModelSonglist::container() const
{
    return m_container;
}
void MModelSonglist::setContainer(MContainer *container)
{
    beginResetModel();

    removeRows(0, rowCount());

    m_container = container;
    m_songlist.clear();

    if(!m_container){
        m_songlist = m_mediaLibrary->songs();
    }
    else if(m_container->type() == Mpi3::PlaylistElement){
        m_songlist = static_cast<MPlaylist*>(m_container)->songs();
    }
    else if(m_container->type() == Mpi3::FolderElement){
        m_songlist = static_cast<MFolder*>(m_container)->childSongs(true);
    }

    insertRows(0, m_songlist.size());

    endResetModel();
}

MMediaLibrary *MModelSonglist::library() const
{
    return m_mediaLibrary;
}
void MModelSonglist::setLibrary(MMediaLibrary *library)
{
    beginResetModel();

    if(m_mediaLibrary) {
        disconnect(m_mediaLibrary, nullptr, this, nullptr);
    }

    m_mediaLibrary = library;
    if(m_mediaLibrary) {
        connect(m_mediaLibrary, &MMediaLibrary::songCreated, this, &MModelSonglist::songCreated);
        connect(m_mediaLibrary, &MMediaLibrary::songDeleted, this, &MModelSonglist::songDeleted);
        connect(m_mediaLibrary, &MMediaLibrary::songPropertyChanged, this, &MModelSonglist::songPropertyChanged);
        connect(m_mediaLibrary, &MMediaLibrary::playlistContentsChanged, this, &MModelSonglist::playlistContentsChanged);
        connect(m_mediaLibrary, &MMediaLibrary::libraryLoaded, this, [this](){setLibrary(m_mediaLibrary);});
        connect(m_mediaLibrary, &MMediaLibrary::libraryReset, this, [this](){setLibrary(m_mediaLibrary);});
    }

    removeRows(0, rowCount());

    m_container = nullptr;
    m_songlist.clear();

    endResetModel();
}

void MModelSonglist::songCreated(MSong *s)
{
    if(!m_container){
        m_songlist.append(s);
        insertRows(0, 1);
    }
}
void MModelSonglist::songDeleted(MSong *s)
{
    if(m_songlist.contains(s)){
        m_songlist.removeAll(s);
        removeRows(0, 1);
    }
}

void MModelSonglist::songPropertyChanged(
    MSong *childSong,
    const QString &propertyName,
    const QVariant &oldPropertyValue,
    const QVariant &newPropertyValue)
{
    Q_UNUSED(propertyName);
    Q_UNUSED(oldPropertyValue);
    Q_UNUSED(newPropertyValue);

    if(m_songlist.contains(childSong)){
        QModelIndex idx = index(m_songlist.indexOf(childSong), 0);
        emit dataChanged(idx, idx);
    }
}

void MModelSonglist::playlistContentsChanged(MPlaylist *childPlaylist)
{
    if(m_container){
        if(m_container == childPlaylist){
            int sizeDiff = childPlaylist->songs().size() - m_songlist.size();
            m_songlist = childPlaylist->songs();

            if(sizeDiff > 0){
                insertRows(0, sizeDiff);
            }
            else if(sizeDiff < 0){
                removeRows(0, -sizeDiff);
            }
        }
        else if(m_container->type() == Mpi3::FolderElement){
            MFolder *parentFolder = childPlaylist->parentFolder();
            while(parentFolder){
                if(parentFolder == m_container){
                    setContainer(m_container);
                    break;
                }
                parentFolder = parentFolder->parentFolder();
            }
        }
    }
}


MModelSonglistProxy::MModelSonglistProxy(QObject *parent) : QSortFilterProxyModel(parent)
{

}
