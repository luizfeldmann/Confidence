/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

/**
 * @file IFileDataLocator.h
 * @brief Defines interface #IFileDataLocator
 */

#ifndef _IFILEDATALOCATOR_H_
#define _IFILEDATALOCATOR_H_

#include "docs/IDocumentable.h"

//! @brief Generic representation of a location in a known file type structure where data can be accessed
class IFileDataLocator : public IDocumentable
{
public:
    virtual ~IFileDataLocator() = default;

    //! @brief Parses the string representation of the location into this structured format
    //! @param[in] strLocator Textual representation of the location
    //! @return True if success
    virtual bool from_string(const std::string& strLocator) = 0;

    //! @brief Obtains a string representation of the relevant location
    virtual std::string to_string() const = 0;
};

#endif