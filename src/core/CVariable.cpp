#include "core/CVariable.h"

DEFINE_SERIALIZATION_SCHEME(CVariable,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
)

REGISTER_POLYMORPHIC_CLASS(CVariable);

CVariable::CVariable()
    : CStoredNameItem("<new variable>")
    , CStoredDescriptionItem("<no variable description>")
{

}

CVariable::~CVariable()
{

}

ETreeItemType CVariable::GetType() const
{
    return EVariable;
}