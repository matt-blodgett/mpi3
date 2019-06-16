#pragma once

#ifndef MMODELSONGLISTITEM_H
#define MMODELSONGLISTITEM_H


#include <QVariant>
#include <QList>

QT_BEGIN_NAMESPACE
class QAbstractTableModel;
QT_END_NAMESPACE


class MModelSonglistItem : public QObject
{
    Q_OBJECT

public:
    explicit MModelSonglistItem(QAbstractTableModel *parent);

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
