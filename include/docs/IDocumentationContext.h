/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

/**
 * @file IDocumentationContext.h
 * @brief Defines interface #IDocumentationContext
 */

#ifndef _IDOCUMENTATION_CONTEXT_H_
#define _IDOCUMENTATION_CONTEXT_H_

#include <string>

//! @brief Stores structured documentation information
class IDocumentationContext
{
public:
    virtual ~IDocumentationContext() = default;

    //! @brief Stores a dependency relation
    virtual void Dependency(const std::string& strSrc, const std::string& strDst) = 0;
};

#endif