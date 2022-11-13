#include "core/items/process/CProcessFireForget.h"
#include "docs/IDocExporter.h"
#include "util/StartProcess.h"

DEFINE_SERIALIZATION_SCHEME(CProcessFireAndForget,
    SERIALIZATION_INHERIT(IProcessPolicy)
);

REGISTER_POLYMORPHIC_CLASS(CProcessFireAndForget);

CProcessFireAndForget::CProcessFireAndForget()
{

}

EProcessPolicy CProcessFireAndForget::GetType() const
{
    return EProcessPolicy::FireForget;
}

bool CProcessFireAndForget::Document(IDocExporter& rExporter) const
{
    bool bStatus = rExporter.FormField("Fire and forget:", "The started process will not block the project execution.");

    return bStatus;
}

bool CProcessFireAndForget::Execute(CExecutionContext& rContext, const SProcessStartInfo& rInfo) const
{
    bool bStatus = StartProcess(rInfo, nullptr);

    return bStatus;
}