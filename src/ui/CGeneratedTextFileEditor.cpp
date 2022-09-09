#include "ui/CGeneratedTextFileEditor.h"

CGeneratedTextFileEditorUI::CGeneratedTextFileEditorUI(wxWindow* pParent)
    : ITextFileEditor(pParent)
{

}

CGeneratedTextFileEditor::CGeneratedTextFileEditor(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
    : CNameDescEditor(rNotebook, rItem)
    , m_pUiTextFile(new CGeneratedTextFileEditorUI(m_pPanel))
{
    m_pSizer->Add(m_pUiTextFile, 0, wxEXPAND, 5);
    m_pPanel->Layout();
}

ITreeItemEditor* CGeneratedTextFileEditor::Create(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
{
    return new CGeneratedTextFileEditor(rNotebook, rItem);
}