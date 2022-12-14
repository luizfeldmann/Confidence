/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "core/CStoredExpression.h"

DEFINE_SERIALIZATION_SCHEME(CStoredExpression,
    SERIALIZATION_MEMBER(m_strExpression)
)

std::string CStoredExpression::GetExpression() const
{
    return m_strExpression;
}

bool CStoredExpression::SetExpression(const std::string& strNewExpression)
{
    m_strExpression = strNewExpression;
    return true;
}