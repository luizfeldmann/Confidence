/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "core/items/group/CGroupFilterConfiguration.h"
#include "core/items/CConfiguration.h"
#include "core/items/CProject.h"
#include "core/CExecutionContext.h"
#include "util/Log.h"
#include "docs/IDocExporter.h"
#include <algorithm>

DEFINE_SERIALIZATION_SCHEME(CGroupFilterConfiguration,
    SERIALIZATION_INHERIT(CGroupFilterBase)
)

REGISTER_POLYMORPHIC_CLASS(CGroupFilterConfiguration);

CGroupFilterConfiguration::CGroupFilterConfiguration()
{

}

CGroupFilterConfiguration::~CGroupFilterConfiguration()
{

}

/* static */ std::unique_ptr<IGroupFilter> CGroupFilterConfiguration::Create()
{
    return std::make_unique<CGroupFilterConfiguration>();
}

ETreeItemType CGroupFilterConfiguration::GetOwnType()
{
    return ETreeItemType(EConfig | EConfGroup);
}

ETreeItemType CGroupFilterConfiguration::GetType() const
{
    return GetOwnType();
}

bool CGroupFilterConfiguration::Filter(const CGroup& rGroup, const CExecutionContext& rContext) const
{
    return CGroupFilterBase::FilterItem(rContext.GetConfiguration());
}

bool CGroupFilterConfiguration::Document(IDocExporter& rExporter) const
{
    bool bStatus = rExporter.FormField("Filter by Configuration:", "This group will be executed only for the listed configurations");
    bStatus = bStatus && CGroupFilterBase::Document(rExporter);

    return bStatus;
}

bool CGroupFilterConfiguration::PostDeserialize(CProject& rProject)
{
    bool bStatus = true;

    m_vItemPtrs.clear();

    for (const std::string& strConfigurationName : m_vItemNames)
    {
        sptr_t sPtr = rProject.GetConfiguration(strConfigurationName);

        if (!sPtr)
        {
            CERROR("Cannot find configuration named '%s'", strConfigurationName.c_str());
            bStatus = false;
            break;
        }

        m_vItemPtrs.push_back(sPtr);
    }

    return bStatus;
}