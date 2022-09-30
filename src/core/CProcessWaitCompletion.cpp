#include "core/CProcessWaitCompletion.h"

DEFINE_SERIALIZATION_SCHEME(CProcessWaitCompletion,
    SERIALIZATION_INHERIT(IProcessPolicy)
    SERIALIZATION_MEMBER(m_bCheckReturnCode)
    SERIALIZATION_MEMBER(m_nExpectedReturnCode)
);

REGISTER_POLYMORPHIC_CLASS(CProcessWaitCompletion);

CProcessWaitCompletion::CProcessWaitCompletion()
    : m_bCheckReturnCode(false)
    , m_nExpectedReturnCode(0)
{

}

EProcessPolicy CProcessWaitCompletion::GetType() const
{
    return EProcessPolicy::WaitCompletion;
}

void CProcessWaitCompletion::SetExpectedReturnCode(opt_returncode_t nRc)
{
    m_bCheckReturnCode = nRc.has_value();
    m_nExpectedReturnCode = nRc.value_or(0);
}

const CProcessWaitCompletion::opt_returncode_t CProcessWaitCompletion::GetExpectedReturnCode() const
{
    return (m_bCheckReturnCode) 
        ? opt_returncode_t(m_nExpectedReturnCode) 
        : opt_returncode_t();
}