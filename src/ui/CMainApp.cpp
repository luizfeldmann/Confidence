#include "ui/CMainApp.h"
#include "ui/CMainWindow.h"
#include "core/CProject.h"

wxIMPLEMENT_APP_NO_MAIN(CMainApp);

/*static*/ std::string CMainApp::m_strOpenProjectFileName;

CMainApp::CMainApp()
    : m_pMainWindow(nullptr)
{

}

bool CMainApp::OnInit()
{
    std::shared_ptr<CProject> pProject;

    if (m_strOpenProjectFileName.empty())
        pProject = CProject::Create();
    else
        pProject = CProject::Create(m_strOpenProjectFileName);

    bool bStatus = false;

    if (pProject)
    {
        bStatus = true;

        m_pMainWindow = new CMainWindow(pProject);
        m_pMainWindow->Show();
    }

    return bStatus;
}