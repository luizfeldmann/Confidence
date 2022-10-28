#include "core/items/file_operation/CFileOperations.h"

DEFINE_SERIALIZATION_SCHEME(CFileOperations,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
)

REGISTER_POLYMORPHIC_CLASS(CFileOperations);

CFileOperations::CFileOperations()
    : CStoredNameItem("<new file operation>")
    , CStoredDescriptionItem("<no operation description>")
{

}

IProjTreeItem::ptr_t CFileOperations::Create()
{
    return ptr_t(new CFileOperations);
}

CFileOperations::~CFileOperations()
{

}

ETreeItemType CFileOperations::GetType() const
{
    return EOperations;
}

bool CFileOperations::Execute(CExecutionContext&) const
{
    return true; // TODO
}

bool CFileOperations::DocumentName(IDocExporter& rExporter) const
{
    return IProjTreeItem::DocumentName(rExporter, "FILE OPERATION:");
}