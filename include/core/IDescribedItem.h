/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

/**
 * @file IDescribedItem.h
 * @brief Defines interface #IDescribedItem
 */

#ifndef _IDESCRIBEDITEM_H_
#define _IDESCRIBEDITEM_H_

#include <string>

//! @brief Interface for tree items with a description
class IDescribedItem
{
public:
    IDescribedItem() = default;
    virtual ~IDescribedItem() = default;

    //! @brief Reads the description of this item
    virtual std::string GetDescription() const = 0;

    //! @brief Sets the description of this item
    //! @param[in] strNewDesc Assigned description
    //! @return True if new description was accepted
    virtual bool SetDescription(const std::string& strNewDesc) = 0;
};

#endif