#pragma once

#ifndef MMODELCONTAINERSITEM_H
#define MMODELCONTAINERSITEM_H


#include <QList>
#include <QIcon>


class MModelContainersItem
{

public:
    enum ItemType {
        EmptyItem,
        RootItem,
        FolderItem,
        PlaylistItem
    };

public:
    explicit MModelContainersItem(MModelContainersItem *parent = nullptr);
    ~MModelContainersItem();

public:
    MModelContainersItem *parent();
    MModelContainersItem *child(int row);

    MModelContainersItem::ItemType type() const;
    void setType(MModelContainersItem::ItemType value);

    QString pid() const;
    void setPID(const QString &value);

    QString label() const;
    void setLabel(const QString &value);

    QIcon icon() const;
    void setIcon(const QIcon &value);

    int childCount() const;
    int childNumber() const;

    bool insertChildren(int position, int count);
    bool removeChildren(int position, int count);

    bool move(MModelContainersItem *parent, int position);

private:
    MModelContainersItem *m_parentItem = nullptr;
    QList<MModelContainersItem*> m_childItems;

    MModelContainersItem::ItemType m_itemType = MModelContainersItem::ItemType::EmptyItem;
    QString m_itemPID;
    QString m_itemLabel;
    QIcon m_itemIcon;
};


#endif
