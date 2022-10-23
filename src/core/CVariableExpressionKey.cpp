#include "core/CVariableExpressionKey.h"
#include "core/CConfiguration.h"
#include "core/CInstance.h"
#include "util/Log.h"

DEFINE_SERIALIZATION_SCHEME(CVariableExpressionKey,
    SERIALIZATION_MEMBER(m_strConfiguration)
    SERIALIZATION_MEMBER(m_strInstance)
    SERIALIZATION_INHERIT(CStoredExpression)
)

CVariableExpressionKey::CVariableExpressionKey()
    : m_gIdConfiguration(CGuid::Null())
    , m_gIdInstance(CGuid::Null())
{

}

CVariableExpressionKey::CVariableExpressionKey(const CConfiguration& rConfig, const CInstance& rInst)
    : m_gIdConfiguration(rConfig.GetID())
    , m_gIdInstance(rInst.GetID())
{

}

const CConfiguration* CVariableExpressionKey::GetConfiguration() const
{
    return CConfiguration::FindByID(m_gIdConfiguration);
}

const CInstance* CVariableExpressionKey::GetInstance() const
{
    return CInstance::FindByID(m_gIdInstance);
}

bool CVariableExpressionKey::Compare(const CConfiguration& rConfig, const CInstance& rInst) const
{
    return (rConfig.GetID() == m_gIdConfiguration)
        && (rInst.GetID() == m_gIdInstance);
}

bool CVariableExpressionKey::PreSerialize()
{
    bool bValid = true;

    // Update configuration name before serialization
    const CConfiguration* const pConfig = GetConfiguration();
    if (pConfig)
        m_strConfiguration = pConfig->GetName();
    else
    {
        bValid = false;
        m_strConfiguration.clear();
    }

    // Update instance name before serialization
    const CInstance* const pInst = GetInstance();
    if (pInst)
        m_strInstance = pInst->GetName();
    else
    {
        bValid = false;
        m_strInstance.clear();
    }

    return bValid;
}

bool CVariableExpressionKey::PostDeserialize()
{
    bool bResult = false;

    const CConfiguration* const pConfig = CConfiguration::FindByName(m_strConfiguration);

    if (!pConfig)
        CERROR("Cannot find configuration named '%s'", m_strConfiguration.c_str());
    else
    {
        m_gIdConfiguration = pConfig->GetID();

        const CInstance* const pInst = CInstance::FindByName(m_strInstance);
        if (!pInst)
            CERROR("Cannot find instance named '%s'", m_strInstance.c_str());
        else
        {
            m_gIdInstance = pInst->GetID();
            bResult = true;
        }
    }

    return bResult;
}

bool CVariableExpressionKey::Document(IDocExporter& rExporter) const
{
    // Read names of keys
    std::string strConfigName   = "<INVALID CONFIG>";
    std::string strInstanceName = "<INVALID INSTANCE>";

    {
        const CConfiguration* const pConfig = GetConfiguration();
        if (pConfig)
            strConfigName = pConfig->GetName();

        const CInstance* const pInst = GetInstance();
        if (pInst)
            strInstanceName = pInst->GetName();
    }

    bool bStatus = rExporter.FormField("Configuration:", strConfigName)
        && rExporter.FormField("Instance:", strInstanceName)
        && rExporter.Paragraph()
        && rExporter.Code(GetExpression())
        && rExporter.PopStack();

    return bStatus;
}