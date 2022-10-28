#include "core/items/CConfigurationGroup.h"

DEFINE_SERIALIZATION_SCHEME(CConfigurationGroup,
    SERIALIZATION_INHERIT(CConfiguration)
)

REGISTER_POLYMORPHIC_CLASS(CConfigurationGroup);

CConfigurationGroup::CConfigurationGroup()
    : CConfiguration("Default", "Default configuration")
{

}

CConfigurationGroup::~CConfigurationGroup()
{

}

/* static */ std::shared_ptr<CConfigurationGroup> CConfigurationGroup::Create()
{
    return std::shared_ptr<CConfigurationGroup>(new CConfigurationGroup);
}

ETreeItemType CConfigurationGroup::GetType() const
{
    return EConfGroup;
}

bool CConfigurationGroup::Execute(CExecutionContext&) const
{
    return true; // Do nothing
}