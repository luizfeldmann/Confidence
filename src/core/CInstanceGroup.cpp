#include "core/CInstanceGroup.h"

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

ETreeItemType CInstanceGroup::GetType() const
{
    return EInstGroup;
}

bool CInstanceGroup::Execute(CExecutionContext&) const
{
    return true; // Do nothing
}