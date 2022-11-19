#include "core/IExpression.h"
#include "core/CEvaluationContext.h"

std::vector<std::string> IExpression::GetDependencies() const
{
    return CEvaluationContext::ListVariables(GetExpression());
}