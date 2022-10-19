#ifndef _CPROCESSFIREFORGET_H_
#define _CPROCESSFIREFORGET_H_

#include "IProcessPolicy.h"

//! @copydoc EProcessPolicy::EProcessFireForget
class CProcessFireAndForget : public IProcessPolicy
{
public:
    CProcessFireAndForget();

    //! @copydoc IProcessPolicy::GetType
    EProcessPolicy GetType() const override;

    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override;
};

DECLARE_SERIALIZATION_SCHEME(CProcessFireAndForget);

#endif