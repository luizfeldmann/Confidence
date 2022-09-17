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

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;
};

DECLARE_SERIALIZATION_SCHEME(CConfigurationGroup)

#endif