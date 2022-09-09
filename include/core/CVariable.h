#ifndef _CVARIABLE_H_
#define _CVARIABLE_H_

#include "IProjTreeItem.h"
#include "CStoredNameItem.h"
#include "CStoredDescriptionItem.h"
#include "CNoChildren.h"

//! @brief Stores a constant or an expression
class CVariable : public IProjTreeItem
                , public CStoredNameItem
                , public CStoredDescriptionItem
                , public CNoChildren
{
public:
    //! @brief Creates a new, empty variable
    CVariable();

    ~CVariable();

    //! @brief Returns a new instance of this class
    static IProjTreeItem* Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;
};

DECLARE_SERIALIZATION_SCHEME(CVariable)

#endif