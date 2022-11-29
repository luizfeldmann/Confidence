#ifndef _CINSTANCE_H_
#define _CINSTANCE_H_

#include "core/IProjTreeItem.h"
#include "core/CStoredNameItem.h"
#include "core/CStoredDescriptionItem.h"
#include "core/CNoChildren.h"

//! @brief Represents an instance of each entity from the configuration
class CInstance : public IProjTreeItem
    , public CStoredNameItem
    , public CStoredDescriptionItem
    , public CNoChildren
{
protected:
    //! @brief Creates a new, empty instance
    CInstance();
    FACTORY_FRIEND();

    //! @copydoc IProjTreeItem::DocumentName
    bool DocumentName(IDocExporter& rExporter) const;

public:

    ~CInstance();

    //! @brief Returns a new instance of this class
    static IProjTreeItem::ptr_t Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    //! @copydoc IExecutable::Execute
    bool Execute(CExecutionContext& rContext) const override;
};

DECLARE_SERIALIZATION_SCHEME(CInstance)

#endif