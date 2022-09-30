#include "ui/Editors/CGroupEditor.h"

CGroupEditorUI::CGroupEditorUI(wxWindow* pParent)
    : IGroupEditor(pParent)
{

}

CGroupEditor::CGroupEditor(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
    : CNameDescEditor(rNotebook, rItem)
    , m_pUiGroup(new CGroupEditorUI(m_pPanel))
{
    m_pSizer->Add(m_pUiGroup, 0, wxEXPAND, 5);
    m_pPanel->Layout();
}

ITreeItemEditor* CGroupEditor::Create(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
{
    return new CGroupEditor(rNotebook, rItem);
}