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

IProjTreeItem::ptr_t CInstance::Create()
{
    return ptr_t(new CInstance);
}

CInstance::~CInstance()
{

}

ETreeItemType CInstance::GetType() const
{
    return EInstance;
}

bool CInstance::Execute(CExecutionContext&) const
{
    return true; // Do nothing
}

bool CInstance::DocumentName(IDocExporter& rExporter) const
{
    return IProjTreeItem::DocumentName(rExporter, "INSTANCE:");
}