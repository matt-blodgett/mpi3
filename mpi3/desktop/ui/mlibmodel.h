#ifndef MLIBMODEL_H
#define MLIBMODEL_H

#include <QAbstractItemModel>
#include <QScopedPointer>

class Mpi3Library;
class LibraryItem;
class Mpi3Song;


class LibraryModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit LibraryModel( QObject *parent, const QStringList &headers);
    ~LibraryModel();

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

private slots:
    void insertItems(int position, int count);
    void changeItems(int position, Mpi3Song *s);

public:
    Mpi3Library *library() const;
    void setLibrary(Mpi3Library *library);

private:
    QScopedPointer<Mpi3Library> m_library;
    LibraryItem *getItem(const QModelIndex &index) const;
    LibraryItem *rootItem;

};


#endif // MLIBMODEL_H

