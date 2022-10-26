#ifndef _CMAINAPP_H_
#define _CMAINAPP_H_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class CMainWindow;

class CMainApp : public wxApp
{
protected:
    //! Pointer to the main application window
    CMainWindow* m_pMainWindow;

public:
    CMainApp();
    virtual bool OnInit();

    //! The path to the project to be opened when the editor launches
    static std::string m_strOpenProjectFileName;
};

wxDECLARE_APP(CMainApp);

#endif