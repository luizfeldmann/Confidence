#include "core/CInstance.h"

DEFINE_SERIALIZATION_SCHEME(CInstance,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
)

REGISTER_POLYMORPHIC_CLASS(CInstance);

CInstance::CInstance()
    : CStoredNameItem("<new instance>")
    , CStoredDescriptionItem("<no instance description>")
{

}

IProjTreeItem* CInstance::Create()
{
    return new CInstance();
}

CInstance::~CInstance()
{

}

ETreeItemType CInstance::GetType() const
{
    return EInstance;
}