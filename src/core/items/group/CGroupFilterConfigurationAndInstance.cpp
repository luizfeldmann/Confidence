#include "core/items/group/CGroupFilterConfigurationAndInstance.h"

DEFINE_SERIALIZATION_SCHEME(CGroupFilterConfigurationAndInstance,
    SERIALIZATION_INHERIT(CGroupFilterConfiguration)
    SERIALIZATION_INHERIT(CGroupFilterInstance)
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

bool CGroupFilterConfigurationAndInstance::Filter(const CGroup& rGroup, const CExecutionContext& rContext) const
{
    return CGroupFilterConfiguration::Filter(rGroup, rContext)
        && CGroupFilterInstance::Filter(rGroup, rContext);
}

bool CGroupFilterConfigurationAndInstance::Document(IDocExporter& rExporter) const
{
    return CGroupFilterConfiguration::Document(rExporter)
        && CGroupFilterInstance::Document(rExporter);
}

bool CGroupFilterConfigurationAndInstance::PreSerialize()
{
    return CGroupFilterConfiguration::PreSerialize()
        && CGroupFilterInstance::PreSerialize();
}

bool CGroupFilterConfigurationAndInstance::PostDeserialize(CProject& rProject)
{
    return CGroupFilterConfiguration::PostDeserialize(rProject)
        && CGroupFilterInstance::PostDeserialize(rProject);
}