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
protected:
    //! @brief Creates a new, empty group
    CGroup();
    FACTORY_FRIEND();

    //! @copydoc IProjTreeItem::DocumentName
    bool DocumentName(IDocExporter& rExporter) const override;

public:
    ~CGroup();

    //! @brief Returns a new instance of this class
    static IProjTreeItem::ptr_t Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    //! @copydoc IExecutable::Execute
    bool Execute(CExecutionContext&) const override;
};

DECLARE_SERIALIZATION_SCHEME(CGroup)

#endif