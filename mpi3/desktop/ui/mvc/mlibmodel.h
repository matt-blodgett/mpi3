#ifndef MLIBMODEL_H
#define MLIBMODEL_H

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


class Mpi3ModelContainers : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit Mpi3ModelContainers(QObject *parent = nullptr);
    ~Mpi3ModelContainers() override;

public:
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDragActions() const override;
    Qt::DropActions supportedDropActions() const override;

    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;

    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

//    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

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
    QIcon m_icnFolder;
    QIcon m_icnPlaylist;

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

    Mpi3Folder *getParentFolderAt(const QModelIndex &index) const;

    void setLibrary(Mpi3Library *library);

private:
    void populate(Mpi3Folder *parentFolder = nullptr, LibraryItem *parentItem = nullptr);
    void playlistInserted(Mpi3Playlist *childPlaylist, Mpi3Element *parentElement);
    void folderInserted(Mpi3Folder *childFolder, Mpi3Element *parentElement);

private:
    void elementModified(Mpi3Element *elemModified);
    void elementInserted(Mpi3Element *elemInserted, Mpi3Element *elemParent);
    void elementRemoved(Mpi3Element *elemRemoved, Mpi3Element *elemParent);
    void elementMoved(Mpi3Element *elemMoved, Mpi3Element *elemParent);
    void elementDeleted(const QString &pidDeleted, int elemType, QVector<QString> pidChildren);
};


class Mpi3ModelSonglist : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit Mpi3ModelSonglist(QObject *parent = nullptr);
    ~Mpi3ModelSonglist() override;

public:
    enum Display {
        DisplayNone,
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
    Mpi3ModelSonglist::Display m_currentDisplay;
    QVector<Mpi3Song*> m_currentSonglist;
    QString m_currentContainer;

public:
    QMap<int, bool> columnVisibility;
    Mpi3Song *getSongAt(const QModelIndex &index) const;

public:
    void setLibrary(Mpi3Library *library);

    QString currentContainer() const;
    Mpi3ModelSonglist::Display currentDisplay() const;
    void setDisplay(Mpi3ModelSonglist::Display display);
    void setDisplay(Mpi3Element *container);

private:
    void elementModified(Mpi3Element *elemModified);
    void elementInserted(Mpi3Element *elemInserted, Mpi3Element *elemParent);
    void elementRemoved(Mpi3Element *elemRemoved, Mpi3Element *elemParent);
    void elementMoved(Mpi3Element *elemMoved, Mpi3Element *elemParent);
    void elementDeleted(const QString &pidDeleted, int elemType, QVector<QString> pidChildren);
};


#endif
