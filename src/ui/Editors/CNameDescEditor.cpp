#include "ui/Editors/CNameDescEditor.h"
#include "wxExport/INameDescEditor.h"
#include "core/IProjTreeItem.h"

wxDEFINE_EVENT(EVT_EDITOR_ITEM_CHANGED_NAME, wxCommandEvent);
wxDEFINE_EVENT(EVT_EDITOR_ITEM_CHANGED_DESC, wxCommandEvent);

/* CNameDescEditorUI */

//! @brief Item editor containing text boxes to edit the name and the description 
class CNameDescEditorUI : public INameDescEditor, public INotifyItemOperation
{
protected:
    //! Reference to the item being edited
    CNameDescEditor& m_rEditor;

    /** @name INameDescEditor overrides */
    //!@{
    
    //! @brief Updates the name saved in the item, updates the label in the editor tab and notifies all the observers
    void onChangedName(wxCommandEvent& event) override;
    
    //! @brief Updates the description in the item, notifies all the observers
    void onChangedDesc(wxCommandEvent& event) override;
    //!@}

public:
    //! @brief Constructs a new editor UI associated with the specified #IProjTreeItem
    CNameDescEditorUI(wxWindow* pParent, CNameDescEditor& rEditor);

    //! @copydoc INotifyItemOperation::OnAnyItemRenamed
    void OnAnyItemRenamed(const IProjTreeItem& rChanged) override;
};

CNameDescEditorUI::CNameDescEditorUI(wxWindow* pParent, CNameDescEditor& rEditor)
    : INameDescEditor(pParent)
    , m_rEditor(rEditor)
{
    OnAnyItemRenamed(m_rEditor.GetItem());
}

void CNameDescEditorUI::OnAnyItemRenamed(const IProjTreeItem& rChanged)
{
    IProjTreeItem& rItem = m_rEditor.GetItem();

    if (&rChanged == &rItem)
    {
        m_textCtrlName->ChangeValue(rItem.GetName());
        m_textCtrlDesc->ChangeValue(rItem.GetDescription());
    }
}

void CNameDescEditorUI::onChangedName(wxCommandEvent& event)
{
    const wxString newName = m_textCtrlName->GetValue();

    IProjTreeItem& rItem = m_rEditor.GetItem();
    bool bSuccess = rItem.SetName(newName.ToStdString());

    if (bSuccess)
    {
        // Make sure the tab name is updated as well
        m_rEditor.ITreeItemEditor::OnAnyItemRenamed(rItem);

        // Notify the main window about the name change
        wxCommandEvent cmdEvent(EVT_EDITOR_ITEM_CHANGED_NAME);
        cmdEvent.SetClientData(&rItem);
        m_rEditor.GetNotebook().GetEventHandler()->ProcessEvent(cmdEvent);
    }
    else
    {
        // Reset to the previous, valid name
        m_textCtrlName->ChangeValue(rItem.GetName());
    }
}

void CNameDescEditorUI::onChangedDesc(wxCommandEvent& event)
{
    const wxString newDesc = m_textCtrlDesc->GetValue();

    IProjTreeItem& rItem = m_rEditor.GetItem();
    bool bSuccess = rItem.SetDescription(newDesc.ToStdString());

    if (bSuccess)
    {
        // Notify the main window about the description change
        wxCommandEvent cmdEvent(EVT_EDITOR_ITEM_CHANGED_DESC);
        cmdEvent.SetClientData(&rItem);
        m_rEditor.GetNotebook().GetEventHandler()->ProcessEvent(cmdEvent);
    }
    else
    {
        // Reset to the previous, valid description
        m_textCtrlDesc->ChangeValue(rItem.GetDescription());
    }
}

/* CNameDescEditor */

CNameDescEditor::CNameDescEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem)
    : ITreeItemEditor(rMainWindow, rItem)
    , m_pUiNameDesc( new CNameDescEditorUI(m_pPanel, *this) )
{
    m_pSizer->Add(m_pUiNameDesc, 0, wxEXPAND, 5);
    m_pPanel->Layout();
}

/* static */ std::unique_ptr<ITreeItemEditor> CNameDescEditor::Create(CMainWindow& rMainWindow, IProjTreeItem& rItem)
{
    return std::make_unique<CNameDescEditor>(rMainWindow, rItem);
}

void CNameDescEditor::OnAnyItemRenamed(const IProjTreeItem& rChanged)
{
    ITreeItemEditor::OnAnyItemRenamed(rChanged);
    m_pUiNameDesc->OnAnyItemRenamed(rChanged);
}