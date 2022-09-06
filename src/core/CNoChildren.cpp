#include "core/CNoChildren.h"

ITreeItemCollection::OptVecPtrItem CNoChildren::GetItems()
{
    return OptVecPtrItem();
}

ITreeItemCollection::OptCVecPtrItem CNoChildren::GetItems() const
{
    return OptCVecPtrItem();
}

ETreeItemType CNoChildren::GetSupportedChildTypes() const
{
    return EInvalid;
}