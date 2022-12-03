/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "core/items/CConfiguration.h"

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

CConfiguration::CConfiguration(const std::string& strName, const std::string& strDesc)
    : CStoredNameItem(strName)
    , CStoredDescriptionItem(strDesc)
    , CStoredItemCollection(EConfig)
{

}

IProjTreeItem::ptr_t CConfiguration::Create()
{
    return ptr_t(new CConfiguration);
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

bool CConfiguration::Execute(CExecutionContext&) const
{
    return true; // Do nothing
}