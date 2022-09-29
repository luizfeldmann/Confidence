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
    return EProcessPolicy::EProcessFireForget;
}