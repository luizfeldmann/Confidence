#ifndef _CINSTANCEGROUP_H_
#define _CINSTANCEGROUP_H_

#include "IProjTreeItem.h"
#include "CStoredNameItem.h"
#include "CStoredDescriptionItem.h"
#include "CStoredItemCollection.h"

//! @brief Special group containing all of the project's instances
class CInstanceGroup : public IProjTreeItem
    , public CStoredNameItem
    , public CStoredDescriptionItem
    , public CStoredItemCollection
{
public:
    //! @brief Creates a new, empty group
    CInstanceGroup();
    ~CInstanceGroup();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;
};

DECLARE_SERIALIZATION_SCHEME(CInstanceGroup)

#endif