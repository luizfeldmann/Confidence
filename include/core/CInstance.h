#ifndef _CINSTANCE_H_
#define _CINSTANCE_H_

#include "IProjTreeItem.h"
#include "CStoredNameItem.h"
#include "CStoredDescriptionItem.h"
#include "CNoChildren.h"
#include "core/IFindByName.h"

//! @brief Represents an instance of each entity from the configuration
class CInstance : public IProjTreeItem
    , public CStoredNameItem
    , public CStoredDescriptionItem
    , public CNoChildren
    , public IFindByName<CInstance>
{
protected:
    //! @copydoc IProjTreeItem::DocumentName
    bool DocumentName(IDocExporter& rExporter) const;

public:
    //! @brief Creates a new, empty instance
    CInstance();

    ~CInstance();

    //! @brief Returns a new instance of this class
    static IProjTreeItem* Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    //! @copydoc IExecutable::Execute
    bool Execute(CExecutionContext&) const override;
};

DECLARE_SERIALIZATION_SCHEME(CInstance)

#endif