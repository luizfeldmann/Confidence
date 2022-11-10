#include "core/items/group/CGroupFilterConfiguration.h"
#include "core/items/CConfiguration.h"
#include "core/items/CProject.h"
#include "core/CExecutionContext.h"
#include "util/Log.h"
#include <algorithm>

DEFINE_SERIALIZATION_SCHEME(CGroupFilterConfiguration,
    SERIALIZATION_MEMBER(m_vConfigurationNames)
)

REGISTER_POLYMORPHIC_CLASS(CGroupFilterConfiguration);

CGroupFilterConfiguration::CGroupFilterConfiguration()
    : m_vConfigurationNames(m_vItemNames)
{

}

CGroupFilterConfiguration::~CGroupFilterConfiguration()
{

}

/* static */ std::unique_ptr<IGroupFilter> CGroupFilterConfiguration::Create()
{
    return std::make_unique<CGroupFilterConfiguration>();
}

bool CGroupFilterConfiguration::Filter(const CGroup& rGroup, const CExecutionContext& rContext) const
{
    return CGroupFilterBase::Filter(rContext.GetConfiguration());
}

bool CGroupFilterConfiguration::Document(IDocExporter& rExporter) const
{
    bool bStatus = CGroupFilterBase::Document(rExporter);
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