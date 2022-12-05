/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "core/items/CConstant.h"
#include "core/items/variable/CEnvironmentVariable.h"
#include "core/CEvaluationContext.h"
#include "docs/IDocExporter.h"

DEFINE_SERIALIZATION_SCHEME(CConstant,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_INHERIT(CAssignable)
    SERIALIZATION_INHERIT(CStoredExpression)
)

REGISTER_POLYMORPHIC_CLASS(CConstant);

CConstant::CConstant()
    : CStoredNameItem("Constant")
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

bool CConstant::SetName(const std::string& strNewName)
{
    bool bStatus = CEvaluationContext::ValidateVariableName(strNewName);
    bStatus = bStatus && CStoredNameItem::SetName(strNewName);

    return bStatus;
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

    if (bStatus)
    {
        const std::string strName = GetName();
        const auto vDependVariables = GetDependencies();

        for (const std::string& strDepVar : vDependVariables)
            rExporter.Dependency(strDepVar, strName);
    }

    return bStatus;
}