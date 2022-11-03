#include "ui/Editors/CConditionalEditor.h"
#include "core/items/condition/CConditional.h"

/* CConditionalEditorUI */

CConditionalEditorUI::CConditionalEditorUI(wxWindow* pParent, CConditional& rEdit)
    : IConditionalEditor(pParent)
    , m_rEdit(rEdit)
{

}

void CConditionalEditorUI::onBtnNew(wxCommandEvent& event)
{

}

void CConditionalEditorUI::onBtnDelete(wxCommandEvent& event)
{

}

void CConditionalEditorUI::onBtnUp(wxCommandEvent& event)
{

}

void CConditionalEditorUI::onBtnDown(wxCommandEvent& event)
{

}

/* CConditionalEditor */

CConditionalEditor::CConditionalEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem)
    : CNameDescEditor(rMainWindow, rItem)
    , m_pUi(new CConditionalEditorUI(m_pPanel, dynamic_cast<CConditional&>(rItem) ))
{
    m_pSizer->Add(m_pUi, 1, wxEXPAND, 5);
    m_pPanel->Layout();
}

ITreeItemEditor* CConditionalEditor::Create(CMainWindow& rMainWindow, IProjTreeItem& rItem)
{
    return new CConditionalEditor(rMainWindow, rItem);
}