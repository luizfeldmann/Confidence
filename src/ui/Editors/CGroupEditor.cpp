#include "ui/Editors/CGroupEditor.h"

CGroupEditorUI::CGroupEditorUI(wxWindow* pParent)
    : IGroupEditor(pParent)
{

}

CGroupEditor::CGroupEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem)
    : CNameDescEditor(rMainWindow, rItem)
    , m_pUiGroup(new CGroupEditorUI(m_pPanel))
{
    m_pSizer->Add(m_pUiGroup, 0, wxEXPAND, 5);
    m_pPanel->Layout();
}

ITreeItemEditor* CGroupEditor::Create(CMainWindow& rMainWindow, IProjTreeItem& rItem)
{
    return new CGroupEditor(rMainWindow, rItem);
}