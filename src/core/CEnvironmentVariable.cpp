#include "core/CEnvironmentVariable.h"
#include "util/Log.h"

static bool _os_checkenv(const char* szName);
static bool _os_unsetenv(const char* szName);
static bool _os_setenv(const char* szName, const char* szValue);

#ifdef _WIN32
#include <Windows.h>

static bool _os_checkenv(const char* szName)
{
    DWORD dwRequiredBuffer = GetEnvironmentVariableA(szName, nullptr, 0);
    return (dwRequiredBuffer > 0);
}

static bool _os_setenv(const char* szName, const char* szValue)
{
    bool bStatus = SetEnvironmentVariable(szName, szValue);
    if (!bStatus)
        CERROR("Failed to set environment variable '%s' to '%s': %d", szName, szValue, GetLastError());

    return bStatus;
}

static bool _os_unsetenv(const char* szName)
{
    bool bStatus = SetEnvironmentVariable(szName, nullptr);
    if (!bStatus)
        CERROR("Failed to remove environment variable '%s': %d", szName, GetLastError());

    return bStatus;
}

#else
    #include <cstdlib>

static bool _os_checkenv(const char* szName)
{
    return (nullptr != getenv(szName));
}

static bool _os_setenv(const char* szName, const char* szValue)
{
    bool bStatus = (0 == setenv(szName, szValue, true));

    if (!bStatus)
        CERROR("Failed to set environment variable '%s' to '%s': %d - %s", szName, szValue, errno, strerror(errno));

    return bStatus;
}

static bool _os_unsetenv(const char* szName)
{
    bool bStatus = (0 == unsetenv(szName));

    if (!bStatus)
        CERROR("Failed to erase environment variable '%s': %d - %s", szName, errno, strerror(errno));

    return bStatus;
}

#endif

CEnvironmentVariable::CEnvironmentVariable(const std::string& strName)
    : m_bSet(false)
    , m_strName(strName)
{

}

CEnvironmentVariable::~CEnvironmentVariable()
{
    if (m_bSet)
        Unset();
}

bool CEnvironmentVariable::Set(const char* szValue)
{
    const char* const szName = m_strName.c_str();

    // Notify the user if overwritting
    if (_os_checkenv(szName))
        CINFO("Environment variable '%s' already exists and will be overwritten", szName);

    // Set in environment
    bool bStatus = _os_setenv(szName, szValue);

    if (bStatus)
        m_bSet = true;

    return bStatus;
}

bool CEnvironmentVariable::Unset()
{
    bool bStatus = _os_unsetenv(m_strName.c_str());
    return bStatus;
}