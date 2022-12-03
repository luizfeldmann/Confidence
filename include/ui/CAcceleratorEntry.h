/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CACCELERATORENTRY_H_
#define _CACCELERATORENTRY_H_

#include <wx/accel.h>
#include <wx/tbarbase.h>

//! @brief Associates a hotkey to a toolbar button
class CAcceleratorEntry : public wxAcceleratorEntry
{
public:
    //! @brief Constructs an accelerator entry (shortcut)
    //! @param[in] nFlags Values from #wxAcceleratorEntryFlags to specify the use of CTRL, SHIFT, etc...
    //! @param[in] nKeyCode ASCII or #wxKeyCode to specify the keyboard key for the shortcut
    //! @param[in] pTool Pointer to the tool bar button to be activated when hitting the hotkey
    CAcceleratorEntry(int nFlags, int nKeyCode, wxToolBarToolBase* pTool);
};

#endif