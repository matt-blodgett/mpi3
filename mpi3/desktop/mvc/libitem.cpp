#include "libitem.h"


LibraryItem::LibraryItem(LibraryItem *parent){
    m_parentItem = parent;
}

LibraryItem::~LibraryItem(){
    qDeleteAll(m_childItems);
}

LibraryItem *LibraryItem::parent(){
    return m_parentItem;
}
LibraryItem *LibraryItem::child(int row){
    return m_childItems.value(row);
}

int LibraryItem::childCount() const{
    return m_childItems.count();
}
int LibraryItem::childNumber() const{
    if(m_parentItem){
        return m_parentItem->m_childItems.indexOf(const_cast<LibraryItem*>(this));
    }

    return 0;
}
int LibraryItem::columnCount() const{
    return m_itemData.count();
}

QVariant LibraryItem::data(int column) const{
    return m_itemData.value(column);
}
bool LibraryItem::setData(int column, const QVariant &value){
    if (column < 0 || column >= m_itemData.size()){
        return false;
    }

    m_itemData[column] = value;
    return true;
}

QIcon LibraryItem::icon() const{
    return m_itemIcon;
}
void LibraryItem::setIcon(const QIcon &icn){
    m_itemIcon = icn;
}

bool LibraryItem::insertChildren(int position, int count, int columns){
    if (position < 0 || position > m_childItems.size()){
        return false;
    }

    for (int row = 0; row < count; ++row){
        LibraryItem *item = new LibraryItem(this);
        item->insertColumns(0, columns);
        m_childItems.insert(position, item);
    }

    return true;
}
bool LibraryItem::insertColumns(int position, int columns){
    if(position < 0 || position > m_itemData.size()){
        return false;
    }

    for(int column = 0; column < columns; ++column){
        m_itemData.insert(position, QVariant());
    }

    for(int i = 0; i < m_childItems.size(); i++){
        LibraryItem *child = m_childItems.at(i);
        child->insertColumns(position, columns);
    }

    return true;
}

bool LibraryItem::removeChildren(int position, int count){
    if (position < 0 || position + count > m_childItems.size()){
        return false;
    }

    for (int row = 0; row < count; ++row){
        delete m_childItems.takeAt(position);
    }

    return true;
}
bool LibraryItem::removeColumns(int position, int columns){
    if (position < 0 || position + columns > m_itemData.size()){
        return false;
    }

    for (int column = 0; column < columns; ++column){
        m_itemData.remove(position);
    }

    for(int i = 0; i < m_childItems.size(); i++){
        LibraryItem *child = m_childItems.at(i);
        child->removeColumns(position, columns);
    }

    return true;
}
