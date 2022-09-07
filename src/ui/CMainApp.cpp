#include "ui/CMainApp.h"
#include "ui/CMainWindow.h"

wxIMPLEMENT_APP_NO_MAIN(CMainApp);

CMainApp::CMainApp()
    : m_pMainWindow(nullptr)
{

}

bool CMainApp::OnInit()
{
    m_pMainWindow = new CMainWindow();
    m_pMainWindow->Show();
    return true;
}