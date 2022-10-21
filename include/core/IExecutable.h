#ifndef _IEXECUTABLE_H_
#define _IEXECUTABLE_H_

#include "CExecutionContext.h"

//! Interface for classes that can be executed when the project runs
class IExecutable
{
public:
    virtual ~IExecutable() = default;

    //! @brief Executes this object using the provided context
    //! @param[in] rContext Reference to the execution context being used
    //! @return True if success
    virtual bool Execute(CExecutionContext& rContext) const = 0;
};

#endif