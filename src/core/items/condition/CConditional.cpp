#include "core/items/condition/CConditional.h"
#include "core/items/condition/ICondition.h"
#include "util/Log.h"

DEFINE_SERIALIZATION_SCHEME(CConditional,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_INHERIT(CStoredItemCollection)
)

REGISTER_POLYMORPHIC_CLASS(CConditional);

CConditional::CConditional()
    : CStoredNameItem("<new conditional>")
    , CStoredDescriptionItem("<no conditional description>")
    , CStoredItemCollection(EGroupable)
{

}

IProjTreeItem::ptr_t CConditional::Create()
{
    return ptr_t(new CConditional());
}

CConditional::~CConditional()
{

}

ETreeItemType CConditional::GetType() const
{
    return EConditional;
}

bool CConditional::Execute(CExecutionContext& rContext) const
{
    bool bResult = false;
    bool bStatus = false;

    if (!m_pCondition)
        CERROR("Conditional '%s' is undefined", GetName().c_str());
    else
    {
        bStatus = m_pCondition->Evaluate(rContext, bResult);

        if (bStatus && bResult)
            bStatus = ExecuteChildren(rContext);
    }

    return bStatus;
}

bool CConditional::DocumentName(IDocExporter& rExporter) const
{
    return IProjTreeItem::DocumentName(rExporter, "CONDITIONAL:");
}

bool CConditional::DocumentCustom(IDocExporter& rExporter) const
{
    bool bStatus = true;

    if (m_pCondition)
        bStatus = m_pCondition->Document(rExporter);

    return bStatus;
}