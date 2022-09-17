#include "core/CConfiguration.h"

DEFINE_SERIALIZATION_SCHEME(CConfiguration,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_INHERIT(CStoredItemCollection)
)

REGISTER_POLYMORPHIC_CLASS(CConfiguration);

CConfiguration::CConfiguration()
    : CStoredNameItem("<new configuration>")
    , CStoredDescriptionItem("<no configuration description>")
    , CStoredItemCollection(EConfig)
{

}

IProjTreeItem* CConfiguration::Create()
{
    return new CConfiguration();
}

CConfiguration::~CConfiguration()
{

}

ETreeItemType CConfiguration::GetType() const
{
    return EConfig;
}