#ifndef MLIBMODEL_H
#define MLIBMODEL_H

#include <QAbstractItemModel>
#include <QScopedPointer>
#include <QIcon>
#include <QMap>
#include <QVector>
#include <QUrl>

QT_BEGIN_NAMESPACE
class QMimeData;
QT_END_NAMESPACE

class LibraryItem;
class Mpi3Library;
class Mpi3Folder;
class Mpi3Playlist;
class Mpi3Song;


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

//    bool match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const override;

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
    LibraryItem *rootItem = nullptr;
    QMap<QString, LibraryItem*> libItems;

    QScopedPointer<Mpi3Library> m_library;
    QScopedPointer<Mpi3Playlist> m_playlist;

public:
    QModelIndex getIndex(const QString &pid, QModelIndex parent = QModelIndex());

    LibraryItem *getItem(const QModelIndex &index) const;
    LibraryItem *getItem(const QString &pid) const;

    QString getPID(const QModelIndex &index) const;
    QString getPID(LibraryItem *item) const;

    void setLibrary(Mpi3Library *library);
    void populate(Mpi3Folder *parentFolder = nullptr, QModelIndex parent = QModelIndex());

private:
    void playlistInserted(Mpi3Playlist *playlist, Mpi3Folder *folder);
    void folderInserted(Mpi3Folder *folder, Mpi3Folder *parent);

public slots:
    void elementModified(const QString &pidModified);
    void elementInserted(const QString &pidInserted, const QString &pidParent);
    void elementRemoved(const QString &pidRemoved, const QString &pidParent);

};


class SonglistModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit SonglistModel(QObject *parent = nullptr);
    ~SonglistModel() override;

public:
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDragActions() const override;
    Qt::DropActions supportedDropActions() const override;

//    bool match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const override;

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

public:
    QMap<int, bool> columnVisibility;

private:
    QScopedPointer<Mpi3Library> m_library;
    QVector<Mpi3Song*> m_songlist;
    QStringList m_headers;

    Mpi3Playlist *m_playlist = nullptr;

public:
    QString getPID(const QModelIndex &index) const;

    void setLibrary(Mpi3Library *library);
    void setPlaylist(Mpi3Playlist *playlist);

public slots:
    void elementModified(const QString &pidModified);
    void elementInserted(const QString &pidInserted, const QString &pidParent);
    void elementRemoved(const QString &pidRemoved, const QString &pidParent);
};


#endif // MLIBMODEL_H
