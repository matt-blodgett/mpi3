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
    explicit LibraryModel(QObject *parent, const QStringList &headers);
    ~LibraryModel();

public:
    enum View {
        Library,
        Artists,
        Containers,
        Playlist
    };

    Qt::ItemFlags flags(const QModelIndex &index) const override;

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

public:
    QMap<int, bool> columnVisibility;

public:
    Mpi3Library *library() const;
    Mpi3Playlist *playlist() const;

    void setLibrary(Mpi3Library *library);
    void setPlaylist(Mpi3Playlist *playlist);
    void setView(LibraryModel::View view);

private:
    QScopedPointer<Mpi3Library> m_library;
    QScopedPointer<Mpi3Playlist> m_playlist;

private:
    LibraryItem *getItem(const QModelIndex &index) const;
    LibraryItem *rootItem = nullptr;

private slots:
    void insertItems(int position, int count);
    void changeItems(int position, Mpi3Song *s);

};


#endif // MLIBMODEL_H

