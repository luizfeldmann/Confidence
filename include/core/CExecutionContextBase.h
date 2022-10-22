#ifndef _CEXECUTIONCONTEXTBASE_H_
#define _CEXECUTIONCONTEXTBASE_H_

#include "core/CEvaluationContext.h"

class CProject;

//! @brief Base execution context
//! Allows execution of items dependant only on project-wide information
class CExecutionContextBase : public CEvaluationContext
{
private:
    CExecutionContextBase(const CExecutionContextBase&) = delete;
    CExecutionContextBase& operator=(const CExecutionContextBase&) = delete;

public:
    //! Reference to the project being executed
    const CProject& m_rProject;

    CExecutionContextBase(const CProject& rProj);
};

#endif