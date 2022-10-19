#include "core/CConfiguration.h"

DEFINE_SERIALIZATION_SCHEME(CConfiguration,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_INHERIT(CStoredItemCollection)
)

REGISTER_POLYMORPHIC_CLASS(CConfiguration);

CConfiguration::ITracked::set_type CConfiguration::ITracked::m_trackSet;

CConfiguration::CConfiguration()
    : CStoredNameItem("<new configuration>")
    , CStoredDescriptionItem("<no configuration description>")
    , CStoredItemCollection(EConfig)
{

}

CConfiguration::CConfiguration(const std::string& strName, const std::string& strDesc)
    : CStoredNameItem(strName)
    , CStoredDescriptionItem(strDesc)
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

bool CConfiguration::DocumentName(IDocExporter& rExporter) const
{
    return IProjTreeItem::DocumentName(rExporter, "CONFIG:");
}