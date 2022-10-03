#include "ui/Models/CBaseTreeItemModel.h"

/* static */ IProjTreeItem* CBaseTreeItemModel::GetItem(const wxDataViewItem& item)
{
    return static_cast<IProjTreeItem*>(item.GetID());
}

wxDataViewItem CBaseTreeItemModel::GetParent(const wxDataViewItem& item) const
{
    return m_mapParents.at(item);
}

unsigned int CBaseTreeItemModel::GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const
{
    unsigned int uiCount = 0;
    array.clear();

    if (NULL == parent.GetID())
    {
        wxDataViewItem item((void*)&GetRootItem());
        array.push_back(item);
        uiCount = 1;

        m_mapParents[item] = parent;
    }
    else
    {
        const IProjTreeItem* pParent = GetItem(parent);
        assert(nullptr != pParent);

        ITreeItemCollection::vec_cref_t vSubitems = pParent->SubItems();
        for (const ITreeItemCollection::cref_t& rChild : vSubitems)
        {
            const IProjTreeItem* pChild = &rChild.get();
            wxDataViewItem child((void*)pChild);
            array.push_back(child);
            uiCount++;

            m_mapParents[child] = parent;
        }
    }

    return uiCount;
}