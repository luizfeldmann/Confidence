/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CPROCESSARGUMENT_H_
#define _CPROCESSARGUMENT_H_

#include "core/CStoredDescriptionItem.h"
#include "core/CStoredExpression.h"
#include "docs/IDocumentable.h"

//! @brief Represents an argument passed to the process when starting
class CProcessArgument : public CStoredDescriptionItem, public CStoredExpression, public IDocumentable
{
private:
    CProcessArgument(const CProcessArgument&) = delete;
    CProcessArgument& operator=(const CProcessArgument&) = delete;

public:
    //! @brief Constructs an empty argument
    CProcessArgument();

    ~CProcessArgument();

    //! @brief Default move-constructor
    CProcessArgument(CProcessArgument&&) = default;

    //! @brief Default move-assignment operator
    CProcessArgument& operator=(CProcessArgument&&) = default;

    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override;
};

DECLARE_SERIALIZATION_SCHEME(CProcessArgument)

#endif