#pragma once

#ifndef MLIBITEM_H
#define MLIBITEM_H

#include <QVariant>
#include <QList>
#include <QIcon>


class MModelItem
{

public:
    explicit MModelItem(MModelItem *parent = nullptr);
    ~MModelItem();

public:
    MModelItem *parent();
    MModelItem *child(int row);

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
    MModelItem *m_parentItem;
    QList<MModelItem*> m_childItems;
    QList<QVariant> m_itemData;
    QIcon m_itemIcon;
};

#endif
