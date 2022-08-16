#include "util/Log.h"
#include "util/CScopedConsoleStyle.h"
#include <cstdio>

bool CLog::m_bShowMessageBox = false;

void CLog::Log(const char* szFormat, ...)
{
    CScopedConsoleStyle cStyle(FOREGROUND_DEFAULT);

    va_list args;

    va_start(args, szFormat);
    vfprintf(stdout, szFormat, args);
    va_end(args);
}
     
void CLog::Info(const char* szFormat, ...)
{
    CScopedConsoleStyle cStyle(FOREGROUND_GREEN);

    va_list args;

    va_start(args, szFormat);
    vfprintf(stdout, szFormat, args);
    va_end(args);
}

static void _MessageBox(const char* szTitle, UINT uType, const char* szFormat, va_list args)
{
    char szText[1024];
    vsnprintf(szText, sizeof(szText), szFormat, args);

    MessageBox(NULL, szText, szTitle, uType);
}
     
void CLog::Warning(const char* szFormat, ...)
{
    CScopedConsoleStyle cStyle(FOREGROUND_RED | FOREGROUND_GREEN);

    va_list args;

    va_start(args, szFormat);
    vfprintf(stderr, szFormat, args);
    va_end(args);

    if (m_bShowMessageBox)
    {
        va_start(args, szFormat);
        _MessageBox("Warning", MB_ICONWARNING, szFormat, args);
        va_end(args);
    }
}
     
void CLog::Error(const char* szFormat, ...)
{
    CScopedConsoleStyle cStyle(FOREGROUND_RED);

    va_list args;
    va_start(args, szFormat);
    vfprintf(stderr, szFormat, args);
    va_end(args);

    if (m_bShowMessageBox)
    {
        va_start(args, szFormat);
        _MessageBox("Error", MB_ICONERROR, szFormat, args);
        va_end(args);
    }
}