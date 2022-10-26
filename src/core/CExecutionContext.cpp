#include "core/CExecutionContext.h"
#include "core/CProject.h"
#include "core/CInstance.h"
#include "core/CConfiguration.h"
#include "core/EBuiltInVariables.h"

CExecutionContext::CExecutionContext(const CProject& rProj, const CInstance& rInst, const CConfiguration& rCfg)
    : CExecutionContextBase(rProj)
    , m_rInstance(rInst)
    , m_rConfiguration(rCfg)
{
    MapParents(rProj);

    // Set the built-in variables
    m_mVariables[GetBuiltinName(EBuiltInVariable::ConfigurationName)] = m_rConfiguration.GetName();
    m_mVariables[GetBuiltinName(EBuiltInVariable::InstanceName)] = m_rInstance.GetName();
}

void CExecutionContext::MapParents(const IProjTreeItem& rParent)
{
    IProjTreeItem::vec_cptr_t vSubitems = rParent.SubItems();
    for (const IProjTreeItem::cptr_t& rItem : vSubitems)
    {
        m_mParents[rItem.get()] = &rParent;
        MapParents(*rItem);
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

void CExecutionContext::Store(std::shared_ptr<IContextStorage> pStore)
{
    m_lStorage.push_back(pStore);
}