/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CCONFIGURATIONGROUP_H_
#define _CCONFIGURATIONGROUP_H_

#include "core/items/CConfiguration.h"

//! @brief The "root" configuration from which all other configurations descend
class CConfigurationGroup : public CConfiguration
{
protected:
    //! @brief Creates a new, empty configuration group
    CConfigurationGroup();
    FACTORY_FRIEND();

public:
    ~CConfigurationGroup();

    //! @brief Instantiates a new item
    static std::shared_ptr<CConfigurationGroup> Create();

    //! @copydoc IProjTreeItem::GetType
    ETreeItemType GetType() const override;

    //! @copydoc IExecutable::Execute
    bool Execute(CExecutionContext& rContext) const override;
};

DECLARE_SERIALIZATION_SCHEME(CConfigurationGroup)

#endif