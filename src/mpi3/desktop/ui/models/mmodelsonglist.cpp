#include "mmodelsonglist.h"
#include "mmodelsonglistitem.h"

#include "mformat.h"

#include "mmediautil.h"

#include <QMimeData>
#include <QUrl>


#include <QDebug>


typedef MModelSonglistItem MModelItem;


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
        MSong *song = m_mediaLibrary->getSong(m_songlist.at(idx.row()).pid());
        if(song && !selectedSongs.contains(song)) {
            selectedSongs.prepend(song);
        }
    }

//    QByteArray pidBytes = MMediaLibrary::songsToBytes(selectedSongs);
//    QList<QUrl> songUrls = MMediaLibrary::songsToPaths(selectedSongs);
    QByteArray pidBytes = Mpi3::Core::songsToBytes(selectedSongs);
    QList<QUrl> songUrls = Mpi3::Core::songsToPaths(selectedSongs);

    mData->setData(QMetaType::typeName(qMetaTypeId<QStringList>()), pidBytes);
    mData->setUrls(songUrls);

    return mData;
}

bool MModelSonglist::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
//    Q_UNUSED(data)
//    Q_UNUSED(action)
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

    MPlaylist *playlist = m_mediaLibrary->getPlaylist(m_pid);
    if(playlist || m_pid == m_mediaLibrary->pid()) {
        bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
//        bool dataIsValidMediaFiles = data->hasUrls() ? MMediaLibrary::validMediaFiles(data->urls()) : false;
        bool dataIsValidMediaFiles = data->hasUrls() ? Mpi3::Core::validMediaFiles(data->urls()) : false;
        bool actionIsCopyAction = action == Qt::CopyAction;

        return (dataIsSonglist && playlist) || (dataIsValidMediaFiles && actionIsCopyAction);
    }

    return false;
}
bool MModelSonglist::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
//    Q_UNUSED(data)
//    Q_UNUSED(action)
//    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

    MPlaylist *playlist = m_mediaLibrary->getPlaylist(m_pid);

    if(playlist || m_pid == m_mediaLibrary->pid()) {
        bool dataIsSonglist = data->hasFormat(QMetaType::typeName(qMetaTypeId<QStringList>()));
//        bool dataIsValidMediaFiles = data->hasUrls() ? MMediaLibrary::validMediaFiles(data->urls()) : false;
        bool dataIsValidMediaFiles = data->hasUrls() ? Mpi3::Core::validMediaFiles(data->urls()) : false;
        bool actionIsCopyAction = action == Qt::CopyAction;
        bool actionIsMoveAction = action == Qt::MoveAction;

        row = row < 0 ? rowCount() : row;

        if(dataIsSonglist && playlist) {
            QByteArray pidBytes = data->data(QMetaType::typeName(qMetaTypeId<QStringList>()));
            QStringList pidStrings = Mpi3::Core::bytesToSongs(pidBytes);


            QVector<MSong*> droppedSongs;
            for(QString pid : pidStrings){
                MSong *song = m_mediaLibrary->getSong(pid);
                droppedSongs.append(song);
            }

            if(actionIsCopyAction) {
                playlist->insert(row, droppedSongs);
            }
            else if(actionIsMoveAction) {
                QStringList pids;
                for(MModelItem item : m_songlist){
                    pids.append(item.pid());
                }

                QList<int> indexes;
                for(MSong *s : droppedSongs){
                    indexes.append(pids.indexOf(s->pid()));
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

QModelIndex MModelSonglist::index(int row, int column, const QModelIndex &index) const
{
    Q_UNUSED(index);
    return createIndex(row, column);
}

int MModelSonglist::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_songlist.size();
}
int MModelSonglist::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_headers.size();
}

QVariant MModelSonglist::data(const QModelIndex &index, int role) const
{
    if(index.isValid()) {
        int row = index.row();
        int col = index.column();

        // move value to after if stmt - dont need before that (role)
        const QVariant value = m_songlist.at(row).data(col);

        if(role == Qt::DisplayRole){
            if(col == 0){
                return row + 1;
            }

            return value;
        }
        else if(role == Qt::EditRole && flags(index).testFlag(Qt::ItemIsEditable)){
            return value;
        }
    }

    return QVariant();
}
bool MModelSonglist::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.isValid()){
        int row = index.row();
        int col = index.column();

        if(role == Qt::EditRole && flags(index).testFlag(Qt::ItemIsEditable)){
            bool result = m_songlist[row].setData(col, value);

            if(result){
                emit dataChanged(index, index);
            }

            return result;
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

MMediaLibrary *MModelSonglist::library() const
{
    return m_mediaLibrary;
}
void MModelSonglist::setLibrary(MMediaLibrary *library)
{
    if(m_mediaLibrary) {
        disconnect(m_mediaLibrary, nullptr, this, nullptr);
    }

    m_mediaLibrary = library;
    connect(m_mediaLibrary, &MMediaLibrary::playlistContentsChanged, this, &MModelSonglist::playlistContentsChanged);
}

void MModelSonglist::setSonglist(MSongList songs, const QString &pid)
{
    beginResetModel();

    m_songlist.clear();
    m_pid = pid;

    for(int i = 0; i < songs.size(); i++){
       MModelItem item = MModelItem();

        MSong *s = songs.at(i);
        item.setPID(s->pid());

        int index = 1;
        item.setData(index++, s->name());
        item.setData(index++, s->artist());
        item.setData(index++, s->album());
        item.setData(index++, Mpi3::Util::timeToString(s->time()));
        item.setData(index++, Mpi3::Util::sizeToString(s->size()));
//        item.setData(index++, s->timeString());
//        item.setData(index++, s->sizeString());
        item.setData(index++, s->kind());
        item.setData(index++, s->path());
        item.setData(index++, s->bitRate());
        item.setData(index++, s->added());
        item.setData(index++, "");
        item.setData(index++, 0);
        item.setData(index++, 0);
        item.setData(index++, "");
        item.setData(index++, "");

        m_songlist.append(item);
    }

    endResetModel();
}
QString MModelSonglist::currentPID() const
{
    return m_pid;
}

void MModelSonglist::playlistContentsChanged(MPlaylist *p)
{
    if(m_pid == p->pid()){
        setSonglist(p->songs(), p->pid());
    }
}
