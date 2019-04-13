#pragma once

#ifndef MMODELSONGLISTITEM_H
#define MMODELSONGLISTITEM_H


#include <QVariant>
#include <QList>


class MModelSonglistItem
{

public:
    explicit MModelSonglistItem();
    ~MModelSonglistItem();

public:
    QString pid() const;
    void setPID(const QString &value);

    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);

private:
    QList<QVariant> m_itemData;
    QString m_itemPID;

};


#endif
