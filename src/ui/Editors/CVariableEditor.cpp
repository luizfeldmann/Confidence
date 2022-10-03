#include "ui/Editors/CVariableEditor.h"
#include "ui/CMainApp.h"
#include <array>

CVariableEditorUI::CVariableEditorUI(wxWindow* pParent, CVariable& rVar)
    : IVariableEditor(pParent)
    , m_rVar(rVar)
    , m_pModel(new CVariableTableModel(rVar, CMainApp::m_cWorkingProject, m_dataView))
{
    m_dataView->AssociateModel(m_pModel);
    m_pModel->DecRef();

    using acc_entry_t = CAcceleratorEntry;
    const std::array<acc_entry_t, 1> arrAccEntry{
        acc_entry_t(wxACCEL_NORMAL,  WXK_DELETE, m_toolDeleteCell),
    };

    m_cAccTbl = wxAcceleratorTable(arrAccEntry.size(), arrAccEntry.data());
    SetAcceleratorTable(m_cAccTbl);
}

void CVariableEditorUI::onDeleteCell(wxCommandEvent& event)
{
    wxDataViewItem 	selectRow = m_dataView->GetCurrentItem();
    wxDataViewColumn* selectColumn = m_dataView->GetCurrentColumn();

    m_pModel->DeleteCell(selectRow, selectColumn);
}

CVariableEditor::CVariableEditor(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
    : CNameDescEditor(rNotebook, rItem)
    , m_pUiVar(new CVariableEditorUI(m_pPanel, dynamic_cast<CVariable&>(rItem)))
{
    m_pSizer->Add(m_pUiVar, 1, wxEXPAND, 5);
    m_pPanel->Layout();
}

ITreeItemEditor* CVariableEditor::Create(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
{
    return new CVariableEditor(rNotebook, rItem);
}

