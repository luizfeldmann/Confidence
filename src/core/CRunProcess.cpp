#include "core/CRunProcess.h"

DEFINE_SERIALIZATION_SCHEME(CRunProcess,
    SERIALIZATION_INHERIT(CStoredNameItem)
    SERIALIZATION_INHERIT(CStoredDescriptionItem)
    SERIALIZATION_MEMBER(m_bRunAsAdmin)
    SERIALIZATION_MEMBER_AS(m_eStartMode, int)
    SERIALIZATION_MEMBER(m_pPolicy)
    SERIALIZATION_MEMBER(m_vArgs)
)

REGISTER_POLYMORPHIC_CLASS(CRunProcess);

CRunProcess::CRunProcess()
    : CStoredNameItem("<new process>")
    , CStoredDescriptionItem("<no process description>")
    , m_bRunAsAdmin(false)
    , m_eStartMode(EProcessStartWindowMode::Normal)
{

}

CRunProcess::~CRunProcess()
{

}

IProjTreeItem* CRunProcess::Create()
{
    return new CRunProcess;
}

ETreeItemType CRunProcess::GetType() const
{
    return ERunProcess;
}

CRunProcess::vec_args_t& CRunProcess::GetArguments()
{
    return m_vArgs;
}

const CRunProcess::vec_args_t& CRunProcess::GetArguments() const
{
    return m_vArgs;
}

IProcessPolicy* CRunProcess::GetPolicy() const
{
    return m_pPolicy.get();
}

void CRunProcess::SetPolicy(IProcessPolicy* pNewPolicy)
{
    m_pPolicy.reset(pNewPolicy);
}