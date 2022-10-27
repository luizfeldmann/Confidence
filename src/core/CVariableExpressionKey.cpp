#include "core/CVariableExpressionKey.h"
#include "core/CConfiguration.h"
#include "core/CInstance.h"
#include "core/CProject.h"
#include "util/Log.h"
#include <cassert>

DEFINE_SERIALIZATION_SCHEME(CVariableExpressionKey,
    SERIALIZATION_MEMBER(m_strConfiguration)
    SERIALIZATION_MEMBER(m_strInstance)
    SERIALIZATION_INHERIT(CStoredExpression)
)

CVariableExpressionKey::CVariableExpressionKey()
    : m_pConfiguration()
    , m_pInstance()
{

}

CVariableExpressionKey::CVariableExpressionKey(const std::weak_ptr<const CConfiguration>& pConfig, const std::weak_ptr<const CInstance>& pInst)
    : m_pConfiguration(pConfig)
    , m_pInstance(pInst)
{

}

std::shared_ptr<const CConfiguration> CVariableExpressionKey::GetConfiguration() const
{
    return m_pConfiguration.lock();
}

std::shared_ptr<const CInstance> CVariableExpressionKey::GetInstance() const
{
    return m_pInstance.lock();
}

bool CVariableExpressionKey::Compare(const CConfiguration* pConfig, const CInstance* pInst) const
{
    return (pConfig == GetConfiguration().get())
        && (pInst == GetInstance().get());
}

bool CVariableExpressionKey::IsValid() const
{
    bool bStatus = GetConfiguration()
        && GetInstance();

    return bStatus;
}

bool CVariableExpressionKey::PreSerialize()
{
    bool bValid = true;

    // Update configuration name before serialization
    std::shared_ptr<const CConfiguration> const pConfig = GetConfiguration();
    if (pConfig)
        m_strConfiguration = pConfig->GetName();
    else
    {
        bValid = false;
        m_strConfiguration.clear();
    }

    // Update instance name before serialization
    std::shared_ptr<const CInstance> const pInst = GetInstance();
    if (pInst)
        m_strInstance = pInst->GetName();
    else
    {
        bValid = false;
        m_strInstance.clear();
    }

    return bValid;
}

bool CVariableExpressionKey::PostDeserialize(CProject& rProject)
{
    bool bResult = true;

    std::shared_ptr<const CConfiguration> const pConfig = rProject.GetConfiguration(m_strConfiguration);

    if (!pConfig)
    {
        bResult = false;
        CERROR("Cannot find configuration named '%s'", m_strConfiguration.c_str());
    }
    else
    {
        m_pConfiguration = pConfig;
    }

    std::shared_ptr<const CInstance> const pInst = rProject.GetInstance(m_strInstance);
    if (!pInst)
    {
        bResult = false;
        CERROR("Cannot find instance named '%s'", m_strInstance.c_str());
    }
    else
    {
        m_pInstance = pInst;
    }

    return bResult;
}

bool CVariableExpressionKey::Document(IDocExporter& rExporter) const
{
    std::shared_ptr<const CConfiguration> const pConfig = GetConfiguration();
    std::shared_ptr<const CInstance const> pInst = GetInstance();

    bool bStatus = false;
    
    if (pConfig && pInst)
    {
        bStatus = rExporter.FormField("Configuration:", pConfig->GetName())
            && rExporter.FormField("Instance:", pInst->GetName())
            && rExporter.Paragraph()
            && rExporter.Code(GetExpression())
            && rExporter.PopStack();
    }

    return bStatus;
}