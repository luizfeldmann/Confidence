#ifndef _CENVIRONMENT_IMPORT_H_
#define _CENVIRONMENT_IMPORT_H_

#include "core/IProjTreeItem.h"
#include "core/CStoredNameItem.h"
#include "core/CStoredDescriptionItem.h"
#include "core/CNoChildren.h"

//! @brief Imports an environment variable
class CEnvironmentImport : public IProjTreeItem
    , public CStoredNameItem
    , public CStoredDescriptionItem
    , public CNoChildren
{
protected:
    //! @brief Creates a new, empty environment variable importer
    CEnvironmentImport();
    FACTORY_FRIEND();

    //! @copydoc IProjTreeItem::DocumentName
    bool DocumentName(IDocExporter& rExporter) const override;

public:
    ~CEnvironmentImport();

    //! @brief Returns a new instance of this class
    static IProjTreeItem::ptr_t Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    //! @copydoc IExecutable::Execute
    bool Execute(CExecutionContext& rContext) const override;
};

DECLARE_SERIALIZATION_SCHEME(CEnvironmentImport)

#endif