#ifndef _ICONDITION_H_
#define _ICONDITION_H_

#include "docs/IDocumentable.h"

class CEvaluationContext;

//! @brief An object that can be evaluated to a boolean
class ICondition : public IDocumentable
{
public:
    virtual ~ICondition() = default;

    //! @brief Evaluates the condition to a boolean
    //! @param[in] rContext The context used to assert the condition
    //! @param[out] bResult Receives the evaluated value of the condition
    //! @return True on success; false on error
    virtual bool Evaluate(CEvaluationContext& rContext, bool& bResult) const = 0;
};

#endif