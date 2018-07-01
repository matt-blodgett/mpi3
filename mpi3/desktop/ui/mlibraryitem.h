#ifndef LIBRARYITEM_H
#define LIBRARYITEM_H

#include <QVariant>
#include <QVector>
#include <QList>


class LibraryItem
{

public:
    explicit LibraryItem(const QVector<QVariant> &data, LibraryItem *parent = 0);
    ~LibraryItem();

    LibraryItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    LibraryItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

private:
    QList<LibraryItem*> childItems;
    QVector<QVariant> itemData;
    LibraryItem *parentItem;
};

#endif // LIBRARYITEM_H
