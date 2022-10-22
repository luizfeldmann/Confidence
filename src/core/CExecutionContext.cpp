#include "core/CExecutionContext.h"
#include "core/CProject.h"
#include "core/CInstance.h"
#include "core/CConfiguration.h"
#include "core/EBuiltInVariables.h"
#include "util/Log.h"
#include <regex>
#include <filesystem>

CExecutionContext::CExecutionContext(const CProject& rProj, const CInstance& rInst, const CConfiguration& rCfg)
    : m_rProject(rProj)
    , m_rInstance(rInst)
    , m_rConfiguration(rCfg)
{
    MapParents(rProj);

    // Set the built-in variables
    m_mVariables[GetBuiltinName(EBuiltInVariable::ProjectName)] = m_rProject.GetName();
    m_mVariables[GetBuiltinName(EBuiltInVariable::ConfigurationName)] = m_rConfiguration.GetName();
    m_mVariables[GetBuiltinName(EBuiltInVariable::InstanceName)] = m_rInstance.GetName();

    const std::string strProjectFile = m_rProject.GetCurrentPath();
    if (!strProjectFile.empty())
    {
        m_mVariables[GetBuiltinName(EBuiltInVariable::ProjectFile)] = strProjectFile;
        m_mVariables[GetBuiltinName(EBuiltInVariable::ProjectDir)] = std::filesystem::path(strProjectFile).parent_path().string();
    }
}

void CExecutionContext::MapParents(const IProjTreeItem& rParent)
{
    IProjTreeItem::vec_cref_t vSubitems = rParent.SubItems();
    for (const IProjTreeItem::cref_t& rItem : vSubitems)
    {
        m_mParents[&rItem.get()] = &rParent;
        MapParents(rItem.get());
    }
}

const IProjTreeItem* CExecutionContext::GetParent(const IProjTreeItem& rItem) const
{
    const IProjTreeItem* pFind = nullptr;

    parent_map_t::const_iterator itFind = m_mParents.find(&rItem);
    if (m_mParents.cend() != itFind)
        pFind = itFind->second;

    return pFind;
}

void CExecutionContext::SetVariableLiteral(const std::string& strKey, const std::string& strValue)
{
    m_mVariables[strKey] = strValue;
    CLOG("%s = %s", strKey.c_str(), strValue.c_str());
}

bool CExecutionContext::SetVariableEvaluate(const std::string& strKey, const std::string& strExpression)
{
    std::string strValue = strExpression; 
    bool bStatus = Evaluate(strValue);

    if (bStatus)
        SetVariableLiteral(strKey, strValue);

    return bStatus;
}

std::optional<std::string> CExecutionContext::GetVariable(const std::string& strKey) const
{
    std::optional<std::string> optValue;

    variable_map_t::const_iterator itFind = m_mVariables.find(strKey);
    if (m_mVariables.cend() != itFind)
        optValue = itFind->second;

    return optValue;
}

bool CExecutionContext::Evaluate(std::string& strExpression) const
{
    //! Regex to find expressions in format $(VariableName)
    const static std::regex c_varRegex("\\$\\(([\\w|\\d]+)\\)");

    //! Indexes the match groups of the Regex
    enum class EGroupIndex : int
    {
        WholeMatch,     //!< Matches the whole expression $(VariableName)
        VariableName,   //!< Matches only the name inside the $()
        Count,          //!< Expected size of the match
    };

    bool bStatus = true;

    // While any matches exist, keep modifying the string
    std::smatch match;
    while (std::regex_search(strExpression, match, c_varRegex) && match.size() == (int)EGroupIndex::Count)
    {
        const std::ssub_match matchWhole = match[(int)EGroupIndex::WholeMatch];
        const std::ssub_match matchName  = match[(int)EGroupIndex::VariableName];

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

void CExecutionContext::Store(std::shared_ptr<IContextStorage> pStore)
{
    m_lStorage.push_back(pStore);
}