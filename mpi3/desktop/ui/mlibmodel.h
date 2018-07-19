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
    ~LibraryModel();

public:
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDragActions() const override;
    Qt::DropActions supportedDropActions() const override;

//    bool match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const override;

    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;

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
    LibraryItem *getItem(const QModelIndex &index) const;
    LibraryItem *getItem(const QString &pid) const;

    QString getPID(const QModelIndex &index) const;
    QString getPID(LibraryItem *item) const;

    QModelIndex currentIndex();
    void setCurrentIndex(QModelIndex &index);

    Mpi3Library *library() const;
    void setLibrary(Mpi3Library *library);

public:
    void insertFolder();
    void insertPlaylist();

private:
    void recurseFolder(LibraryItem *item, Mpi3Folder *folder);
    QIcon icn_folder;
    QIcon icn_playlist;

private:
    LibraryItem *rootItem = nullptr;
    QMap<QString, LibraryItem*> libItems;

    QModelIndex m_currentIndex = QModelIndex();
    QScopedPointer<Mpi3Library> m_library;
    QScopedPointer<Mpi3Playlist> m_playlist;
};


class SonglistModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit SonglistModel(QObject *parent = nullptr);
    ~SonglistModel();

public:
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDragActions() const override;
    Qt::DropActions supportedDropActions() const override;

//    bool match(const QModelIndex &start, int role, const QVariant &value, int hits, Qt::MatchFlags flags) const override;

    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;

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
    Mpi3Library *library() const;
    void setLibrary(Mpi3Library *library);

    QString getPID(const QModelIndex &index) const;

    void setSonglist(QVector<Mpi3Song*> songlist);


private:
    QScopedPointer<Mpi3Library> m_library;
    QVector<Mpi3Song*> m_songlist;
    QStringList m_headers;

public slots:
    void dropExternalFiles(QModelIndex index, QList<QUrl> urls);

};


#endif // MLIBMODEL_H
