#ifndef _CPROCESSARGUMENT_H_
#define _CPROCESSARGUMENT_H_

#include "CStoredDescriptionItem.h"
#include "CStoredExpression.h"
#include "docs/IDocumentable.h"

//! @brief Represents an argument passed to the process when starting
class CProcessArgument : public CStoredDescriptionItem, public CStoredExpression, public IDocumentable
{
private:
    CProcessArgument(const CProcessArgument&) = delete;
    CProcessArgument& operator=(const CProcessArgument&) = delete;

public:
    CProcessArgument();
    ~CProcessArgument();

    CProcessArgument(CProcessArgument&&) = default;
    CProcessArgument& operator=(CProcessArgument&&) = default;

    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override;
};

DECLARE_SERIALIZATION_SCHEME(CProcessArgument)

#endif