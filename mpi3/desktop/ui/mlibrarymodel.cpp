#include "mlibrarymodel.h"
#include "mlibraryitem.h"
#include "util/medialib.h"


#include <QFileInfo>
#include <QUrl>


#include <QDebug>


LibraryModel::LibraryModel(QObject *parent) : QAbstractItemModel(parent)
{

}

LibraryModel::~LibraryModel()
{

}

int LibraryModel::rowCount(const QModelIndex &parent) const
{
    return m_library && !parent.isValid() ? m_library->mediaCount() : 0;
}

int LibraryModel::columnCount(const QModelIndex &parent) const
{
    return !parent.isValid() ? ColumnCount : 0;
}

QModelIndex LibraryModel::index(int row, int column, const QModelIndex &parent) const
{
    return m_library && !parent.isValid()
            && row >= 0 && row < m_library->mediaCount()
            && column >= 0 && column < ColumnCount
        ? createIndex(row, column)
        : QModelIndex();
}

QModelIndex LibraryModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);

    return QModelIndex();
}

QVariant LibraryModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole) {
        QVariant value = m_data[index];
        if (!value.isValid() && index.column() == Title) {
            QUrl location = m_library->songs->values().at(index.row())->path;
            return QFileInfo(location.path()).fileName();
        }

        return value;
    }
    return QVariant();
}




Mpi3Library *LibraryModel::library() const
{
    return m_library.data();
}

void LibraryModel::setLibrary(Mpi3Library *library)
{
    if (m_library) {
        disconnect(m_library.data(), &Mpi3Library::mediaAboutToBeInserted, this, &LibraryModel::beginInsertItems);
        disconnect(m_library.data(), &Mpi3Library::mediaInserted, this, &LibraryModel::endInsertItems);
        disconnect(m_library.data(), &Mpi3Library::mediaAboutToBeRemoved, this, &LibraryModel::beginRemoveItems);
        disconnect(m_library.data(), &Mpi3Library::mediaRemoved, this, &LibraryModel::endRemoveItems);
        disconnect(m_library.data(), &Mpi3Library::mediaChanged, this, &LibraryModel::changeItems);
    }

    beginResetModel();
    m_library.reset(library);

    if (m_library) {
        connect(m_library.data(), &Mpi3Library::mediaAboutToBeInserted, this, &LibraryModel::beginInsertItems);
        connect(m_library.data(), &Mpi3Library::mediaInserted, this, &LibraryModel::endInsertItems);
        connect(m_library.data(), &Mpi3Library::mediaAboutToBeRemoved, this, &LibraryModel::beginRemoveItems);
        connect(m_library.data(), &Mpi3Library::mediaRemoved, this, &LibraryModel::endRemoveItems);
        connect(m_library.data(), &Mpi3Library::mediaChanged, this, &LibraryModel::changeItems);
    }

    endResetModel();

}

bool LibraryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role);
    m_data[index] = value;
    emit dataChanged(index, index);
    return true;
}






void LibraryModel::beginInsertItems(int start, int end)
{
    m_data.clear();
    beginInsertRows(QModelIndex(), start, end);
}

void LibraryModel::endInsertItems()
{
    endInsertRows();
}

void LibraryModel::beginRemoveItems(int start, int end)
{
    m_data.clear();
    beginRemoveRows(QModelIndex(), start, end);
}

void LibraryModel::endRemoveItems()
{
    endInsertRows();
}

void LibraryModel::changeItems(int start, int end)
{
    m_data.clear();
    emit dataChanged(index(start,0), index(end,ColumnCount));
}









QVariant LibraryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}



bool LibraryModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}




TreeItem *LibraryModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

void LibraryModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
    Q_UNUSED(lines);
    Q_UNUSED(parent);
//    QList<TreeItem*> parents;
//    QList<int> indentations;
//    parents << parent;
//    indentations << 0;

//    int number = 0;

//    while (number < lines.count()) {
//        int position = 0;
//        while (position < lines[number].length()) {
//            if (lines[number].at(position) != ' ')
//                break;
//            ++position;
//        }

//        QString lineData = lines[number].mid(position).trimmed();

//        if (!lineData.isEmpty()) {
//            // Read the column data from the rest of the line.
//            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
//            QVector<QVariant> columnData;
//            for (int column = 0; column < columnStrings.count(); ++column)
//                columnData << columnStrings[column];

//            if (position > indentations.last()) {
//                // The last child of the current parent is now the new parent
//                // unless the current parent has no children.

//                if (parents.last()->childCount() > 0) {
//                    parents << parents.last()->child(parents.last()->childCount()-1);
//                    indentations << position;
//                }
//            } else {
//                while (position < indentations.last() && parents.count() > 0) {
//                    parents.pop_back();
//                    indentations.pop_back();
//                }
//            }

//            // Append a new item to the current parent's list of children.
//            TreeItem *parent = parents.last();
//            parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
//            for (int column = 0; column < columnData.size(); ++column)
//                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
//        }

//        ++number;
//    }
}












