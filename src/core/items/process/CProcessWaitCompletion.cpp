#include "core/items/process/CProcessWaitCompletion.h"
#include "docs/IDocExporter.h"
#include "util/StartProcess.h"
#include "util/Log.h"

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

//! @copydoc IDocumentable::Document
bool CProcessWaitCompletion::Document(IDocExporter& rExporter) const
{
    bool bStatus = rExporter.FormField("Wait for completion:", "The project will continue execution only after this process returns.");

    if (bStatus && m_bCheckReturnCode)
    {
        bStatus = rExporter.FormField("Expected return code:", std::to_string(m_nExpectedReturnCode));
    }

    return bStatus;
}

bool CProcessWaitCompletion::Execute(CExecutionContext& rContext, const SProcessStartInfo& rInfo) const
{
    void* hProcess = nullptr;
    bool bStatus = StartProcess(rInfo, &hProcess);

    if (bStatus)
    {
        if (!m_bCheckReturnCode)
            bStatus = WaitProcess(hProcess, nullptr);
        else
        {
            unsigned long ulExitCode = 0;

            bStatus = WaitProcess(hProcess, &ulExitCode);

            if (bStatus)
            {
                bStatus = (ulExitCode == m_nExpectedReturnCode);

                if (!bStatus)
                    CERROR("Process exited with code 0x%X but expected code was 0x%X", ulExitCode, m_nExpectedReturnCode);
            }
        }
    }

    return bStatus;
}