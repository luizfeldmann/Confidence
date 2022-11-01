#include "core/items/variable/CVariable.h"
#include "core/items/CConfiguration.h"
#include "core/items/variable/CEnvironmentVariable.h"
#include "util/Log.h"
#include <cstdlib>

DEFINE_SERIALIZATION_SCHEME(CVariable,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_INHERIT(CAssignable)
    SERIALIZATION_MEMBER(m_vRules)
)

REGISTER_POLYMORPHIC_CLASS(CVariable);

/* CVariable */
CVariable::CVariable()
    : CStoredNameItem("<new variable>")
    , CStoredDescriptionItem("<no variable description>")
{

}

IProjTreeItem::ptr_t CVariable::Create()
{
    return ptr_t(new CVariable);
}

CVariable::~CVariable()
{

}

ETreeItemType CVariable::GetType() const
{
    return EVariable;
}

bool CVariable::PostDeserialize(CProject& rProject)
{
    bool bSuccess = true;

    for (CVariableExpressionKey& rKey : m_vRules)
        bSuccess = bSuccess && rKey.PostDeserialize(rProject);

    return bSuccess;
}

bool CVariable::PreSerialize()
{
    // Remove invalid expressions before serializing
    m_vRules.erase(std::remove_if(m_vRules.begin(), m_vRules.end(),
        [](CVariableExpressionKey& rKey)->bool
        {
            return !rKey.PreSerialize();
        }), m_vRules.end());

    return true;
}

IExpression& CVariable::AddRule(std::weak_ptr<const CConfiguration> pKeyConfig, std::weak_ptr<const CInstance> pKeyInstance)
{
    m_vRules.emplace_back(pKeyConfig, pKeyInstance);
    return m_vRules.back();
}

bool CVariable::EraseRule(const IExpression* pExpr)
{
    bool bSuccess = false;

    vec_rules_t::const_iterator itFind = std::find_if(m_vRules.cbegin(), m_vRules.cend(),
        [&pExpr](const IExpression& rSearch) -> bool {
            return (&rSearch == pExpr);
        }
    );

    if (m_vRules.cend() != itFind)
    {
        m_vRules.erase(itFind);
        bSuccess = true;
    }

    return bSuccess;
}

IExpression* CVariable::GetRule(const CConfiguration* pKeyConfig, const CInstance* pKeyInstance)
{
    IExpression* pFoundRule = nullptr;

    vec_rules_t::iterator itFind = std::find_if(m_vRules.begin(), m_vRules.end(), 
        std::bind(&CVariableExpressionKey::Compare, std::placeholders::_1, pKeyConfig, pKeyInstance));

    if (m_vRules.end() != itFind)
        pFoundRule = &*itFind;

    return pFoundRule;
}

const IExpression* CVariable::GetRule(const CConfiguration* pKeyConfig, const CInstance* pKeyInstance) const
{
    const IExpression* pFoundRule = nullptr;

    vec_rules_t::const_iterator itFind = std::find_if(m_vRules.cbegin(), m_vRules.cend(),
        std::bind(&CVariableExpressionKey::Compare, std::placeholders::_1, pKeyConfig, pKeyInstance));

    if (m_vRules.cend() != itFind)
        pFoundRule = &*itFind;

    return pFoundRule;
}

bool CVariable::DocumentName(IDocExporter& rExporter) const
{
    return IProjTreeItem::DocumentName(rExporter, "VARIABLE:");
}

bool CVariable::DocumentCustom(IDocExporter& rExporter) const
{
    bool bStatus = rExporter.List();

    for (vec_rules_t::const_iterator it = m_vRules.cbegin(); bStatus && (it != m_vRules.cend()); ++it)
    {
        // Skip rules related to deleted configurations and / instances
        // Those rules will be removed when the project is saved
        if (!it->IsValid())
            continue;

        bStatus = rExporter.Item()
            && it->Document(rExporter)
            && rExporter.PopStack();
    }

    bStatus = bStatus && rExporter.PopStack();

    return bStatus;
}

bool CVariable::Execute(CExecutionContext& rContext) const
{
    LogExecution();

    bool bStatus = true;
    const std::string strVarName = GetName();

    const IExpression* pExpression = nullptr;
    std::shared_ptr<const CConfiguration> pConfiguration = rContext.GetConfiguration();

    // Find the expression for the variable
    // Search in the current and the parent configurations (inheritance)
    do
    {
        pExpression = GetRule(pConfiguration.get(), rContext.GetInstance().get());

        if (!pExpression)
            pConfiguration = std::dynamic_pointer_cast<const CConfiguration>( rContext.GetParent(pConfiguration.get()) );

    } while (pConfiguration && !pExpression);

    if (!pExpression)
        CINFO("Variable '%s' is undefined", strVarName.c_str());
    else
    {
        // Alert user about applied inheritance
        if (pConfiguration && pConfiguration != rContext.GetConfiguration())
        {
            const std::string strCfgName = pConfiguration->GetName();
            CINFO("Variable '%s' inherits from configuration '%s'", strVarName.c_str(), strCfgName.c_str());
        }

        // Evaluate expression to a literal
        std::string strValue = pExpression->GetExpression();
        bStatus = Assign(rContext, strVarName, strValue);
    }

    return bStatus;
}