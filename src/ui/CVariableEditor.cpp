#include "ui/CVariableEditor.h"

CVariableEditorUI::CVariableEditorUI(wxWindow* pParent)
    : IVariableEditor(pParent)
{

}

CVariableEditor::CVariableEditor(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
    : CNameDescEditor(rNotebook, rItem)
    , m_pUiVar(new CVariableEditorUI(m_pPanel))
{
    m_pSizer->Add(m_pUiVar, 0, wxEXPAND, 5);
    m_pPanel->Layout();
}

ITreeItemEditor* CVariableEditor::Create(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
{
    return new CVariableEditor(rNotebook, rItem);
}