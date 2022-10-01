#ifndef _CMAINAPP_H_
#define _CMAINAPP_H_

#include "core/CProject.h"

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

    //! Stores the current working project of the application
    static CProject m_cWorkingProject;
};

wxDECLARE_APP(CMainApp);

#endif