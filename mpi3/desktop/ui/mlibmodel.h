#ifndef MLIBMODEL_H
#define MLIBMODEL_H

#include <QAbstractItemModel>
#include <QScopedPointer>
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
        Library,
        Artists,
        Albums,
        Containers,
        Playlist
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
    View currentView();
    QMap<int, bool> columnVisibility;

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

private:
    LibraryItem *getItem(const QModelIndex &index) const;
    LibraryItem *rootItem = nullptr;

    QScopedPointer<Mpi3Library> m_library;
    QMap<QString, LibraryItem*> libItems;

private slots:
    void insertItems(int position, int count);
    void changeItems(int position, Mpi3Song *s);
    void removeItems(int position, int count);

};


#endif // MLIBMODEL_H

