#ifndef _CPROCESSARGUMENT_H_
#define _CPROCESSARGUMENT_H_

#include "CStoredDescriptionItem.h"

//! @brief Represents an argument passed to the process when starting
class CProcessArgument : public CStoredDescriptionItem
{
public:
    CProcessArgument();
    ~CProcessArgument();
};

DECLARE_SERIALIZATION_SCHEME(CProcessArgument)

#endif