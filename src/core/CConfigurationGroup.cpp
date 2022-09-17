#include "core/CConfigurationGroup.h"

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

ETreeItemType CConfigurationGroup::GetType() const
{
    return EConfGroup;
}