/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "core/IExpression.h"
#include "core/CEvaluationContext.h"

std::vector<std::string> IExpression::GetDependencies() const
{
    return CEvaluationContext::ListVariables(GetExpression());
}