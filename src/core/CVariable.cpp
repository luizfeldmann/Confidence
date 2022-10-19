#include "core/CVariable.h"

DEFINE_SERIALIZATION_SCHEME(CVariable,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_MEMBER(m_bExportToEnvironment)
    SERIALIZATION_MEMBER(m_vRules)
)

REGISTER_POLYMORPHIC_CLASS(CVariable);

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

bool CVariable::PostDeserialize()
{
    bool bSuccess = true;

    for (CVariableExpressionKey& rKey : m_vRules)
        bSuccess = bSuccess && rKey.PostDeserialize();

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