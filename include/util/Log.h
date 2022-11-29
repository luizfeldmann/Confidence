#ifndef _LOG_H_
#define _LOG_H_

//! @brief Utility class to group all log-related function
class CLog 
{
public:
    //! When true, warnings and error produce a GUI message box; otherwise, just it's just printed to the console.
    static bool m_bShowMessageBox;

    //! @brief Print a message to stdout with white coloring on the console.
    static void Log(const char* szFormat, ...);

    //! @brief Print a message to stdout with green coloring on the console.
    static void Info(const char* szFormat, ...);

    //! @brief Prints a message to stderr with orange coloring on the console.
    //!        If a GUI exists, shows a warning message box.
    static void Warning(const char* szFormat, ...);

    //! @brief Prints a message to stderr with red coloring on the console.
    //!        If GUI exists, shows an error message box.
    static void Error(const char* szFormat, ...);
};

#define LOG_ENDL "\n"

//#define LOG_TRACE !NDEBUG

#ifdef LOG_TRACE
    #define CLOG(szFormat, ...) CLog::Log("Log: " __FUNCTION__ ": " szFormat LOG_ENDL, __VA_ARGS__)
    #define CINFO(szFormat, ...) CLog::Info("Info: " __FUNCTION__ ": " szFormat  LOG_ENDL, __VA_ARGS__)
    #define CWARNING(szFormat, ...) CLog::Warning("Warning: " __FUNCTION__ ": " szFormat  LOG_ENDL, __VA_ARGS__)
    #define CERROR(szFormat, ...) CLog::Error("Error: " __FUNCTION__ ": " szFormat  LOG_ENDL, __VA_ARGS__)
#else
    #define CLOG(szFormat, ...) CLog::Log(szFormat  LOG_ENDL, __VA_ARGS__)
    #define CINFO(szFormat, ...) CLog::Info(szFormat  LOG_ENDL, __VA_ARGS__)
    #define CWARNING(szFormat, ...) CLog::Warning(szFormat LOG_ENDL, __VA_ARGS__)
    #define CERROR(szFormat, ...) CLog::Error(szFormat  LOG_ENDL, __VA_ARGS__)
#endif

#endif