#include "ui/CNameDescEditor.h"

wxDEFINE_EVENT(EVT_EDITOR_ITEM_CHANGED_NAME, wxCommandEvent);
wxDEFINE_EVENT(EVT_EDITOR_ITEM_CHANGED_DESC, wxCommandEvent);

CNameDescEditorUI::CNameDescEditorUI(wxWindow* pParent, CNameDescEditor& rEditor)
    : INameDescEditor(pParent)
    , m_rEditor(rEditor)
{
    ItemChanged();
}

void CNameDescEditorUI::ItemChanged()
{
    IProjTreeItem& rItem = m_rEditor.GetItem();

    m_textCtrlName->ChangeValue(rItem.GetName());
    m_textCtrlDesc->ChangeValue(rItem.GetDescription());
}

void CNameDescEditorUI::onChangedName(wxCommandEvent& event)
{
    const wxString newName = m_textCtrlName->GetValue();

    IProjTreeItem& rItem = m_rEditor.GetItem();
    rItem.SetName(newName.ToStdString());

    // Make sure the tab name is updated as well
    m_rEditor.ITreeItemEditor::ItemChanged();

    wxCommandEvent cmdEvent(EVT_EDITOR_ITEM_CHANGED_NAME);
    cmdEvent.SetClientData(&rItem);
    m_rEditor.GetNotebook().GetEventHandler()->ProcessEvent(cmdEvent);
}

void CNameDescEditorUI::onChangedDesc(wxCommandEvent& event)
{
    const wxString newDesc = m_textCtrlDesc->GetValue();

    IProjTreeItem& rItem = m_rEditor.GetItem();
    rItem.SetDescription(newDesc.ToStdString());

    wxCommandEvent cmdEvent(EVT_EDITOR_ITEM_CHANGED_DESC);
    cmdEvent.SetClientData(&rItem);
    m_rEditor.GetNotebook().GetEventHandler()->ProcessEvent(cmdEvent);
}

CNameDescEditor::CNameDescEditor(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
    : ITreeItemEditor(rNotebook, rItem)
    , m_pUiNameDesc( new CNameDescEditorUI(m_pPanel, *this) )
{
    m_pSizer->Add(m_pUiNameDesc, 0, wxEXPAND, 5);
    m_pPanel->Layout();
}

ITreeItemEditor* CNameDescEditor::Create(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
{
    return new CNameDescEditor(rNotebook, rItem);
}

void CNameDescEditor::ItemChanged()
{
    ITreeItemEditor::ItemChanged();
    m_pUiNameDesc->ItemChanged();
}