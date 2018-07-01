
#include "mlibraryitem.h"

#include <QStringList>

LibraryItem::LibraryItem(const QVector<QVariant> &data, LibraryItem *parent)
{
    parentItem = parent;
    itemData = data;
}

LibraryItem::~LibraryItem()
{
    qDeleteAll(childItems);
}

LibraryItem *LibraryItem::child(int number)
{
    return childItems.value(number);
}

int LibraryItem::childCount() const
{
    return childItems.count();
}

int LibraryItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<LibraryItem*>(this));

    return 0;
}

int LibraryItem::columnCount() const
{
    return itemData.count();
}

QVariant LibraryItem::data(int column) const
{
    return itemData.value(column);
}

bool LibraryItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        LibraryItem *item = new LibraryItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool LibraryItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    foreach (LibraryItem *child, childItems)
        child->insertColumns(position, columns);

    return true;
}

LibraryItem *LibraryItem::parent()
{
    return parentItem;
}

bool LibraryItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}


bool LibraryItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    foreach (LibraryItem *child, childItems)
        child->removeColumns(position, columns);

    return true;
}

bool LibraryItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}
