#include "mpi3/desktop/ui/models/mmodelcontainersitem.h"


MModelContainersItem::MModelContainersItem(MModelContainersItem *parent)
{
    m_parentItem = parent;

    if(!m_parentItem) {
        m_itemType = MModelContainersItem::RootItem;
    }
}

MModelContainersItem::~MModelContainersItem()
{
    qDeleteAll(m_childItems);
}

MModelContainersItem *MModelContainersItem::parent()
{
    return m_parentItem;
}
MModelContainersItem *MModelContainersItem::child(int row)
{
    return m_childItems.value(row);
}

MModelContainersItem::ItemType MModelContainersItem::type() const
{
    return m_itemType;
}
void MModelContainersItem::setType(MModelContainersItem::ItemType value)
{
    m_itemType = value;
}

QString MModelContainersItem::pid() const
{
    return m_itemPid;
}
void MModelContainersItem::setPid(const QString &value)
{
    m_itemPid = value;
}

QString MModelContainersItem::label() const
{
    return m_itemLabel;
}
void MModelContainersItem::setLabel(const QString &value)
{
    m_itemLabel = value;
}

int MModelContainersItem::childCount() const
{
    return m_childItems.count();
}
int MModelContainersItem::childNumber() const
{
    if(m_parentItem) {
        return m_parentItem->m_childItems.indexOf(const_cast<MModelContainersItem*>(this));
    }

    return 0;
}

bool MModelContainersItem::insertChildren(int position, int count)
{
    if (position < 0 || position > m_childItems.size()) {
        return false;
    }

    for (int row = 0; row < count; ++row) {
        MModelContainersItem *item = new MModelContainersItem(this);
        m_childItems.insert(position, item);
    }

    return true;
}
bool MModelContainersItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > m_childItems.size()) {
        return false;
    }

    for (int row = 0; row < count; ++row) {
        delete m_childItems.takeAt(position);
    }

    return true;
}

bool MModelContainersItem::move(MModelContainersItem *parent, int position)
{
    if(!parent) {
        return false;
    }

    if (parent == m_parentItem) {
        return false;
    }

    if(position < 0 || position > parent->childCount()) {
        return false;
    }

    if(m_parentItem) {
        m_parentItem->m_childItems.removeAt(this->childNumber());
    }

    m_parentItem = parent;
    m_parentItem->m_childItems.insert(position, this);

    return true;
}
