/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CMAINAPP_H_
#define _CMAINAPP_H_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class CMainWindow;

//! @brief Represents the current GUI application
//! @internal
class CMainApp : public wxApp
{
protected:
    //! Pointer to the main application window
    CMainWindow* m_pMainWindow;

public:
    CMainApp();

    //! @brief Loads the project pointed by m_strOpenProjectFileName, creates and shows the m_pMainWindow
    virtual bool OnInit();

    //! The path to the project to be opened when the editor launches
    static std::string m_strOpenProjectFileName;
};

wxDECLARE_APP(CMainApp);

#endif