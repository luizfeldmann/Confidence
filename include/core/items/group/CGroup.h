#ifndef _CGROUP_H_
#define _CGROUP_H_

#include "core/IProjTreeItem.h"
#include "core/CStoredNameItem.h"
#include "core/CStoredDescriptionItem.h"
#include "core/CStoredItemCollection.h"

class IGroupFilter;

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

    //! Stores the filter who decides if this group will execute
    std::shared_ptr<IGroupFilter> m_pFilter;

    SERIALIZATION_FRIEND(CGroup);

    //! @copydoc IProjTreeItem::DocumentName
    bool DocumentName(IDocExporter& rExporter) const override;

    //! @copydoc IProjTreeItem::DocumentCustom
    bool DocumentCustom(IDocExporter& rExporter) const override;

public:
    ~CGroup();

    //! @brief Returns a new instance of this class
    static IProjTreeItem::ptr_t Create();

    //! @brief Gets a pointer to the current filter
    std::shared_ptr<IGroupFilter> GetFilter();

    //! @brief Gets a pointer to the current filter
    std::shared_ptr<const IGroupFilter> GetFilter() const;

    //! @brief Changes the current filter used for this group
    void SetFilter(std::shared_ptr<IGroupFilter> pNewFilter);

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    //! @copydoc IExecutable::Execute
    bool Execute(CExecutionContext&) const override;

    //! @copydoc ISerializationNotify::PostDeserialize
    bool PostDeserialize(CProject& rProject) override;

    //! @copydoc ISerializationNotify::PreSerialize
    bool PreSerialize() override;
};

DECLARE_SERIALIZATION_SCHEME(CGroup)

#endif