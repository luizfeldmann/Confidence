#ifndef _CGROUP_H_
#define _CGROUP_H_

#include "IProjTreeItem.h"
#include "CStoredNameItem.h"
#include "CStoredDescriptionItem.h"
#include "CStoredItemCollection.h"

//! @brief Organizes other tree items
class CGroup : public IProjTreeItem
             , public CStoredNameItem
             , public CStoredDescriptionItem
             , public CStoredItemCollection
{
public:
    //! @brief Creates a new, empty group
    CGroup();
    ~CGroup();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;
};

DECLARE_SERIALIZATION_SCHEME(CGroup)

#endif