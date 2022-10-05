#include "ui/Models/CBaseTreeItemModel.h"

/* static */ IProjTreeItem* CBaseTreeItemModel::GetPointer(const wxDataViewItem& item)
{
    return static_cast<IProjTreeItem*>(item.GetID());
}

/* static */ wxDataViewItem CBaseTreeItemModel::GetViewItem(const IProjTreeItem* pItem)
{
    return wxDataViewItem((void*)pItem);
}

wxDataViewItem CBaseTreeItemModel::GetParent(const wxDataViewItem& item) const
{
    parent_map_t::const_iterator itFind = m_mapParents.find(item);
    
    return (m_mapParents.cend() == itFind)
        ? GetViewItem(nullptr)
        : itFind->second;
}

unsigned int CBaseTreeItemModel::GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const
{
    unsigned int uiCount = 0;
    array.clear();

    if (NULL == parent.GetID())
    {
        wxDataViewItem item = GetViewItem(&GetRootItem());
        array.push_back(item);
        uiCount = 1;

        m_mapParents[item] = parent;
    }
    else
    {
        const IProjTreeItem* pParent = GetPointer(parent);
        assert(nullptr != pParent);

        ITreeItemCollection::vec_cref_t vSubitems = pParent->SubItems();
        for (const ITreeItemCollection::cref_t& rChild : vSubitems)
        {
            const IProjTreeItem* pChild = &rChild.get();
            wxDataViewItem cChild = GetViewItem(pChild);
            array.push_back(cChild);
            uiCount++;

            m_mapParents[cChild] = parent;
        }
    }

    return uiCount;
}