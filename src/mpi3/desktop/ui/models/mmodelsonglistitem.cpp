#include "mpi3/desktop/ui/models/mmodelsonglistitem.h"

#include <QAbstractTableModel>


MModelSonglistItem::MModelSonglistItem(QAbstractTableModel *parent) : QObject(parent)
{
    for(int i = 0; i < parent->columnCount() - 1; i++) {
        m_itemData.append(QVariant());
    }
}

QString MModelSonglistItem::pid() const
{
    return m_itemPID;
}
void MModelSonglistItem::setPID(const QString &value)
{
    m_itemPID = value;
}

QVariant MModelSonglistItem::data(int column) const
{
    if(column >= 0 && column < m_itemData.size()) {
        return m_itemData.at(column);
    }

    return QVariant();
}
bool MModelSonglistItem::setData(int column, const QVariant &value)
{
    if(column >= 0 && column < m_itemData.size()){
        m_itemData[column] = value;
        return true;
    }

    return false;
}
