#include "core/CInstance.h"

DEFINE_SERIALIZATION_SCHEME(CInstance,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
)

REGISTER_POLYMORPHIC_CLASS(CInstance);

CInstance::ITracked::set_type CInstance::ITracked::m_trackSet;

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


bool CInstance::DocumentName(IDocExporter& rExporter) const
{
    return IProjTreeItem::DocumentName(rExporter, "INSTANCE:");
}