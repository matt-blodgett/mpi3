#ifndef MLIBRARYMODEL_H
#define MLIBRARYMODEL_H

#include <QAbstractItemModel>
#include <QScopedPointer>

class Mpi3Library;

class TreeItem;


class LibraryModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    explicit LibraryModel( QObject *parent, const QStringList &headers);
    ~LibraryModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;


    Mpi3Library *library() const;
    void setLibrary(Mpi3Library *library);

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole) override;


//private slots:
//    void beginInsertItems(int start, int end);
//    void endInsertItems();
//    void beginRemoveItems(int start, int end);
//    void endRemoveItems();
//    void changeItems(int start, int end);

private:
    QScopedPointer<Mpi3Library> m_library;
    QMap<QModelIndex, QVariant> m_data;



public:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;

private:
    TreeItem *getItem(const QModelIndex &index) const;

    TreeItem *rootItem;
};


#endif // MLIBRARYMODEL_H

