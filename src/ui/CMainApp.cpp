#include "ui/CMainApp.h"
#include "ui/CMainWindow.h"

wxIMPLEMENT_APP_NO_MAIN(CMainApp);

/*static*/ CProject CMainApp::m_cWorkingProject;

CMainApp::CMainApp()
    : m_pMainWindow(nullptr)
{

}

bool CMainApp::OnInit()
{
    m_pMainWindow = new CMainWindow(m_cWorkingProject);
    m_pMainWindow->Show();
    return true;
}