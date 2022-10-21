#ifndef _CCONFIGURATIONGROUP_H_
#define _CCONFIGURATIONGROUP_H_

#include "core/CConfiguration.h"

//! @brief The "root" configuration from which all other configurations descend
class CConfigurationGroup : public CConfiguration
{
public:
    //! @brief Creates a new, empty configuration group
    CConfigurationGroup();
    ~CConfigurationGroup();

    //! @brief Default move-constructor
    CConfigurationGroup(CConfigurationGroup&&) = default;

    //! @brief Default move-assignment operator
    CConfigurationGroup& operator=(CConfigurationGroup&&) = default;

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    //! @copydoc IExecutable::Execute
    bool Execute(CExecutionContext&) const override;
};

DECLARE_SERIALIZATION_SCHEME(CConfigurationGroup)

#endif