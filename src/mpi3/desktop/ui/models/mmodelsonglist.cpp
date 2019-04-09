#include "mmodelsonglist.h"
#include "mmodelsonglistitem.h"

#include <QMimeData>
#include <QUrl>


#include <QDebug>



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
    Q_UNUSED(indexes);

    QMimeData *mData = new QMimeData();

//    QVector<MSong*> selectedSongs;
//    for(QModelIndex idx : indexes) {
//        MSong *song = getSong(idx);
//        if(song && !selectedSongs.contains(song)) {
//            selectedSongs.prepend(song);
//        }
//    }

//    QByteArray pidBytes = MMediaLibrary::songsToBytes(selectedSongs);
//    QList<QUrl> songUrls = MMediaLibrary::songsToPaths(selectedSongs);

//    mData->setData(QMetaType::typeName(qMetaTypeId<QStringList>()), pidBytes);
//    mData->setUrls(songUrls);

    return mData;
}

bool MModelSonglist::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(data);
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

    return false;
}
bool MModelSonglist::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(data);
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

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

bool MModelSonglist::insertRows(int position, int count, const QModelIndex &parent)
{
    if(count > 0) {
        int last = position + count - 1;

        beginInsertRows(parent, position, last);

        for(int i = position; i < last; i++){
            m_songlist.insert(i, MModelSonglistItem());
        }

        endInsertRows();
        return true;
    }

    return false;
}
bool MModelSonglist::removeRows(int position, int count, const QModelIndex &parent)
{
    if(count > 0) {
        int last = position + count - 1;

        beginRemoveRows(parent, position, last);

        for(int i = position; i < last; i++){
            m_songlist.removeAt(position);
        }

        endRemoveRows();
        return true;
    }

    return false;
}

QVariant MModelSonglist::data(const QModelIndex &index, int role) const
{
    if(index.isValid()) {
        int row = index.row();
        int col = index.column();

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

void MModelSonglist::setSonglist(MSongList songs)
{
    beginResetModel();
    removeRows(0, rowCount());
    insertRows(0, songs.size());

    for(int i = 0; i < songs.size(); i++){
        MModelSonglistItem item = m_songlist[i];
        MSong *s = songs.at(i);

        int index = 1;
        item.setData(index++, s->name());
        item.setData(index++, s->artist());
        item.setData(index++, s->album());
        item.setData(index++, s->timeString());
        item.setData(index++, s->sizeString());
        item.setData(index++, s->kind());
        item.setData(index++, s->path());
        item.setData(index++, s->bitRate());
        item.setData(index++, s->added());
        item.setData(index++, "");
        item.setData(index++, 0);
        item.setData(index++, 0);
        item.setData(index++, "");
        item.setData(index++, "");
    }
}
