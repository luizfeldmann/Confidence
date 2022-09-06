#include "core/CStoredItemCollection.h"
#include "core/IProjTreeItem.h"

DEFINE_SERIALIZATION_SCHEME(CStoredItemCollection,
    SERIALIZATION_MEMBER(m_vItems)
)

CStoredItemCollection::CStoredItemCollection(ETreeItemType eSupportedSubitems)
    : m_eSupportedItemTypes(eSupportedSubitems)
{
}

ITreeItemCollection::OptVecPtrItem CStoredItemCollection::GetItems()
{
    return OptVecPtrItem(std::ref(m_vItems));
}

ITreeItemCollection::OptCVecPtrItem CStoredItemCollection::GetItems() const
{
    return OptCVecPtrItem(std::cref(m_vItems));
}

ETreeItemType CStoredItemCollection::GetSupportedChildTypes() const
{
    return m_eSupportedItemTypes;
}