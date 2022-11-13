#include "core/items/process/CProcessArgument.h"
#include "docs/IDocExporter.h"

DEFINE_SERIALIZATION_SCHEME(CProcessArgument,
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_INHERIT(CStoredExpression)
)

CProcessArgument::CProcessArgument()
    : CStoredDescriptionItem("<no argument description>")
{

}

CProcessArgument::~CProcessArgument()
{

}

bool CProcessArgument::Document(IDocExporter& rExporter) const
{
    bool bStatus = rExporter.FormField("ARGUMENT:", GetDescription())
        && rExporter.Paragraph()
        && rExporter.Code(GetExpression())
        && rExporter.PopStack();

    return bStatus;
}