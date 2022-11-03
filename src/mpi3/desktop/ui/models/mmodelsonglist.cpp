#include "mpi3/desktop/ui/models/mmodelsonglist.h"
#include "mpi3/desktop/ui/models/mmodelsonglistitem.h"
#include "mpi3/core/mmedialibrary.h"
#include "mpi3/core/mmediautil.h"
#include "mpi3/util/mformat.h"

#include <QMimeData>
#include <QUrl>


#include <QDebug>


typedef MModelSonglistItem MModelItem;


static void populateItem(MModelItem *item, MSong *song)
{
    item->setPID(song->pid());

    int index = 1;
    item->setData(index++, song->name());
    item->setData(index++, song->artist());
    item->setData(index++, song->album());
    item->setData(index++, Mpi3::Util::timeToString(song->time()));
    item->setData(index++, Mpi3::Util::sizeToString(song->size()));
    item->setData(index++, song->kind());
    item->setData(index++, song->path());
    item->setData(index++, song->bitRate());
    item->setData(index++, song->sampleRate());
    item->setData(index++, "");
    item->setData(index++, "");
    item->setData(index++, 0);
    item->setData(index++, 0);
    item->setData(index++, "");
    item->setData(index++, "");
}


MModelSonglist::MModelSonglist(QObject *parent) : QAbstractTableModel(parent)
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

    mTypes << MPI3_MIME_TYPE_PIDS_SONGS;

    return mTypes;
}
QMimeData *MModelSonglist::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mData = new QMimeData();

    QVector<MSong*> selectedSongs;
    for(QModelIndex idx : indexes) {
        MSong *song = m_mediaLibrary->getSong(m_songList.at(idx.row())->pid());
        if(song && !selectedSongs.contains(song)) {
            selectedSongs.prepend(song);
        }
    }

    QByteArray pidBytes = Mpi3::Core::songsToBytes(selectedSongs);
    QList<QUrl> songUrls = Mpi3::Core::songsToPaths(selectedSongs);

    mData->setData(MPI3_MIME_TYPE_PIDS_SONGS, pidBytes);
    mData->setUrls(songUrls);

    return mData;
}

bool MModelSonglist::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(row)
    Q_UNUSED(column)
    Q_UNUSED(parent)

    MPlaylist *playlist = m_mediaLibrary->getPlaylist(m_pid);
    if(playlist || m_pid == m_mediaLibrary->pid()) {
        bool dataIsSonglist = data->hasFormat(MPI3_MIME_TYPE_PIDS_SONGS);
        bool dataIsValidMediaFiles = data->hasUrls() ? Mpi3::Core::validMediaFiles(data->urls()) : false;
        bool actionIsCopyAction = action == Qt::CopyAction;
        return (dataIsSonglist && playlist) || (dataIsValidMediaFiles && actionIsCopyAction);
    }

    return false;
}
bool MModelSonglist::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(column)
    Q_UNUSED(parent)

    MPlaylist *playlist = m_mediaLibrary->getPlaylist(m_pid);

    if(playlist || m_pid == m_mediaLibrary->pid()) {
        bool dataIsSonglist = data->hasFormat(MPI3_MIME_TYPE_PIDS_SONGS);
        bool dataIsValidMediaFiles = data->hasUrls() ? Mpi3::Core::validMediaFiles(data->urls()) : false;
        bool actionIsCopyAction = action == Qt::CopyAction;
        bool actionIsMoveAction = action == Qt::MoveAction;

        row = row < 0 ? rowCount() : row;

        if(dataIsSonglist && playlist) {
            QByteArray pidBytes = data->data(MPI3_MIME_TYPE_PIDS_SONGS);
            QStringList pidStrings = Mpi3::Core::bytesToSongs(pidBytes);

            if(actionIsCopyAction) {
                QStringList pidStringsCombined = playlist->songsPidList();
                for (const QString &pidString : pidStrings) {
                    pidStringsCombined.insert(row++, pidString);
                }
                m_mediaLibrary->edit(playlist, "songs", pidStringsCombined);
                return true;
            }
            else if(actionIsMoveAction) {
                return false;
//                QStringList pids;
//                for(MModelItem item : m_songlist) {
//                    pids.append(item.pid());
//                }

//                QList<int> indexes;
//                for(MSong *s : droppedSongs) {
//                    indexes.append(pids.indexOf(s->pid()));
//                }

//                playlist->move(indexes, row);
            }

            return true;
        }
        else if(dataIsValidMediaFiles && actionIsCopyAction) {
            QStringList pidStrings;

            for(const QUrl &url : data->urls()) {
                MSong *song = m_mediaLibrary->newSong(url);
                if (song) {
                    pidStrings << song->pid();
                }
            }

            if(playlist) {
                QStringList pidStringsCombined = playlist->songsPidList();
                for (const QString &pidString : pidStrings) {
                    pidStringsCombined.insert(row++, pidString);
                }
                m_mediaLibrary->edit(playlist, "songs", pidStringsCombined);
                return true;
            }

            return true;
        }
    }

    return false;
}

QModelIndex MModelSonglist::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    MModelItem *item = m_songList.at(row);
    if(item) {
        return createIndex(row, column, item);
    }

    return QModelIndex();
}

int MModelSonglist::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_songList.size();
}
int MModelSonglist::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_headers.size();
}

QVariant MModelSonglist::data(const QModelIndex &index, int role) const
{
    if(index.isValid()) {
        int row = index.row();
        int col = index.column();

        if(role == Qt::DisplayRole) {
            if(col == 0) {
                return row + 1;
            }

            return m_songList.at(row)->data(col);
        }
        else if(role == Qt::EditRole && flags(index).testFlag(Qt::ItemIsEditable)) {
            return m_songList.at(row)->data(col);
        }
    }

    return QVariant();
}
bool MModelSonglist::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid()) {
        int row = index.row();
        int col = index.column();

        if(role == Qt::EditRole && flags(index).testFlag(Qt::ItemIsEditable)) {
            MSong *song = m_mediaLibrary->getSong(m_songList.at(row)->pid());

            switch(col) {
                case 1: {
                    m_mediaLibrary->edit(song, "name", value);
                    break;
                }
                case 2: {
                    m_mediaLibrary->edit(song, "artist", value);
                    break;
                }
                case 3: {
                    m_mediaLibrary->edit(song, "album", value);
                    break;
                }
            }

            return true;
        }
    }

    return false;
}

QVariant MModelSonglist::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return m_headers.at(section);
    }

    return QVariant();
}

MModelItem *MModelSonglist::getItem(const QModelIndex &index) const
{
    if(index.isValid()) {
        MModelItem *item = static_cast<MModelItem*>(index.internalPointer());
        if(item) {
            return item;
        }
    }

    return nullptr;
}

QModelIndex MModelSonglist::pidToIndex(const QString &pid) const
{
    for (int i = 0; i < m_songList.length(); i++) {
        if (m_songList.at(i)->pid() == pid) {
            return index(i, 0);
        }
    }

    return QModelIndex();
}
QString MModelSonglist::pidFromIndex(const QModelIndex &index) const
{
    return m_songList.at(index.row())->pid();
}

QString MModelSonglist::pidCurrentSonglist() const
{
    return m_pid;
}
void MModelSonglist::setSongList(MSongList songs, const QString &pid)
{
    beginResetModel();

    qDeleteAll(m_songList);
    m_songList.clear();
    m_pid = pid;

    for(MSong *song : songs) {
        MModelItem *item = new MModelItem(this);
        populateItem(item, song);
        m_songList.append(item);
    }

    endResetModel();
}

void MModelSonglist::setLibrary(MMediaLibrary *library)
{
    if(m_mediaLibrary) {
        disconnect(m_mediaLibrary, nullptr, this, nullptr);
    }

    m_mediaLibrary = library;
    connect(m_mediaLibrary, &MMediaLibrary::songCreated, this, &MModelSonglist::songCreated);
    connect(m_mediaLibrary, &MMediaLibrary::songDeleted, this, &MModelSonglist::songDeleted);
    connect(m_mediaLibrary, &MMediaLibrary::songChanged, this, &MModelSonglist::songChanged);
    connect(m_mediaLibrary, &MMediaLibrary::playlistSongsChanged, this, &MModelSonglist::playlistSongsChanged);
    connect(m_mediaLibrary, &MMediaLibrary::libraryReset, this, [this](){setSongList(m_mediaLibrary->songs(), m_mediaLibrary->pid());});
}

void MModelSonglist::songCreated(MSong *song)
{
    if(m_pid == m_mediaLibrary->pid()) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        MModelItem *item = new MModelItem(this);
        populateItem(item, song);
        m_songList.append(item);
        endInsertRows();
    }
}
void MModelSonglist::songDeleted(MSong *song)
{
    QList<MModelItem*> removeItems;
    for(MModelItem *item : m_songList) {
        if(item->pid() == song->pid()) {
            removeItems.append(item);
        }
    }

    for(MModelItem *item : removeItems) {
        int row = m_songList.indexOf(item);
        beginRemoveRows(QModelIndex(), row, row);
        delete m_songList.takeAt(row);
        endRemoveRows();
    }
}
void MModelSonglist::songChanged(MSong *song)
{
    for(MModelItem *item : m_songList) {
        if(item->pid() == song->pid()) {
            populateItem(item, song);
            int row = m_songList.indexOf(item);
            emit dataChanged(index(row, 0), index(row, columnCount() - 1));
        }
    }
}
void MModelSonglist::playlistSongsChanged(MPlaylist *playlist)
{
    if(m_pid == playlist->pid()) {
        setSongList(playlist->songs(), playlist->pid());
    }
}
