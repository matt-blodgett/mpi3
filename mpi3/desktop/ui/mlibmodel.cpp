#include "mlibmodel.h"
#include "mlibitem.h"
#include "util/medialib.h"


#include <QDebug>


LibraryModel::LibraryModel(QObject *parent, const QStringList &headers) : QAbstractItemModel(parent){
//    columnVisibility = new QMap<int, bool>;

    QVector<QVariant> rootData;
    foreach (QString header, headers){
        rootData << header;
    }

    rootItem = new LibraryItem(rootData);
}

LibraryModel::~LibraryModel(){
    delete rootItem;
}

Qt::ItemFlags LibraryModel::flags(const QModelIndex &index) const{
    if (!index.isValid()){
        return 0;
    }

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

int LibraryModel::rowCount(const QModelIndex &parent) const{
    LibraryItem *parentItem = getItem(parent);
    return parentItem->childCount();
}

int LibraryModel::columnCount(const QModelIndex &) const{
    return rootItem->columnCount();
}

QModelIndex LibraryModel::index(int row, int column, const QModelIndex &parent) const{
    if (parent.isValid() && parent.column() != 0){
        return QModelIndex();
    }

    LibraryItem *parentItem = getItem(parent);
    LibraryItem *childItem = parentItem->child(row);
    if (childItem){
        return createIndex(row, column, childItem);
    }
    else{
        return QModelIndex();
    }
}

QModelIndex LibraryModel::parent(const QModelIndex &child) const{
    Q_UNUSED(child);
    return QModelIndex();
}

QVariant LibraryModel::data(const QModelIndex &index, int role) const{
    if (!index.isValid()){
        return QVariant();
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole){
        return QVariant();
    }

    LibraryItem *item = getItem(index);
    return item->data(index.column());
}

QVariant LibraryModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        return rootItem->data(section);
    }

    return QVariant();
}

bool LibraryModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if (role != Qt::EditRole){
        return false;
    }

    LibraryItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result){
        emit dataChanged(index, index);
    }

    return result;
}

bool LibraryModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role){
    if (role != Qt::EditRole || orientation != Qt::Horizontal){
        return false;
    }

    bool result = rootItem->setData(section, value);

    if (result){
        emit headerDataChanged(orientation, section, section);
    }

    return result;
}








//bool LibraryModel::insertColumns(int position, int columns, const QModelIndex &parent)
//{
//    bool success;

//    beginInsertColumns(parent, position, position + columns - 1);
//    success = rootItem->insertColumns(position, columns);
//    endInsertColumns();

//    return success;
//}

//bool LibraryModel::removeColumns(int position, int columns, const QModelIndex &parent)
//{
//    bool success;

//    beginRemoveColumns(parent, position, position + columns - 1);
//    success = rootItem->removeColumns(position, columns);
//    endRemoveColumns();

//    if (rootItem->columnCount() == 0)
//        removeRows(0, rowCount());

//    return success;
//}

//bool LibraryModel::insertRows(int position, int rows, const QModelIndex &parent)
//{
//    LibraryItem *parentItem = getItem(parent);
//    bool success;

//    beginInsertRows(parent, position, position + rows - 1);
//    success = parentItem->insertChildren(position, rows, parentItem->columnCount());
//    endInsertRows();

//    return success;
//}

//bool LibraryModel::removeRows(int position, int rows, const QModelIndex &parent)
//{
//    LibraryItem *parentItem = getItem(parent);
//    bool success = true;

//    beginRemoveRows(parent, position, position + rows - 1);
//    success = parentItem->removeChildren(position, rows);
//    endRemoveRows();

//    return success;
//}











void LibraryModel::insertItems(int position, int count){
    beginInsertRows(QModelIndex(), position, position + count);
    rootItem->insertChildren(position, count, rootItem->columnCount());
    endInsertRows();
}

void LibraryModel::changeItems(int position, Mpi3Song *s){
    rootItem->child(position)->setData(0, s->name);
    rootItem->child(position)->setData(1, s->artist);
    rootItem->child(position)->setData(2, s->path);
}






Mpi3Library *LibraryModel::library() const{
    return m_library.data();
}

void LibraryModel::setLibrary(Mpi3Library *library){
    if (m_library) {
        disconnect(m_library.data(), &Mpi3Library::mediaInserted, this, &LibraryModel::insertItems);
        disconnect(m_library.data(), &Mpi3Library::mediaChanged, this, &LibraryModel::changeItems);
    }

    beginResetModel();
    m_library.reset(library);

    if (m_library) {
        connect(m_library.data(), &Mpi3Library::mediaInserted, this, &LibraryModel::insertItems);
        connect(m_library.data(), &Mpi3Library::mediaChanged, this, &LibraryModel::changeItems);
    }

    endResetModel();

    for(int i = 0; i < this->columnCount(); i++){
        columnVisibility[i] = false;
    }
}

LibraryItem *LibraryModel::getItem(const QModelIndex &index) const{
    if (index.isValid()) {
        LibraryItem *item = static_cast<LibraryItem*>(index.internalPointer());
        if (item){
            return item;
        }
    }
    return rootItem;
}




























