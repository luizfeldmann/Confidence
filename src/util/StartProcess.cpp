#include "util/StartProcess.h"
#include "util/Log.h"
#include <sstream>
#include <Windows.h>

static std::string FormatError(DWORD dwError)
{
    char* szMessageBuffer = nullptr;

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&szMessageBuffer, 0, NULL);

    std::string strMessage(szMessageBuffer);
    LocalFree(szMessageBuffer);

    return strMessage;
}

bool StartProcess(const SProcessStartInfo& rInfo, void** ppProcHandle)
{
    SHELLEXECUTEINFO sei = {};
    sei.cbSize = sizeof(sei);

    // Choose flags
    sei.fMask = (nullptr != ppProcHandle) 
        ? SEE_MASK_NOCLOSEPROCESS
        : SEE_MASK_DEFAULT;

    // Choose the 'verb'
    if (rInfo.m_bRunAsAdmin)
        sei.lpVerb = "runas";

    // Choose the start mode
    switch (rInfo.m_eStartMode)
    {
    case EProcessStartWindowMode::Minimized:
        sei.nShow = SW_MINIMIZE;
        break;
    case EProcessStartWindowMode::Normal:
        sei.nShow = SW_NORMAL;
        break;
    case EProcessStartWindowMode::Maximized:
        sei.nShow = SW_MAXIMIZE;
        break;
    }

    // Prepare the arguments
    std::string strParameters;
    if (!rInfo.m_vArguments.empty())
    {
        sei.lpFile = rInfo.m_vArguments.front().c_str();

        std::stringstream ss;
        for (size_t i = 1; i < rInfo.m_vArguments.size(); i++)
        {
            if (i != 1)
                ss << " ";

            ss << rInfo.m_vArguments[i];
        }

        strParameters = ss.str();
    }

    sei.lpParameters = strParameters.c_str();

    // Start the process
    bool bStatus = ShellExecuteExA(&sei);

    if (!bStatus)
    {
        // Print detailed error message on failure
        DWORD dwError = GetLastError();
        std::string strError = FormatError(dwError);

        CERROR("Failed to start process: 0x%X - %s", dwError, strError.c_str());
    }
    else if (ppProcHandle)
    {
        *ppProcHandle = sei.hProcess;
    }

    return bStatus;
}

bool WaitProcess(void* pProcHandle, unsigned long* pnExitCode)
{
    bool bStatus = true;

    DWORD dwStatus = WaitForSingleObject((HANDLE)pProcHandle, INFINITE);
    if (dwStatus != WAIT_OBJECT_0)
    {
        CERROR("Failed to wait for process handle: 0x%X", dwStatus);
        bStatus = false;
    }

    // Read the return code when requested
    if (bStatus && (nullptr != pnExitCode))
    {
        bStatus = GetExitCodeProcess((HANDLE)pProcHandle, pnExitCode);
        if (!bStatus)
        {
            dwStatus = GetLastError();
            std::string strError = FormatError(dwStatus);
            
            CERROR("Failed to read process return code: 0x%X - %s", dwStatus, strError.c_str());
        }
    }

    CloseHandle(pProcHandle);

    return bStatus;
}