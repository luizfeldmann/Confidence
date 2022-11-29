#ifndef _CPROCESSFIREFORGET_H_
#define _CPROCESSFIREFORGET_H_

#include "IProcessPolicy.h"

//! @copydoc EProcessPolicy::FireForget
class CProcessFireAndForget : public IProcessPolicy
{
public:
    CProcessFireAndForget();

    //! @copydoc IProcessPolicy::GetType
    EProcessPolicy GetType() const override;

    //! @copydoc IDocumentable::Document
    bool Document(IDocExporter& rExporter) const override;

    //! @copydoc IProcessPolicy::Execute
    bool Execute(CExecutionContext& rContext, const SProcessStartInfo& rInfo) const override;
};

DECLARE_SERIALIZATION_SCHEME(CProcessFireAndForget);

#endif