#include "ui/Editors/CVariableEditor.h"
#include "ui/Models/CVariableTableModel.h"
#include "ui/CAcceleratorEntry.h"
#include "wxExport/IVariableEditor.h"
#include "core/items/variable/CVariable.h"
#include <array>

/* CVariableEditorUI */

//! @brief Item editor for variable types
class CVariableEditorUI : public IVariableEditor, public INotifyItemOperation
{
protected:
    //! Reference to the variable being edited
    CVariable& m_rVar;

    //! Pointer to the model managing the data view table for the variable
    CVariableTableModel* m_pModel;

    //! Manages keyboard shortcuts
    wxAcceleratorTable m_cAccTbl;

public:
    //! @brief Constructs a new editor UI associated with the specified #CVariable
    //! @param[in] pParent Pointer to the window where this editor will be created
    //! @param[in] pProject Pointer to the current project which owns \p rVar
    //! @param[in] rVar @copybrief m_rVar
    CVariableEditorUI(wxWindow* pParent, std::shared_ptr<const CProject> pProject, CVariable& rVar);

    //! @copydoc INotifyItemOperation::OnItemCreated
    void OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent) override;

    //! @copydoc INotifyItemOperation::OnAnyItemErased
    void OnAnyItemErased(const IProjTreeItem& rItem) override;

    //! @copydoc INotifyItemOperation::OnAnyItemRenamed
    void OnAnyItemRenamed(const IProjTreeItem& pItem) override;

    /* OVERRIDES FROM IVariableEditor */
    /** @name IVariableEditor overrides */
    //!@{
    
    //! @brief Updates the stored configuration regarding the "per instance" mode of the variable
    void onToolPerInstance(wxCommandEvent& event) override;
    
    //! @brief Updates the stored configuration regarding the "export variable to environment" feature
    void onToolExportEnv(wxCommandEvent& event) override;
    
    //! @brief Deletes the rule associated to the selected cell in the table
    void onDeleteCell(wxCommandEvent& event) override;

    //!@}
};

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

    m_toolBar->ToggleTool(m_toolExportEnv->GetId(), m_rVar.GetExportToEnv());
    m_toolBar->ToggleTool(m_toolPerInstance->GetId(), m_rVar.GetPerInstance());
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

void CVariableEditorUI::onToolPerInstance(wxCommandEvent& event)
{
    m_rVar.SetPerInstance(m_toolPerInstance->IsToggled());
    m_pModel->ReloadColumns();
}

void CVariableEditorUI::onToolExportEnv(wxCommandEvent& event)
{
    m_rVar.SetExportToEnv(m_toolExportEnv->IsToggled());
}

/* CVariableEditor */

CVariableEditor::CVariableEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem)
    : CNameDescEditor(rMainWindow, rItem)
    , m_pUiVar(new CVariableEditorUI(m_pPanel, GetProject(), dynamic_cast<CVariable&>(rItem)))
{
    m_pSizer->Add(m_pUiVar, 1, wxEXPAND, 5);
    m_pPanel->Layout();
}

/* static */ std::unique_ptr<ITreeItemEditor> CVariableEditor::Create(CMainWindow& rMainWindow, IProjTreeItem& rItem)
{
    return std::make_unique<CVariableEditor>(rMainWindow, rItem);
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

