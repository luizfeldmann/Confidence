#include "core/CNoChildren.h"
#include "core/IProjTreeItem.h"

CNoChildren::vec_ptr_t CNoChildren::SubItems()
{
    vec_ptr_t vEmpty;
    return vEmpty;
}

CNoChildren::vec_cptr_t CNoChildren::SubItems() const
{
    vec_cptr_t vEmpty;
    return vEmpty;
}

CNoChildren::ptr_t CNoChildren::TakeItem(const IProjTreeItem& rItem)
{
    ptr_t pEmpty;
    return pEmpty;
}

bool CNoChildren::AddItem(ptr_t pNewItem)
{
    return false;
}

bool CNoChildren::SwapItems(const IProjTreeItem& rA, const IProjTreeItem& rB)
{
    return false;
}

ETreeItemType CNoChildren::GetSupportedChildTypes() const
{
    return EInvalid;
}