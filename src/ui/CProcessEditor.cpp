#include "ui/CProcessEditor.h"
#include "ui/Models/CProcessArgsModel.h"

/* CRunProcessEditorUI */
CRunProcessEditorUI::CRunProcessEditorUI(wxWindow* pParent, CRunProcess& cEdit)
    : IRunProcessEditor(pParent)
    , m_cEdit(cEdit)
    , m_pModel(new CProcessArgsModel(m_cEdit))
{
    // Create model
    m_dataViewCtrlArgs->AssociateModel(m_pModel);
    m_pModel->DecRef();

    // Draw all initial items
    wxDataViewItem wxRoot(nullptr);
    wxDataViewItemArray arrItems;

    m_pModel->GetChildren(wxRoot, arrItems);
    m_pModel->ItemsAdded(wxRoot, arrItems);
}

CProcessArgument* CRunProcessEditorUI::GetSelectedArgument() const
{
    return static_cast<CProcessArgument*>(m_dataViewCtrlArgs->GetSelection().GetID());
}

void CRunProcessEditorUI::onToolNewArg(wxCommandEvent& event)
{
    CProcessArgument* pNewItem = m_pModel->NewItem();
    m_dataViewCtrlArgs->Select(wxDataViewItem(pNewItem));
}

void CRunProcessEditorUI::onToolDelArg(wxCommandEvent& event)
{
    CProcessArgument* pSelected = GetSelectedArgument();

    if (nullptr != pSelected)
    {
        m_pModel->DeleteItem(pSelected);
    }
}

void CRunProcessEditorUI::onToolMoveUp(wxCommandEvent& event)
{
}

void CRunProcessEditorUI::onToolMoveDown(wxCommandEvent& event)
{
}

void CRunProcessEditorUI::onPolicyChanged(wxCommandEvent& event)
{
}

void CRunProcessEditorUI::onWindowChanged(wxCommandEvent& event)
{
}

void CRunProcessEditorUI::onAdminChanged(wxCommandEvent& event)
{
}

void CRunProcessEditorUI::onReturnCodeCombo(wxCommandEvent& event)
{
}

void CRunProcessEditorUI::onReturnCodeText(wxCommandEvent& event)
{
}

/* CRunProcessEditor */
CRunProcessEditor::CRunProcessEditor(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
    : CNameDescEditor(rNotebook, rItem)
    , m_pUiProc(new CRunProcessEditorUI(m_pPanel, dynamic_cast<CRunProcess&>(rItem)))
{
    m_pSizer->Add(m_pUiProc, 1, wxEXPAND, 5);
    m_pPanel->Layout();
}

ITreeItemEditor* CRunProcessEditor::Create(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
{
    return new CRunProcessEditor(rNotebook, rItem);
}