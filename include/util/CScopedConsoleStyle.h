#ifndef _CSCOPEDCONSOLESTYLE_H_
#define _CSCOPEDCONSOLESTYLE_H_

#include <Windows.h>
#include <WinCon.h>

enum {
    FOREGROUND_DEFAULT = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,
};

//! @brief Modifies the style of the console during the instance's lifetime
class CScopedConsoleStyle
{
private:
    CScopedConsoleStyle(const CScopedConsoleStyle&) = delete;
    CScopedConsoleStyle& operator=(const CScopedConsoleStyle&) = delete;

protected:
    //! Handle to the current console
    const HANDLE m_hConsole;

    //! Holds the console's style before the construction
    CONSOLE_SCREEN_BUFFER_INFO m_initialState;

public:
    //! @brief Saves the current style and assigns the new one
    //! @param wAttributes The new style to use during this instance's lifetime
    CScopedConsoleStyle(WORD wAttributes);

    //! @brief On destruction, the original console style is reset
    ~CScopedConsoleStyle();
};

#endif