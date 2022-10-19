#include "core/CProcessFireForget.h"

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