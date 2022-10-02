#include "core/CVariable.h"

DEFINE_SERIALIZATION_SCHEME(CVariable,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_MEMBER(m_vRules)
)

REGISTER_POLYMORPHIC_CLASS(CVariable);

CVariable::CVariable()
    : CStoredNameItem("<new variable>")
    , CStoredDescriptionItem("<no variable description>")
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

bool CVariable::PostDeserialize()
{
    bool bSuccess = true;

    for (CVariableExpressionKey& rKey : m_vRules)
        bSuccess = bSuccess && rKey.PostDeserialize();

    return bSuccess;
}

bool CVariable::PreSerialize()
{
    bool bSuccess = true;

    for (CVariableExpressionKey& rKey : m_vRules)
        bSuccess = bSuccess && rKey.PreSerialize();

    return bSuccess;
}

IExpression& CVariable::AddRule(const CConfiguration& rKeyConfig, const CInstance& rKeyInstance)
{
    m_vRules.emplace_back(rKeyConfig, rKeyInstance);
    return m_vRules.back();
}

void CVariable::EraseRule(IExpression& rExpr)
{
    vec_rules_t::const_iterator itFind = std::find_if(m_vRules.cbegin(), m_vRules.cend(),
        [&rExpr](const IExpression& rSearch) -> bool {
            return (&rSearch == &rExpr);
        }
    );

    if (m_vRules.cend() != itFind)
        m_vRules.erase(itFind);
}

IExpression* CVariable::GetRule(const CConfiguration& rKeyConfig, const CInstance& rKeyInstance)
{
    IExpression* pFoundRule = nullptr;

    vec_rules_t::iterator itFind = std::find_if(m_vRules.begin(), m_vRules.end(),
        [&rKeyConfig, &rKeyInstance](const CVariableExpressionKey& rSearch) -> bool {
            const CConfiguration* const pConfig = rSearch.GetConfiguration();
            const CInstance* const pInst = rSearch.GetInstance();

            return (&rKeyConfig == pConfig) && (&rKeyInstance == pInst);
        }
    );

    if (m_vRules.end() != itFind)
        pFoundRule = &*itFind;

    return pFoundRule;
}