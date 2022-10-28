#include "core/items/CGroup.h"

DEFINE_SERIALIZATION_SCHEME(CGroup,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_INHERIT(CStoredItemCollection)
)

REGISTER_POLYMORPHIC_CLASS(CGroup);

CGroup::CGroup()
    : CStoredNameItem("<new group>")
    , CStoredDescriptionItem("<no group description>")
    , CStoredItemCollection(ETreeItemType(EGroup | EVariable | ETextFile | ERunProcess | EImportEnv | EOperations))
{

}

IProjTreeItem::ptr_t CGroup::Create()
{
    return ptr_t(new CGroup());
}

CGroup::~CGroup()
{

}

ETreeItemType CGroup::GetType() const
{
    return EGroup;
}

bool CGroup::Execute(CExecutionContext& rContext) const
{
    bool bStatus = true;

    bool bCondition = true; // TODO: Check we must execute this group

    if (bCondition)
    {
        LogExecution();
        bStatus = ExecuteChildren(rContext);
    }

    return bStatus;
}

bool CGroup::DocumentName(IDocExporter& rExporter) const
{
    return IProjTreeItem::DocumentName(rExporter, "GROUP:");
}