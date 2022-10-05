#include "ui/Editors/CProjectEditor.h"
#include "ui/Models/CDocExporterTableModel.h"
#include "core/CProject.h"
#include "ui/SDocExporterTypeInfo.h"
#include <wx/menu.h>

/* CProjectEditorUI */
CProjectEditorUI::CProjectEditorUI(wxWindow* pParent, CProject& rEdit)
    : IProjectEditor(pParent)
    , m_rEdit(rEdit)
    , m_pModel(new CDocExporterTableModel(rEdit))
{
    // Create exporters model
    m_dataViewCtrlDocs->AssociateModel(m_pModel);
    m_pModel->DecRef();

    // Draw all initial items
    wxDataViewItem wxRoot = CDocExporterTableModel::GetViewItem(nullptr);
    wxDataViewItemArray arrItems;

    m_pModel->GetChildren(wxRoot, arrItems);
    m_pModel->ItemsAdded(wxRoot, arrItems);
}

void CProjectEditorUI::onBtnNewItemMenu(wxCommandEvent& evt)
{
    const EDocExporter eType = (EDocExporter)evt.GetId();
    const wxDataViewItem cItem = m_pModel->NewItem(eType);

    m_dataViewCtrlDocs->Select(cItem);
}

void CProjectEditorUI::onToolNewDoc(wxCommandEvent& event)
{
    wxMenu menu;
    menu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CProjectEditorUI::onBtnNewItemMenu), NULL, this);

    for (const auto& rInfo : SDocExporterTypeInfo::GetRegistry())
    {
        wxMenuItem* pMenuItem = menu.Append((int)rInfo.first, rInfo.second.strTypeName);
    }

    // Display the menu only if there are relevant options
    if (0 < menu.GetMenuItemCount())
        PopupMenu(&menu);
}

void CProjectEditorUI::onToolDeleteDoc(wxCommandEvent& event)
{
    if (!m_dataViewCtrlDocs->HasSelection())
        return;

    m_pModel->DeleteItem(m_dataViewCtrlDocs->GetSelection());
}

/* CProjectEditor */
CProjectEditor::CProjectEditor(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
    : CNameDescEditor(rNotebook, rItem)
    , m_pUiProj(new CProjectEditorUI(m_pPanel, dynamic_cast<CProject&>(rItem)))
{
    m_pSizer->Add(m_pUiProj, 1, wxEXPAND, 5);
    m_pPanel->Layout();
}

ITreeItemEditor* CProjectEditor::Create(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
{
    return new CProjectEditor(rNotebook, rItem);
}