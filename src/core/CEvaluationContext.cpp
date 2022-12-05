/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "core/CEvaluationContext.h"
#include "util/Log.h"
#include "core/items/variable/EBuiltInVariables.h"
#include <regex>

CEvaluationContext::CEvaluationContext()
{

}

void CEvaluationContext::SetVariableLiteral(const std::string& strKey, const std::string& strValue)
{
    m_mVariables[strKey] = strValue;
    CLOG("%s = %s", strKey.c_str(), strValue.c_str());
}

bool CEvaluationContext::SetVariableEvaluate(const std::string& strKey, const std::string& strExpression)
{
    std::string strValue = strExpression;
    bool bStatus = Evaluate(strValue);

    if (bStatus)
        SetVariableLiteral(strKey, strValue);

    return bStatus;
}

std::optional<std::string> CEvaluationContext::GetVariable(const std::string& strKey) const
{
    std::optional<std::string> optValue;

    variable_map_t::const_iterator itFind = m_mVariables.find(strKey);
    if (m_mVariables.cend() != itFind)
        optValue = itFind->second;

    return optValue;
}

//! Indexes the match groups of the Regex
enum class EGroupIndex : int
{
    WholeMatch,     //!< Matches the whole expression $(VariableName)
    VariableName,   //!< Matches only the name inside the $()
    Count,          //!< Expected size of the match
};

bool CEvaluationContext::Evaluate(std::string& strExpression) const
{
    //! Regex to find expressions in format $(VariableName)
    const std::regex& rVarRegex = GetVariableRegex();

    bool bStatus = true;

    // While any matches exist, keep modifying the string
    std::smatch match;
    while (std::regex_search(strExpression, match, rVarRegex) && match.size() == (int)EGroupIndex::Count)
    {
        const std::ssub_match matchWhole = match[(int)EGroupIndex::WholeMatch];
        const std::ssub_match matchName = match[(int)EGroupIndex::VariableName];

        std::string strKey = matchName.str();
        std::optional<std::string> optValue = GetVariable(strKey);

        if (!optValue.has_value())
        {
            CWARNING("Use of undefined variable '%s'", strKey.c_str());
            bStatus = false;
        }

        strExpression.replace(matchWhole.first, matchWhole.second, optValue.value_or(""));
    }

    return bStatus;
}

/* static */ std::vector<std::string> CEvaluationContext::ListVariables(const std::string& strExpression)
{
    std::vector<std::string> vNames;

    for (auto it =  std::sregex_iterator(strExpression.cbegin(), strExpression.cend(), GetVariableRegex());
        it != std::sregex_iterator(); ++it)
    {
        const std::ssub_match matchName = (*it)[(int)EGroupIndex::VariableName];
        vNames.emplace_back( matchName );
    }

    return vNames;
}

/* static */ bool CEvaluationContext::ValidateVariableName(const std::string& strName)
{
    const std::string strExpression = MakeExpression(strName);

    return std::regex_match(strExpression, GetVariableRegex());
}