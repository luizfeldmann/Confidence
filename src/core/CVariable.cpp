#include "core/CVariable.h"
#include "core/CConfiguration.h"
#include "core/CEnvironmentVariable.h"
#include "util/Log.h"
#include <cstdlib>

DEFINE_SERIALIZATION_SCHEME(CVariable,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_MEMBER(m_bExportToEnvironment)
    SERIALIZATION_MEMBER(m_vRules)
)

REGISTER_POLYMORPHIC_CLASS(CVariable);

/* CVariable */
CVariable::CVariable()
    : CStoredNameItem("<new variable>")
    , CStoredDescriptionItem("<no variable description>")
    , m_bExportToEnvironment(false)
{

}

IProjTreeItem* CVariable::Create()
{
    return new CVariable();
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

IExpression& CVariable::AddRule(const CConfiguration& rKeyConfig, const CInstance& rKeyInstance)
{
    m_vRules.emplace_back(rKeyConfig, rKeyInstance);
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

IExpression* CVariable::GetRule(const CConfiguration& rKeyConfig, const CInstance& rKeyInstance)
{
    IExpression* pFoundRule = nullptr;

    vec_rules_t::iterator itFind = std::find_if(m_vRules.begin(), m_vRules.end(), 
        std::bind(&CVariableExpressionKey::Compare, std::placeholders::_1, std::ref(rKeyConfig), std::ref(rKeyInstance)));

    if (m_vRules.end() != itFind)
        pFoundRule = &*itFind;

    return pFoundRule;
}

const IExpression* CVariable::GetRule(const CConfiguration& rKeyConfig, const CInstance& rKeyInstance) const
{
    const IExpression* pFoundRule = nullptr;

    vec_rules_t::const_iterator itFind = std::find_if(m_vRules.cbegin(), m_vRules.cend(),
        std::bind(&CVariableExpressionKey::Compare, std::placeholders::_1, std::ref(rKeyConfig), std::ref(rKeyInstance)));

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
    const CConfiguration* pConfiguration = &rContext.m_rConfiguration;
    
    // Find the expression for the variable
    // Search in the current and the parent configurations (inheritance)
    do
    {
        pExpression = GetRule(*pConfiguration, rContext.m_rInstance);

        if (!pExpression)
            pConfiguration = dynamic_cast<const CConfiguration*>(rContext.GetParent(*pConfiguration));

    } while (pConfiguration && !pExpression);

    if (!pExpression)
        CINFO("Variable '%s' is undefined", strVarName.c_str());
    else
    {
        // Alert user about applied inheritance
        if (pConfiguration && pConfiguration != &rContext.m_rConfiguration)
        {
            const std::string strCfgName = pConfiguration->GetName();
            CINFO("Variable '%s' inherits from configuration '%s'", strVarName.c_str(), strCfgName.c_str());
        }

        // Evaluate expression to a literal
        std::string strValue = pExpression->GetExpression();
        bStatus = rContext.Evaluate(strValue);

        if (bStatus)
        {
            // Set the variable in the context
            rContext.SetVariableLiteral(strVarName, strValue);

            // If requested, export to environment
            if (m_bExportToEnvironment)
            {
                std::shared_ptr<CEnvironmentVariable> pEnv = 
                    std::make_shared<CEnvironmentVariable>(strVarName);
                
                bStatus = pEnv && pEnv->Set(strValue.c_str());
                if (bStatus)
                    rContext.Store(pEnv);
            }
        }
    }

    return bStatus;
}