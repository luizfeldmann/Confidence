#include "core/CVariableExpressionKey.h"
#include "core/CConfiguration.h"
#include "core/CInstance.h"
#include "util/Log.h"

DEFINE_SERIALIZATION_SCHEME(CVariableExpressionKey,
    SERIALIZATION_MEMBER(m_strConfiguration)
    SERIALIZATION_MEMBER(m_strInstance)
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

CConfiguration* CVariableExpressionKey::GetConfiguration() const
{
    return dynamic_cast<CConfiguration*>(IProjTreeItem::FindByID(m_gIdConfiguration));
}

CInstance* CVariableExpressionKey::GetInstance() const
{
    return dynamic_cast<CInstance*>(IProjTreeItem::FindByID(m_gIdInstance));
}

bool CVariableExpressionKey::PreSerialize()
{
    // Update configuration name before serialization
    const CConfiguration* const pConfig = GetConfiguration();
    if (pConfig)
        m_strConfiguration = pConfig->GetName();
    else
        m_strConfiguration.clear();

    // Update instance name before serialization
    const CInstance* const pInst = GetInstance();
    if (pInst)
        m_strInstance = pInst->GetName();
    else
        m_strInstance.clear();

    return true;
}

bool CVariableExpressionKey::PostDeserialize()
{
    bool bFoundConfig = false;
    bool bFoundInstance = false;

    for (IProjTreeItem::const_iterator it = IProjTreeItem::TrackedBegin(); 
        (it != IProjTreeItem::TrackedEnd()) && (!bFoundConfig || !bFoundInstance); 
        ++it)
    {
        // Search the ID of the configuration
        if (!bFoundConfig)
        {
            const CConfiguration* const pConfig = dynamic_cast<const CConfiguration*>(*it);
            if (pConfig)
            {
                const std::string strName = pConfig->GetName();

                if (strName == m_strConfiguration)
                {
                    m_gIdConfiguration = pConfig->GetID();
                    bFoundConfig = true;
                }
            }
        }

        // Search the ID of the instance
        if (!bFoundInstance)
        {
            const CInstance* const pInst = dynamic_cast<const CInstance*>(*it);
            if (pInst)
            {
                const std::string strName = pInst->GetName();

                if (strName == m_strInstance)
                {
                    m_gIdInstance = pInst->GetID();
                    bFoundInstance = true;
                }
            }
        }
    }

    if (!bFoundConfig)
        CERROR("Cannot find configuration named '%s'", m_strConfiguration.c_str());

    if (!bFoundInstance)
        CERROR("Cannot find instance named '%s'", m_strInstance.c_str());

    return (bFoundConfig && bFoundInstance);
}