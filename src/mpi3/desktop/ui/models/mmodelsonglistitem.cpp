#include "mmodelsonglistitem.h"


MModelSonglistItem::MModelSonglistItem()
{

}
MModelSonglistItem::~MModelSonglistItem()
{

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
