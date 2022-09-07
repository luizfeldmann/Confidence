#include "ui/CMainWindow.h"
#include "ui/CTreeBrowserModel.h"
#include <wx/filedlg.h>
#include "core/CProject.h"

const static wxString g_szProjectFileFilter = "Confidence projects (*.cfx)|*.cfx";

CMainWindow::CMainWindow() :
    IMainWindow(nullptr)
{
    SetIcon(wxIcon("RES_ID_ICON_APPLICATION"));

    CTreeBrowserModel* pTreeModel = new CTreeBrowserModel;
    m_dataViewCtrlBrowser->AssociateModel(pTreeModel);
    pTreeModel->DecRef();
    m_dataViewCtrlBrowser->ExpandChildren(m_dataViewCtrlBrowser->GetTopItem());

    ReloadProject();
}

CMainWindow::~CMainWindow()
{

}

/// =======================================================
/// Top toolbar
/// =======================================================

void CMainWindow::onBtnNewProject(wxCommandEvent& event)
{
    CProject::TheProject() = std::move( CProject() );
    ReloadProject();
}

void CMainWindow::onBtnOpenProject(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString,
            g_szProjectFileFilter, wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (wxID_CANCEL == openFileDialog.ShowModal())
        return;

    CProject openProj;
    if (openProj.OpenFile(openFileDialog.GetPath().ToStdString()))
    {
        CProject::TheProject() = std::move( openProj );
        ReloadProject();
    }
}

void CMainWindow::onBtnSaveProject(wxCommandEvent& event)
{
    const std::string& szCurrentPath = CProject::TheProject().GetCurrentPath();
    if (szCurrentPath.empty())
    {
        onBtnSaveAsProject(event);
    }
    else
    {
        CProject::TheProject().SaveToFile(szCurrentPath);
    }
}

void CMainWindow::onBtnSaveAsProject(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString,
        g_szProjectFileFilter, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (wxID_CANCEL == saveFileDialog.ShowModal())
        return;

    CProject::TheProject().SaveToFile(saveFileDialog.GetPath().ToStdString());
}

void CMainWindow::onBtnRunProject(wxCommandEvent& event)
{

}

void CMainWindow::onBtnStopProject(wxCommandEvent& event)
{

}

/// =======================================================
/// Tree browser
/// =======================================================

void CMainWindow::onBtnNewItem(wxCommandEvent& event)
{

}

void CMainWindow::onBtnDeleteItem(wxCommandEvent& event)
{

}

void CMainWindow::onBtnItemUp(wxCommandEvent& event)
{

}

void CMainWindow::onBtnItemDown(wxCommandEvent& event)
{

}

/// =======================================================
/// General
/// =======================================================

void CMainWindow::ReloadProject()
{

}