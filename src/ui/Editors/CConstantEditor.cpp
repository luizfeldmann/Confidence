#include "core/items/CConstant.h"
#include "ui/Editors/CConstantEditor.h"

/* CConstantEditorUI */

CConstantEditorUI::CConstantEditorUI(wxWindow* pParent, CConstant& rEdit)
    : IConstantEditor(pParent)
    , m_rEdit(rEdit)
{
    m_textCtrlExpression->ChangeValue(m_rEdit.GetExpression());

    bool bExport = m_rEdit.GetExportToEnv();
    m_checkBoxExport->SetValue(bExport);
    m_bitmapExport->Show(bExport);
}

void CConstantEditorUI::onExpressionChanged(wxCommandEvent& event)
{
    const std::string strExpr = m_textCtrlExpression->GetValue().ToStdString();

    if (!m_rEdit.SetExpression(strExpr))
        m_textCtrlExpression->ChangeValue(m_rEdit.GetExpression());
}

void CConstantEditorUI::onChkboxExport(wxCommandEvent& event)
{
    bool bExport = m_checkBoxExport->GetValue();
    m_rEdit.SetExportToEnv(bExport);
    m_bitmapExport->Show(bExport);
}

/* CConstantEditor */

CConstantEditor::CConstantEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem)
    : CNameDescEditor(rMainWindow, rItem)
    , m_pUi(new CConstantEditorUI(m_pPanel, dynamic_cast<CConstant&>(rItem)))
{
    m_pSizer->Add(m_pUi, 1, wxEXPAND, 5);
    m_pPanel->Layout();
}

ITreeItemEditor* CConstantEditor::Create(CMainWindow& rMainWindow, IProjTreeItem& rItem)
{
    return new CConstantEditor(rMainWindow, rItem);
}