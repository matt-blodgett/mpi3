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

        if(index.column() >= 0 && index.column() <= 2) {
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

    Q_UNUSED(data)
    Q_UNUSED(action)
//    Mpi3::ElementType elemType = m_mediaContainer->type();
//    if(elemType == Mpi3::PlaylistElement || elemType == Mpi3::LibraryElement) {

//        bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
//        bool dataIsValidMediaFiles = data->hasUrls() ? MMediaLibrary::validMediaFiles(data->urls()) : false;
//        bool actionIsCopyAction = action == Qt::CopyAction;

//        return dataIsSonglist || (dataIsValidMediaFiles && actionIsCopyAction);
//    }

    return false;
}
bool MModelSonglist::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(column);
    Q_UNUSED(parent);

    Q_UNUSED(data)
    Q_UNUSED(action)
    Q_UNUSED(row)
//    Mpi3::ElementType elemType = m_mediaContainer->type();
//    if(elemType == Mpi3::PlaylistElement || elemType == Mpi3::LibraryElement) {

//        bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
//        bool dataIsValidMediaFiles = data->hasUrls() ? m_mediaLibrary->validMediaFiles(data->urls()) : false;
//        bool actionIsCopyAction = action == Qt::CopyAction;
//        bool actionIsMoveAction = action == Qt::MoveAction;

//        row = row < 0 ? rowCount() - 1: row;

//        MPlaylist *parentPlaylist = nullptr;
//        if(elemType == Mpi3::PlaylistElement) {
//            parentPlaylist = static_cast<MPlaylist*>(m_mediaContainer);
//        }

//        if(dataIsSonglist) {
//            QByteArray pidBytes = data->data(QMetaType::typeName(qMetaTypeId<QStringList>()));
//            QVector<MSong*> droppedSongs = m_mediaLibrary->songsFromBytes(pidBytes);

//            if(actionIsCopyAction) {
//                (MSong *song, droppedSongs) {
//                    m_mediaLibrary->insert(song, parentPlaylist, row);
//                }
//                return true;
//            }
//            else if(actionIsMoveAction && elemType == Mpi3::PlaylistElement) {
//                (MSong *song, droppedSongs) {
//                    m_mediaLibrary->move(song, parentPlaylist, row);
//                }
//                return true;
//            }
//        }
//        else if(dataIsValidMediaFiles && actionIsCopyAction) {
//            (QUrl url, data->urls()) {
//                MSong *droppedSong = m_mediaLibrary->newSong(url.toString());
//                m_mediaLibrary->insert(droppedSong, parentPlaylist, row);
//            }
//            return true;
//        }
//    }

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
                case 0: return index.row();
                case 1: return song->name();
                case 2: return song->artist();
                case 3: return song->album();
                case 4: return song->timeString();
                case 5: return song->sizeString();
                case 6: return song->kind();
                case 7: return song->path();
                case 8: return song->bitRate();
                case 9: return song->sampleRate();
            }
        }
        else if(role == Qt::EditRole) {

            switch(index.column()) {
                case 1: return song->name();
                case 2: return song->artist();
                case 3: return song->album();
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
    if(index.isValid() && role == Qt::EditRole) {
        MSong *song = m_songlist.at(index.row());

        if(!song || (index.column() >= 1 && index.column() <= 3)) {
            return false;
        }

        Q_UNUSED(value)
//        MSong::MutableProperty attrib = MSong::SongName;
//        if(index.column() == 1) {
//            attrib  = MSong::SongName;
//        }
//        else if(index.column() == 2) {
//            attrib  = MSong::SongArtist;
//        }
//        else if(index.column() == 3) {
//            attrib  = MSong::SongAlbum;
//        }

//        m_mediaLibrary->modify(song, value.toString(), attrib);
//        emit dataChanged(index, index);
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

void MModelSonglist::viewAllSongs()
{
    m_currentView = MModelSonglist::ViewAllSongs;
    m_currentContainer = nullptr;
    emit viewChanged();
}
void MModelSonglist::viewArtists()
{
    m_currentView = MModelSonglist::ViewArtists;
    m_currentContainer = nullptr;
    emit viewChanged();
}
void MModelSonglist::viewAlbums()
{
    m_currentView = MModelSonglist::ViewAlbums;
    m_currentContainer = nullptr;
    emit viewChanged();
}
void MModelSonglist::viewContainer(MContainer *container)
{
    m_currentView = MModelSonglist::ViewContainer;
    m_currentContainer = container;
    emit viewChanged();
}

MModelSonglist::View MModelSonglist::currentView() const
{
    return m_currentView;
}
Mpi3::ElementType MModelSonglist::currentType() const
{
    if(m_currentView == MModelSonglist::ViewContainer){
        Q_ASSERT(m_currentContainer);
        return m_currentContainer->type();
    }

    Q_ASSERT(m_mediaLibrary);
    return Mpi3::LibraryElement;
}
QString MModelSonglist::currentPID() const
{
    if(currentType() == Mpi3::LibraryElement){
        return m_mediaLibrary->pid();
    }

    return m_currentContainer->pid();
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
    m_currentContainer = nullptr;
    m_currentView = MModelSonglist::ViewAllSongs;

    if(m_mediaLibrary) {
//        connect(m_mediaLibrary, &MMediaLibrary::elementModified, this, &MModelSonglist::elementModified);
//        connect(m_mediaLibrary, &MMediaLibrary::elementInserted, this, &MModelSonglist::elementInserted);
//        connect(m_mediaLibrary, &MMediaLibrary::elementRemoved, this, &MModelSonglist::elementRemoved);
//        connect(m_mediaLibrary, &MMediaLibrary::elementMoved, this, &MModelSonglist::elementMoved);
//        connect(m_mediaLibrary, &MMediaLibrary::elementDeleted, this, &MModelSonglist::elementDeleted);

        connect(m_mediaLibrary, &MMediaLibrary::libraryLoaded, this, [this]() {setLibrary(m_mediaLibrary);});
        connect(m_mediaLibrary, &MMediaLibrary::libraryReset, this, [this]() {setLibrary(m_mediaLibrary);});
    }

    removeRows(0, rowCount());
    endResetModel();

    emit viewChanged();
}
//void MModelSonglist::setContainer(MContainer *container)
//{
//    removeRows(0, rowCount());

//    m_mediaContainer = container;
//    m_songlist = container->songs();

//    insertRows(0, rowCount());
//}


//MContainer *MModelSonglist::container() const
//{
//    return m_mediaContainer;
//}
QVector<MSong*> MModelSonglist::songlist() const
{
    return m_songlist;
}


MSong *MModelSonglist::getSong(const QModelIndex &index) const
{
    if(index.row() > -1 && index.row() < m_songlist.size()) {
        return m_songlist.at(index.row());
    }

    return nullptr;
}


//void MModelSonglist::elementModified(MMediaElement *elemModified)
//{
//    if(elemModified->type() == Mpi3::SongElement) {
//        MSong *sc_song = static_cast<MSong*>(elemModified);

//        if(m_songlist.contains(sc_song)) {
//            int pos = m_songlist.indexOf(sc_song);
//            emit dataChanged(index(pos, 0), index(pos, columnCount()));
//        }
//    }
//}
//void MModelSonglist::elementInserted(MMediaElement *elemInserted, MMediaContainer *elemParent)
//{
//    if(elemInserted->type() == Mpi3::SongElement && elemParent == m_mediaContainer) {
//        MSong *sc_song = static_cast<MSong*>(elemInserted);

//        if(!m_songlist.contains(sc_song) && m_mediaContainer->songs().contains(sc_song)) {
//            int pos = m_mediaContainer->songs().indexOf(sc_song);
//            m_songlist.insert(pos, sc_song);
//            insertRows(pos, 1);
//        }
//    }
//}
////void MModelSonglist::elementRemoved(MMediaElement *elemRemoved, MMediaContainer *elemParent)
////{
////    if(elemRemoved->type() == Mpi3::SongElement && elemParent == m_mediaContainer) {
////        MSong *sc_song = static_cast<MSong*>(elemRemoved);

////        if(m_songlist.contains(sc_song) && !m_mediaContainer->songs().contains(sc_song)) {
////            removeRows(m_songlist.indexOf(sc_song), 1);
////            m_songlist.removeAll(sc_song);
////        }
////    }
////}
//void MModelSonglist::elementMoved(MMediaElement *elemMoved, MMediaContainer *elemParent)
//{
//    if(elemMoved->type() == Mpi3::SongElement && elemParent == m_mediaContainer) {
//        MSong *sc_song = static_cast<MSong*>(elemMoved);

//        int fromPosition = m_songlist.indexOf(sc_song);
//        int toPosition = elemParent->songs().indexOf(sc_song);

//        m_songlist.move(fromPosition, toPosition);
//    }
//}
//void MModelSonglist::elementDeleted(MMediaElement *elemDeleted)
//{
//    if(elemDeleted->type() == Mpi3::SongElement) {
//        MSong *sc_song = static_cast<MSong*>(elemDeleted);

//        if(m_songlist.contains(sc_song)) {
//            removeRows(m_songlist.indexOf(sc_song), 1);
//            m_songlist.removeAll(sc_song);
//        }
//    }
//}




MModelSonglistProxy::MModelSonglistProxy(QObject *parent) : QSortFilterProxyModel(parent)
{

}
