/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

/**
 * @file IDocumentable.h
 * @brief Defines interface #IDocumentable
 */

#ifndef _IDOCUMENTABLE_H_
#define _IDOCUMENTABLE_H_

class IDocExporter;

//! @brief Interface for classes who can self-document using an exporter
class IDocumentable
{
public:
    virtual ~IDocumentable() = default;

    //! @brief Generates documentation for this class using the provided exporter
    //! @return True if success
    virtual bool Document(IDocExporter& rExporter) const = 0;
};

#endif