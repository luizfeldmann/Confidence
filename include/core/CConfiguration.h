#ifndef _CCONFIGURATION_H_
#define _CCONFIGURATION_H_

#include "IProjTreeItem.h"
#include "CStoredNameItem.h"
#include "CStoredDescriptionItem.h"
#include "CStoredItemCollection.h"

//! @brief A named set of settings that are chosen together
class CConfiguration : public IProjTreeItem
    , public CStoredNameItem
    , public CStoredDescriptionItem
    , public CStoredItemCollection
{
public:
    //! @brief Creates a new, empty configuration
    CConfiguration();
    ~CConfiguration();

    //! @brief Returns a new instance of this class
    static IProjTreeItem* Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;
};

DECLARE_SERIALIZATION_SCHEME(CConfiguration)

#endif