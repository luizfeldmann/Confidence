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
    CMainWindow* m_pMainWindow;

public:
    CMainApp();
    virtual bool OnInit();
};

wxDECLARE_APP(CMainApp);

#endif