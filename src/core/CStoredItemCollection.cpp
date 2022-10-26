#include "core/CStoredItemCollection.h"
#include "core/IProjTreeItem.h"
#include <algorithm>

DEFINE_SERIALIZATION_SCHEME(CStoredItemCollection,
    SERIALIZATION_MEMBER(m_vItems)
)

CStoredItemCollection::CStoredItemCollection(ETreeItemType eSupportedSubitems)
    : m_eSupportedItemTypes(eSupportedSubitems)
{
}

CStoredItemCollection::vstore_t::iterator CStoredItemCollection::find(const IProjTreeItem& rItem)
{
    CStoredItemCollection::vstore_t::iterator it = std::find_if(m_vItems.begin(), m_vItems.end(), 
        [&rItem](const ptr_t& pItem)->bool {
            return (&rItem == pItem.get());
        }
    );

    return it;
}

CStoredItemCollection::vec_ptr_t CStoredItemCollection::SubItems()
{
    return m_vItems;
}

CStoredItemCollection::vec_cptr_t CStoredItemCollection::SubItems() const
{
    CStoredItemCollection::vec_cptr_t vGetItems(m_vItems.cbegin(), m_vItems.cend());

    return vGetItems;
}

CStoredItemCollection::ptr_t CStoredItemCollection::TakeItem(const IProjTreeItem& rItem)
{
    ptr_t pTakeItem;

    vstore_t::iterator itFind = CStoredItemCollection::find(rItem);
    if (m_vItems.cend() != itFind)
    {
        pTakeItem = std::move( *itFind );
        m_vItems.erase(itFind);
    }

    return pTakeItem;
}

bool CStoredItemCollection::AddItem(ptr_t pNewItem)
{
    m_vItems.emplace_back(pNewItem);
    return true;
}

bool CStoredItemCollection::SwapItems(const IProjTreeItem& rA, const IProjTreeItem& rB)
{
    bool bSuccess = false;

    vstore_t::iterator itA = CStoredItemCollection::find(rA);
    vstore_t::iterator itB = CStoredItemCollection::find(rB);

    if ((m_vItems.end() != itA) && (m_vItems.end() != itB))
    {
        std::iter_swap(itA, itB);
        bSuccess = true;
    }

    return bSuccess;
}

ETreeItemType CStoredItemCollection::GetSupportedChildTypes() const
{
    return m_eSupportedItemTypes;
}