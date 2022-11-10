#include "core/items/group/CGroupFilterInstance.h"
#include "core/items/CInstance.h"
#include "core/items/CProject.h"
#include "core/CExecutionContext.h"
#include "util/Log.h"
#include <algorithm>

DEFINE_SERIALIZATION_SCHEME(CGroupFilterInstance,
    SERIALIZATION_MEMBER(m_vInstanceNames)
)

REGISTER_POLYMORPHIC_CLASS(CGroupFilterInstance);

CGroupFilterInstance::CGroupFilterInstance()
    : m_vInstanceNames(m_vItemNames)
{

}

CGroupFilterInstance::~CGroupFilterInstance()
{

}

/* static */ std::unique_ptr<IGroupFilter> CGroupFilterInstance::Create()
{
    return std::make_unique<CGroupFilterInstance>();
}

bool CGroupFilterInstance::Filter(const CGroup& rGroup, const CExecutionContext& rContext) const
{
    return CGroupFilterBase::Filter(rContext.GetInstance());
}

bool CGroupFilterInstance::Document(IDocExporter& rExporter) const
{
    bool bStatus = CGroupFilterBase::Document(rExporter);
    return bStatus;
}

bool CGroupFilterInstance::PostDeserialize(CProject& rProject)
{
    bool bStatus = true;

    m_vItemPtrs.clear();

    for (const std::string& strInstanceName : m_vItemNames)
    {
        sptr_t sPtr = rProject.GetInstance(strInstanceName);

        if (!sPtr)
        {
            CERROR("Cannot find instance named '%s'", strInstanceName.c_str());
            bStatus = false;
            break;
        }

        m_vItemPtrs.push_back( sPtr );
    }

    return bStatus;
}