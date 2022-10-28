#ifndef _CCONFIGURATION_H_
#define _CCONFIGURATION_H_

#include "core/IProjTreeItem.h"
#include "core/CStoredNameItem.h"
#include "core/CStoredDescriptionItem.h"
#include "core/CStoredItemCollection.h"

//! @brief A named set of settings that are chosen together
class CConfiguration : public IProjTreeItem
    , public CStoredNameItem
    , public CStoredDescriptionItem
    , public CStoredItemCollection
{
protected:
    //! @brief Creates a new, empty configuration
    CConfiguration();
    FACTORY_FRIEND();

    //! @copydoc IProjTreeItem::DocumentName
    bool DocumentName(IDocExporter& rExporter) const override;

public:
    ~CConfiguration();

    //! @brief Creates a new configuration with the provided name and description
    //! @param[in] strName Name of new configuration
    //! @param[in] strDesc Description of new configuration
    CConfiguration(const std::string& strName, const std::string& strDesc);

    //! @brief Default move-constructor
    CConfiguration(CConfiguration&&) = default;

	//! @brief Default move-assignment operator
    CConfiguration& operator=(CConfiguration&&) = default;

    //! @brief Returns a new instance of this class
    static IProjTreeItem::ptr_t Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    //! @copydoc IExecutable::Execute
    bool Execute(CExecutionContext&) const override;
};

DECLARE_SERIALIZATION_SCHEME(CConfiguration)

#endif