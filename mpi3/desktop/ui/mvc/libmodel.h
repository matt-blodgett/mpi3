#ifndef LIBMODEL_H
#define LIBMODEL_H

#include <QAbstractItemModel>
#include <QScopedPointer>
#include <QIcon>
#include <QMap>

QT_BEGIN_NAMESPACE
class QMimeData;
QT_END_NAMESPACE

class LibraryItem;
class Mpi3Element;
class Mpi3Library;
class Mpi3Folder;
class Mpi3Playlist;
class Mpi3Song;


bool validMediaFiles(const QMimeData *data);


class LibraryModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit LibraryModel(QObject *parent = nullptr);
    ~LibraryModel() override;

public:
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDragActions() const override;
    Qt::DropActions supportedDropActions() const override;

    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;

    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex index(int row, int column, const QModelIndex &index = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    bool insertRows(int position, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int position, int count, const QModelIndex &parent = QModelIndex()) override;

    bool removeRows(int position, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    QIcon icn_folder;
    QIcon icn_playlist;

private:
    QScopedPointer<Mpi3Library> m_mediaLibrary;
    QMap<QString, LibraryItem*> m_libItems;
    LibraryItem *m_rootItem = nullptr;

public:
    QModelIndex getIndex(const QString &pid) const;
    LibraryItem *getItem(const QModelIndex &index) const;
    LibraryItem *getItem(const QString &pid) const;
    QString getPID(const QModelIndex &index) const;
    QString getPID(LibraryItem *item) const;

    void setLibrary(Mpi3Library *library);

private:
    void populate(Mpi3Folder *parentFolder = nullptr, LibraryItem *parentItem = nullptr);
    void playlistInserted(Mpi3Playlist *childPlaylist, Mpi3Element *parentElement);
    void folderInserted(Mpi3Folder *childFolder, Mpi3Element *parentElement);

private slots:
    void elementModified(Mpi3Element *elemModifed);
    void elementInserted(Mpi3Element *elemInserted, Mpi3Element *elemParent);
    void elementRemoved(Mpi3Element *elemRemoved, Mpi3Element *elemParent);
    void elementDeleted(const QString &pidDeleted, int elemType, QVector<QString> pidChildren);
};


class SonglistModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit SonglistModel(QObject *parent = nullptr);
    ~SonglistModel() override;

public:
    enum Display {
        DisplayAllSongs,
        DisplayArtists,
        DisplayAlbums,
        DisplayPlaylist,
        DisplayFolder
    };

public:
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDragActions() const override;
    Qt::DropActions supportedDropActions() const override;

    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;

    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex index(int row, int column, const QModelIndex &index = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    bool insertRows(int position, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int position, int count, const QModelIndex &parent = QModelIndex()) override;

    bool removeRows(int position, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    QStringList m_headers;
    QScopedPointer<Mpi3Library> m_mediaLibrary;
    SonglistModel::Display m_currentDisplay;
    QVector<Mpi3Song*> m_currentSonglist;

    QString m_pidParentContainer;
    QStringList m_pidChildContainers;

public:
    QMap<int, bool> columnVisibility;
    QString getPID(const QModelIndex &index) const;

public:
    void setLibrary(Mpi3Library *library);

    SonglistModel::Display currentDisplay() const;
    void setDisplay(SonglistModel::Display display);

    void setContainer(Mpi3Playlist *playlist);
    void setContainer(Mpi3Folder *folder);

private slots:
    void elementModified(Mpi3Element *elemModifed);
    void elementInserted(Mpi3Element *elemInserted, Mpi3Element *elemParent);
    void elementRemoved(Mpi3Element *elemRemoved, Mpi3Element *elemParent);
    void elementDeleted(const QString &pidDeleted, int elemType, QVector<QString> pidChildren);
};


#endif
