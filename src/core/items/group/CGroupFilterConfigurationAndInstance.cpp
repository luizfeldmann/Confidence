/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "core/items/group/CGroupFilterConfigurationAndInstance.h"
#include "core/IProjTreeItem.h"
#include "docs/IDocExporter.h"

DEFINE_SERIALIZATION_SCHEME(CGroupFilterConfigurationAndInstance,
    SERIALIZATION_MEMBER(m_byConfig)
    SERIALIZATION_MEMBER(m_byInstance)
)

REGISTER_POLYMORPHIC_CLASS(CGroupFilterConfigurationAndInstance);

CGroupFilterConfigurationAndInstance::CGroupFilterConfigurationAndInstance()
{

}

CGroupFilterConfigurationAndInstance::~CGroupFilterConfigurationAndInstance()
{

}

/* static */  std::unique_ptr<IGroupFilter> CGroupFilterConfigurationAndInstance::Create()
{
    return std::make_unique<CGroupFilterConfigurationAndInstance>();
}

/* static */ ETreeItemType CGroupFilterConfigurationAndInstance::GetOwnType()
{
    return ETreeItemType(CGroupFilterConfiguration::GetOwnType() | CGroupFilterInstance::GetOwnType());
}

ETreeItemType CGroupFilterConfigurationAndInstance::GetType() const
{
    return GetOwnType();
}

void CGroupFilterConfigurationAndInstance::push_back(wptr_t pItem)
{
    IGroupFilter* pFilter = GetSubFilter(pItem.lock());
    if (pFilter)
        pFilter->push_back(pItem);
}

void CGroupFilterConfigurationAndInstance::erase(sptr_t pItem)
{
    IGroupFilter* pFilter = GetSubFilter(pItem);
    if (pFilter)
        pFilter->erase(pItem);
}

CGroupFilterConfigurationAndInstance::ptrvec_t CGroupFilterConfigurationAndInstance::GetItems() const
{
    // Gets the list of items from each subfilter
    ptrvec_t vItems1 = m_byInstance.GetItems();
    ptrvec_t vItems2 = m_byConfig.GetItems();

    // Concatenate the list
    vItems2.insert(vItems2.end(), vItems1.begin(), vItems1.end());

    return vItems2;
}

bool CGroupFilterConfigurationAndInstance::FilterItem(sptr_t pItem) const
{
    bool bFilter = false;

    const IGroupFilter* pFilter = GetSubFilter(pItem);
    if (pFilter)
        bFilter = pFilter->FilterItem(pItem);

    return bFilter;
}

IGroupFilter* CGroupFilterConfigurationAndInstance::GetSubFilter(sptr_t pItem)
{
    IGroupFilter* pFilter = nullptr;

    if (pItem)
    {
        const ETreeItemType eType = pItem->GetType();

        if (eType & CGroupFilterConfiguration::GetOwnType())
            pFilter = &m_byConfig;
        else if (eType & CGroupFilterInstance::GetOwnType())
            pFilter = &m_byInstance;
    }

    return pFilter;
}

const IGroupFilter* CGroupFilterConfigurationAndInstance::GetSubFilter(sptr_t pItem) const
{
    const IGroupFilter* pFilter = nullptr;

    if (pItem)
    {
        const ETreeItemType eType = pItem->GetType();

        if (eType & CGroupFilterConfiguration::GetOwnType())
            pFilter = &m_byConfig;
        else if (eType & CGroupFilterInstance::GetOwnType())
            pFilter = &m_byInstance;
    }

    return pFilter;
}

bool CGroupFilterConfigurationAndInstance::Filter(const CGroup& rGroup, const CExecutionContext& rContext) const
{
    return m_byConfig.Filter(rGroup, rContext)
        && m_byInstance.Filter(rGroup, rContext);
}

bool CGroupFilterConfigurationAndInstance::Document(IDocExporter& rExporter) const
{
    bool bStatus = rExporter.FormField("Filter Configuration & Instance:", 
        "Both the configuration filter and the instance filter must be satisfied");
    bStatus = bStatus && m_byConfig.Document(rExporter);
    bStatus = bStatus && m_byInstance.Document(rExporter);

    return bStatus;
}

bool CGroupFilterConfigurationAndInstance::PreSerialize()
{
    return m_byConfig.PreSerialize()
        && m_byInstance.PreSerialize();
}

bool CGroupFilterConfigurationAndInstance::PostDeserialize(CProject& rProject)
{
    return m_byConfig.PostDeserialize(rProject)
        && m_byInstance.PostDeserialize(rProject);
}