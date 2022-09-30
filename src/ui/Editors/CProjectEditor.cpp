#include "ui/Editors/CProjectEditor.h"

CProjectEditorUI::CProjectEditorUI(wxWindow* pParent)
    : IProjectEditor(pParent)
{

}

CProjectEditor::CProjectEditor(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
    : CNameDescEditor(rNotebook, rItem)
    , m_pUiProj(new CProjectEditorUI(m_pPanel))
{
    m_pSizer->Add(m_pUiProj, 0, wxEXPAND, 5);
    m_pPanel->Layout();
}

ITreeItemEditor* CProjectEditor::Create(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
{
    return new CProjectEditor(rNotebook, rItem);
}