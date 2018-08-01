#ifndef LIBITEM_H
#define LIBITEM_H

#include <QVariant>
#include <QVector>
#include <QList>
#include <QIcon>


class LibraryItem
{

public:
    explicit LibraryItem(LibraryItem *parent = nullptr);
    ~LibraryItem();

public:
    LibraryItem *parent();
    LibraryItem *child(int row);

    int childCount() const;
    int childNumber() const;
    int columnCount() const;

    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);

    QIcon icon() const;
    void setIcon(const QIcon &icn);

    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);

    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);

private:
    QList<LibraryItem*> m_childItems;
    QVector<QVariant> m_itemData;
    LibraryItem *m_parentItem;
    QIcon m_itemIcon;

};

#endif
