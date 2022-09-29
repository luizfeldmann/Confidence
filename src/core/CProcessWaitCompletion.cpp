#include "core/CProcessWaitCompletion.h"

DEFINE_SERIALIZATION_SCHEME(CProcessWaitCompletion,
    SERIALIZATION_INHERIT(IProcessPolicy)
);

REGISTER_POLYMORPHIC_CLASS(CProcessWaitCompletion);

CProcessWaitCompletion::CProcessWaitCompletion()
{

}

EProcessPolicy CProcessWaitCompletion::GetType() const
{
    return EProcessPolicy::EProcessWaitCompletion;
}