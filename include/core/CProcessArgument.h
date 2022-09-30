#ifndef _CPROCESSARGUMENT_H_
#define _CPROCESSARGUMENT_H_

#include "CStoredDescriptionItem.h"
#include "CStoredExpression.h"

//! @brief Represents an argument passed to the process when starting
class CProcessArgument : public CStoredDescriptionItem, public CStoredExpression
{
private:
    CProcessArgument(const CProcessArgument&) = delete;
    CProcessArgument& operator=(const CProcessArgument&) = delete;

public:
    CProcessArgument();
    ~CProcessArgument();

    CProcessArgument(CProcessArgument&&) = default;
    CProcessArgument& operator=(CProcessArgument&&) = default;
};

DECLARE_SERIALIZATION_SCHEME(CProcessArgument)

#endif