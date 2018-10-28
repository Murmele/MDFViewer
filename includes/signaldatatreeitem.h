#ifndef SIGNALDATATREEITEM_H
#define SIGNALDATATREEITEM_H

#include <QList>

class SignalDataTreeItem
{
public:
    SignalDataTreeItem(const QList<QVariant> &data, SignalDataTreeItem *parentItem = nullptr);
    SignalDataTreeItem(SignalDataTreeItem *parentItem = nullptr);
    ~SignalDataTreeItem();

    void appendChild(SignalDataTreeItem *child);

    SignalDataTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    SignalDataTreeItem *parentItem();
    QList<QVariant>* getData(){return &m_itemData;}
    bool removeChildren(int position, int count);

private:
    QList<SignalDataTreeItem*> m_childItems;
    QList<QVariant> m_itemData; // muss eventuell geändert werden
    SignalDataTreeItem *m_parentItem;
};

#endif // SIGNALDATATREEITEM_H
