#include "core/items/CConstant.h"
#include "core/items/variable/CEnvironmentVariable.h"

DEFINE_SERIALIZATION_SCHEME(CConstant,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_INHERIT(CAssignable)
    SERIALIZATION_INHERIT(CStoredExpression)
)

REGISTER_POLYMORPHIC_CLASS(CConstant);

CConstant::CConstant()
    : CStoredNameItem("<new constant>")
    , CStoredDescriptionItem("<no constant description>")
{

}

IProjTreeItem::ptr_t CConstant::Create()
{
    return ptr_t(new CConstant);
}

CConstant::~CConstant()
{

}

ETreeItemType CConstant::GetType() const
{
    return EConstant;
}

bool CConstant::Execute(CExecutionContext& rContext) const
{
    LogExecution();

    const std::string strName = GetName();
    std::string strValue = GetExpression();

    bool bStatus = Assign(rContext, strName, strValue);
    return bStatus;
}

bool CConstant::DocumentName(IDocExporter& rExporter) const
{
    return IProjTreeItem::DocumentName(rExporter, "CONSTANT:");
}

bool CConstant::DocumentCustom(IDocExporter& rExporter) const
{
    bool bStatus = rExporter.FormField("Expression:", GetExpression(), true);

    return bStatus;
}