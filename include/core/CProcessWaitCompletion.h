#ifndef _CPROCESSWAITCOMPLETION_H_
#define _CPROCESSWAITCOMPLETION_H_

#include "IProcessPolicy.h"

//! @copydoc EProcessPolicy::EProcessWaitCompletion
class CProcessWaitCompletion : public IProcessPolicy
{
public:
    CProcessWaitCompletion();

    //! @copydoc IProcessPolicy::GetType
    EProcessPolicy GetType() const override;
};

DECLARE_SERIALIZATION_SCHEME(CProcessWaitCompletion);

#endif