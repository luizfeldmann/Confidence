#include "core/CProcessArgument.h"

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