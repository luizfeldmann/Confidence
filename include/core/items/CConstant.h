#ifndef _CCONSTANT_H_
#define _CCONSTANT_H_

#include "core/IProjTreeItem.h"
#include "core/CStoredNameItem.h"
#include "core/CStoredDescriptionItem.h"
#include "core/CStoredExpression.h"
#include "core/CNoChildren.h"
#include "core/CAssignable.h"

//! @brief Represents an assignable value that is not directly dependant on configuration or instance
class CConstant : public IProjTreeItem
    , public CStoredNameItem
    , public CStoredDescriptionItem
    , public CStoredExpression
    , public CNoChildren
    , public CAssignable
{
protected:
    //! @brief Creates a new, empty constant
    CConstant();
    FACTORY_FRIEND();

    //! @copydoc IProjTreeItem::DocumentName
    bool DocumentName(IDocExporter& rExporter) const;

    //! @copydoc IProjTreeItem::DocumentCustom
    bool DocumentCustom(IDocExporter& rExporter) const override;

public:

    ~CConstant();

    //! @brief Returns a new instance of this class
    static IProjTreeItem::ptr_t Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    //! @copydoc IExecutable::Execute
    bool Execute(CExecutionContext& rContext) const override;
};

DECLARE_SERIALIZATION_SCHEME(CConstant)

#endif