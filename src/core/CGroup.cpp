#include "core/CGroup.h"

DEFINE_SERIALIZATION_SCHEME(CGroup,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_INHERIT(CStoredItemCollection)
)

REGISTER_POLYMORPHIC_CLASS(CGroup);

CGroup::CGroup()
    : CStoredNameItem("<new group>")
    , CStoredDescriptionItem("<no group description>")
    , CStoredItemCollection(ETreeItemType(EGroup | EVariable | ETextFile | ERunProcess))
{

}

IProjTreeItem* CGroup::Create()
{
    return new CGroup();
}

CGroup::~CGroup()
{

}

ETreeItemType CGroup::GetType() const
{
    return EGroup;
}