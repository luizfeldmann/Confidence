#ifndef _CINSTANCEGROUP_H_
#define _CINSTANCEGROUP_H_

#include "core/IProjTreeItem.h"
#include "core/CStoredNameItem.h"
#include "core/CStoredDescriptionItem.h"
#include "core/CStoredItemCollection.h"

//! @brief Special group containing all of the project's instances
class CInstanceGroup : public IProjTreeItem
    , public CStoredNameItem
    , public CStoredDescriptionItem
    , public CStoredItemCollection
{
protected:
    //! @brief Creates a new, empty instances-group
    CInstanceGroup();
    FACTORY_FRIEND();

public:
    ~CInstanceGroup();

    //! @brief Instantiates a new item
    static std::shared_ptr<CInstanceGroup> Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    //! @copydoc IExecutable::Execute
    bool Execute(CExecutionContext& rContext) const override;
};

DECLARE_SERIALIZATION_SCHEME(CInstanceGroup)

#endif