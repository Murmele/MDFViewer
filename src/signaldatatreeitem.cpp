#include "signaldatatreeitem.h"

SignalDataTreeItem::SignalDataTreeItem(const QList<QVariant> &data, SignalDataTreeItem *parent)
{
    m_parentItem = parent;
    m_itemData = data;
}

SignalDataTreeItem::SignalDataTreeItem(SignalDataTreeItem *parent)
{
    m_parentItem = parent;
}

SignalDataTreeItem::~SignalDataTreeItem()
{
    qDeleteAll(m_childItems);
}

void SignalDataTreeItem::appendChild(SignalDataTreeItem *item)
{
    m_childItems.append(item);
}

SignalDataTreeItem *SignalDataTreeItem::child(int row)
{
    return m_childItems.value(row);
}

int SignalDataTreeItem::childCount() const
{
    return m_childItems.count();
}

int SignalDataTreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<SignalDataTreeItem*>(this));

    return 0;
}

bool SignalDataTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete m_childItems.takeAt(position);

    return true;
}

int SignalDataTreeItem::columnCount() const
{
    return m_itemData.count();
}

QVariant SignalDataTreeItem::data(int column) const
{
    return m_itemData.value(column);
}

SignalDataTreeItem *SignalDataTreeItem::parentItem()
{
    return m_parentItem;
}


