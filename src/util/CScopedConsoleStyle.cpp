/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "util/CScopedConsoleStyle.h"

CScopedConsoleStyle::CScopedConsoleStyle(WORD wAttributes)
    : m_hConsole(GetStdHandle(STD_OUTPUT_HANDLE))
{
    if (GetConsoleScreenBufferInfo(m_hConsole, &m_initialState))
        SetConsoleTextAttribute(m_hConsole, wAttributes);
}

CScopedConsoleStyle::~CScopedConsoleStyle()
{
    SetConsoleTextAttribute(m_hConsole, m_initialState.wAttributes);
}