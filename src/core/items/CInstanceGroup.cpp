/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "core/items/CInstanceGroup.h"

DEFINE_SERIALIZATION_SCHEME(CInstanceGroup,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_INHERIT(CStoredItemCollection)
)

REGISTER_POLYMORPHIC_CLASS(CInstanceGroup);

CInstanceGroup::CInstanceGroup()
    : CStoredNameItem("Instances")
    , CStoredDescriptionItem("List of all of the project's instances")
    , CStoredItemCollection(EInstance)
{

}

CInstanceGroup::~CInstanceGroup()
{

}

/* static */ std::shared_ptr<CInstanceGroup> CInstanceGroup::Create()
{
    return std::shared_ptr<CInstanceGroup>(new CInstanceGroup);
}

ETreeItemType CInstanceGroup::GetType() const
{
    return EInstGroup;
}

bool CInstanceGroup::Execute(CExecutionContext&) const
{
    return true; // Do nothing
}