#include "mmodelsonglistitem.h"


#define MPI3_NUMBER_OF_FIELDS 16

MModelSonglistItem::MModelSonglistItem()
{
    for(int i = 0; i < MPI3_NUMBER_OF_FIELDS; i++){
        m_itemData.append("");
    }
}
MModelSonglistItem::~MModelSonglistItem()
{

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
    return m_itemData.at(column);
}
bool MModelSonglistItem::setData(int column, const QVariant &value)
{
    if(column >= 0 && column < m_itemData.size()){
        m_itemData[column] = value;
        return true;
    }

    return false;
}
