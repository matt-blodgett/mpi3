#ifndef MLIBMODEL_H
#define MLIBMODEL_H

#include <QAbstractItemModel>
#include <QScopedPointer>
#include <QIcon>
#include <QMap>

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
    enum View {
        ViewLibrary,
        ViewArtists,
        ViewAlbums,
        ViewContainers,
        ViewPlaylists
    };

    Qt::ItemFlags flags(const QModelIndex &index) const override;

public:
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
    QString getPID(LibraryItem *item) const;

    QMap<int, bool> columnVisibility;
    View currentView();

private:
    bool setView(LibraryModel::View view);
    View m_currentView;

public:
    Mpi3Library *library() const;
    void setLibrary(Mpi3Library *library);

    void viewLibrarySonglist();
    void viewLibraryContainers();
    void viewLibraryArtists();
    void viewPlaylist(Mpi3Playlist *playlist);

private:
    void insertFolder(LibraryItem *item, Mpi3Folder *folder);
    QIcon icn_folder;
    QIcon icn_playlist;

    void insertChildren(int position, int count);
    void removeChildren(int position, int count);

private:
    LibraryItem *rootItem = nullptr;
    QMap<QString, LibraryItem*> libItems;
    QScopedPointer<Mpi3Library> m_library;
    QScopedPointer<Mpi3Playlist> m_playlist;

private slots:
    void songUpdated(Mpi3Song *song);
    void playlistUpdated(Mpi3Playlist *playlist);
    void folderUpdated(Mpi3Folder *folder);

    void songInserted(Mpi3Song *song, Mpi3Playlist *parent, int position);
    void playlistInserted(Mpi3Playlist *playlist, Mpi3Folder *parent, int position);
    void folderInserted(Mpi3Folder *folder, Mpi3Folder *parent, int position);

};


#endif // MLIBMODEL_H
