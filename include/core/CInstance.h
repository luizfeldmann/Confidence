#ifndef _CINSTANCE_H_
#define _CINSTANCE_H_

#include "IProjTreeItem.h"
#include "CStoredNameItem.h"
#include "CStoredDescriptionItem.h"
#include "CNoChildren.h"

//! @brief Represents an instance of each entity from the configuration
class CInstance : public IProjTreeItem
    , public CStoredNameItem
    , public CStoredDescriptionItem
    , public CNoChildren
{
public:
    //! @brief Creates a new, empty instance
    CInstance();

    ~CInstance();

    //! @brief Returns a new instance of this class
    static IProjTreeItem* Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;
};

DECLARE_SERIALIZATION_SCHEME(CInstance)

#endif