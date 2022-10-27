#include "core/CExecutionContext.h"
#include "core/CProject.h"
#include "core/CInstance.h"
#include "core/CConfiguration.h"
#include "core/EBuiltInVariables.h"
#include <cassert>

CExecutionContext::CExecutionContext(const std::shared_ptr<const CProject>& pProject, const std::shared_ptr<const CInstance>& pInstance, const std::shared_ptr<const CConfiguration>& pConfiguration)
    : CExecutionContextBase(pProject)
    , m_pInstance(pInstance)
    , m_pConfiguration(pConfiguration)
{
    MapParents(pProject);

    // Set the built-in variables
    assert(pConfiguration && "Creating execution context from null configuration");
    m_mVariables[GetBuiltinName(EBuiltInVariable::ConfigurationName)] = pConfiguration->GetName();

    assert(pInstance && "Creating execution context from null instance");
    m_mVariables[GetBuiltinName(EBuiltInVariable::InstanceName)] = pInstance->GetName();
}

void CExecutionContext::MapParents(const IProjTreeItem::cptr_t& pParent)
{
    assert(pParent && "Mapping children of null parent");

    IProjTreeItem::vec_cptr_t vSubitems = pParent->SubItems();
    for (const IProjTreeItem::cptr_t& pItem : vSubitems)
    {
        m_mParents[pItem.get()] = pParent;
        MapParents(pItem);
    }
}

std::shared_ptr<const CInstance> CExecutionContext::GetInstance() const
{
    std::shared_ptr<const CInstance> pInstance = m_pInstance.lock();
    assert(pInstance && "Execution context instance is null");

    return pInstance;
}

std::shared_ptr<const CConfiguration> CExecutionContext::GetConfiguration() const
{
    std::shared_ptr<const CConfiguration> pConfiguration = m_pConfiguration.lock();
    assert(pConfiguration && "Execution context configuration is null");

    return pConfiguration;
}

IProjTreeItem::cptr_t CExecutionContext::GetParent(const IProjTreeItem* pItem) const
{
    assert(pItem && "Getting parent of a null item");

    IProjTreeItem::cptr_t pFind;

    parent_map_t::const_iterator itFind = m_mParents.find(pItem);
    if (m_mParents.cend() != itFind)
        pFind = itFind->second.lock();

    return pFind;
}

void CExecutionContext::Store(std::shared_ptr<IContextStorage> pStore)
{
    m_lStorage.push_back(pStore);
}