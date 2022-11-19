#ifndef _ICONDITION_H_
#define _ICONDITION_H_

#include "docs/IDocumentable.h"
#include "core/IDependsOnVariables.h"
#include <memory>

class CEvaluationContext;

//! @brief An object that can be evaluated to a boolean
class ICondition : public IDocumentable, public IDependsOnVariables, public std::enable_shared_from_this<ICondition>
{
public:
    virtual ~ICondition() = default;

    //! @brief Categories of conditions
    enum class ECategory
    {
        Operator,   //!< Associates sub-conditions
        Comparator  //!< Relations between expressions
    };

    //! @brief Reflects the condition category
    virtual ECategory GetCategory() const = 0;

    //! @brief Gets a string representation of the infix condition
    virtual std::string GetInfix() const = 0;

    //! @brief Evaluates the condition to a boolean
    //! @param[in] rContext The context used to assert the condition
    //! @param[out] bResult Receives the evaluated value of the condition
    //! @return True on success; false on error
    virtual bool Evaluate(CEvaluationContext& rContext, bool& bResult) const = 0;
};

#endif