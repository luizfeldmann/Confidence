#include "ui/Editors/CVariableEditor.h"
#include "ui/CMainApp.h"
#include <array>

CVariableEditorUI::CVariableEditorUI(wxWindow* pParent, std::shared_ptr<const CProject> pProject, CVariable& rVar)
    : IVariableEditor(pParent)
    , m_rVar(rVar)
    , m_pModel(new CVariableTableModel(rVar, pProject, m_dataView))
{
    m_dataView->AssociateModel(m_pModel);
    m_pModel->DecRef();

    using acc_entry_t = CAcceleratorEntry;
    const std::array<acc_entry_t, 1> arrAccEntry{
        acc_entry_t(wxACCEL_NORMAL,  WXK_DELETE, m_toolDeleteCell),
    };

    m_cAccTbl = wxAcceleratorTable(arrAccEntry.size(), arrAccEntry.data());
    m_dataView->SetAcceleratorTable(m_cAccTbl);

    m_toolBar->ToggleTool(m_toolExportEnv->GetId(), m_rVar.m_bExportToEnvironment);
}

void CVariableEditorUI::OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent)
{
    m_pModel->OnItemCreated(rItem, rParent);
}

void CVariableEditorUI::OnAnyItemErased(const IProjTreeItem& rItem)
{
    m_pModel->OnAnyItemErased(rItem);
}

void CVariableEditorUI::OnAnyItemRenamed(const IProjTreeItem& rItem)
{
    m_pModel->OnAnyItemRenamed(rItem);
}

void CVariableEditorUI::onDeleteCell(wxCommandEvent& event)
{
    wxDataViewItem 	selectRow = m_dataView->GetCurrentItem();
    wxDataViewColumn* selectColumn = m_dataView->GetCurrentColumn();

    m_pModel->DeleteCell(selectRow, selectColumn);
}

void CVariableEditorUI::onToolExportEnv(wxCommandEvent& event)
{
    m_rVar.m_bExportToEnvironment = m_toolExportEnv->IsToggled();
}

CVariableEditor::CVariableEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem)
    : CNameDescEditor(rMainWindow, rItem)
    , m_pUiVar(new CVariableEditorUI(m_pPanel, GetProject(), dynamic_cast<CVariable&>(rItem)))
{
    m_pSizer->Add(m_pUiVar, 1, wxEXPAND, 5);
    m_pPanel->Layout();
}

ITreeItemEditor* CVariableEditor::Create(CMainWindow& rMainWindow, IProjTreeItem& rItem)
{
    return new CVariableEditor(rMainWindow, rItem);
}

void CVariableEditor::OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent)
{
    CNameDescEditor::OnItemCreated(rItem, rParent);
    m_pUiVar->OnItemCreated(rItem, rParent);
}

void CVariableEditor::OnAnyItemErased(const IProjTreeItem& rItem)
{
    CNameDescEditor::OnAnyItemErased(rItem);
    m_pUiVar->OnAnyItemErased(rItem);
}

void CVariableEditor::OnAnyItemRenamed(const IProjTreeItem& rChanged)
{
    CNameDescEditor::OnAnyItemRenamed(rChanged);
    m_pUiVar->OnAnyItemRenamed(rChanged);
}

