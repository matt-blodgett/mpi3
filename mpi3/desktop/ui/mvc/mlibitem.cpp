#include "mlibitem.h"


MModelItem::MModelItem(MModelItem *parent){
    m_parentItem = parent;
}

MModelItem::~MModelItem(){
    qDeleteAll(m_childItems);
}

MModelItem *MModelItem::parent(){
    return m_parentItem;
}
MModelItem *MModelItem::child(int row){
    return m_childItems.value(row);
}

int MModelItem::childCount() const{
    return m_childItems.count();
}
int MModelItem::childNumber() const{
    if(m_parentItem){
        return m_parentItem->m_childItems.indexOf(const_cast<MModelItem*>(this));
    }

    return 0;
}
int MModelItem::columnCount() const{
    return m_itemData.count();
}

QVariant MModelItem::data(int column) const{
    return m_itemData.value(column);
}
bool MModelItem::setData(int column, const QVariant &value){
    if (column < 0 || column >= m_itemData.size()){
        return false;
    }

    m_itemData[column] = value;
    return true;
}

QIcon MModelItem::icon() const{
    return m_itemIcon;
}
void MModelItem::setIcon(const QIcon &icn){
    m_itemIcon = icn;
}

bool MModelItem::insertChildren(int position, int count, int columns){
    if (position < 0 || position > m_childItems.size()){
        return false;
    }

    for (int row = 0; row < count; ++row){
        MModelItem *item = new MModelItem(this);
        item->insertColumns(0, columns);
        m_childItems.insert(position, item);
    }

    return true;
}
bool MModelItem::insertColumns(int position, int columns){
    if(position < 0 || position > m_itemData.size()){
        return false;
    }

    for(int column = 0; column < columns; ++column){
        m_itemData.insert(position, QVariant());
    }

    for(int i = 0; i < m_childItems.size(); i++){
        MModelItem *child = m_childItems.at(i);
        child->insertColumns(position, columns);
    }

    return true;
}

bool MModelItem::removeChildren(int position, int count){
    if (position < 0 || position + count > m_childItems.size()){
        return false;
    }

    for (int row = 0; row < count; ++row){
        delete m_childItems.takeAt(position);
    }

    return true;
}
bool MModelItem::removeColumns(int position, int columns){
    if (position < 0 || position + columns > m_itemData.size()){
        return false;
    }

    for (int column = 0; column < columns; ++column){
        m_itemData.removeAt(position);
    }

    for(int i = 0; i < m_childItems.size(); i++){
        MModelItem *child = m_childItems.at(i);
        child->removeColumns(position, columns);
    }

    return true;
}
