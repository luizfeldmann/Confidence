#include "core/CEnvironmentImport.h"
#include "core/CEnvironmentVariable.h"
#include "util/Log.h"

DEFINE_SERIALIZATION_SCHEME(CEnvironmentImport,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
)

REGISTER_POLYMORPHIC_CLASS(CEnvironmentImport);

CEnvironmentImport::CEnvironmentImport()
    : CStoredNameItem("<new env import>")
    , CStoredDescriptionItem("<no environment import description>")
{

}

IProjTreeItem::ptr_t CEnvironmentImport::Create()
{
    return ptr_t(new CEnvironmentImport());
}

CEnvironmentImport::~CEnvironmentImport()
{

}

ETreeItemType CEnvironmentImport::GetType() const
{
    return EImportEnv;
}

bool CEnvironmentImport::Execute(CExecutionContext& rContext) const
{
    LogExecution();

    const std::string strName = GetName();
    std::string strValue;
    bool bStatus = CEnvironmentVariable::Read(strName, strValue);
    if (!bStatus)
    {
        CERROR("Failed reading environment variable '%s'", strName.c_str());
    }
    else
    {
        rContext.SetVariableLiteral(strName, strValue);
    }

    return bStatus;
}

bool CEnvironmentImport::DocumentName(IDocExporter& rExporter) const
{
    return IProjTreeItem::DocumentName(rExporter, "IMPORT ENVIRONMENT:");
}