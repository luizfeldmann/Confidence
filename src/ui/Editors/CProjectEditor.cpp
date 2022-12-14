/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "ui/Editors/CProjectEditor.h"
#include "ui/Models/CDocExporterTableModel.h"
#include "core/items/CProject.h"
#include "ui/SDocExporterTypeInfo.h"
#include "ui/CAcceleratorEntry.h"
#include "wxExport/IProjectEditor.h"
#include <wx/menu.h>
#include <array>

/* CProjectEditorUI */

//! @brief Item editor for the project
class CProjectEditorUI : public IProjectEditor
{
protected:
    //! The underlying project being edited
    CProject& m_rEdit;

    //! The model used to manage the list of exporters
    CDocExporterTableModel* const m_pModel;

    //! Manages keyboard shortcuts
    wxAcceleratorTable m_cAccTbl;

    //! Event fired when the user choses the type of exporter to add, from the popup menu
    void onBtnNewItemMenu(wxCommandEvent& evt);

public:
    //! @brief Constructs a new editor UI associated with the specified #CProject
    //! @param[in] pParent Pointer to the window where this editor will be created
    //! @param[in] rEdit @copybrief m_rEdit
    CProjectEditorUI(wxWindow* pParent, CProject& rEdit);

    /** @name IProjectEditor overrides */
    //!@{
    
    //! @brief Creates a new document exporter in the list of exporters
    void onToolNewDoc(wxCommandEvent& event) override;

    //! @brief Deletes the selected exporter from the list
    void onToolDeleteDoc(wxCommandEvent& event) override;

    //!@}
};

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

    // Configure accelerator
    using acc_entry_t = CAcceleratorEntry;
    const std::array<acc_entry_t, 2> arrAccEntry{
        acc_entry_t(wxACCEL_CTRL,    (int)'N',   m_toolNewDoc),
        acc_entry_t(wxACCEL_NORMAL,  WXK_DELETE, m_toolDeleteDoc),
    };

    m_cAccTbl = wxAcceleratorTable(arrAccEntry.size(), arrAccEntry.data());
    m_dataViewCtrlDocs->SetAcceleratorTable(m_cAccTbl);
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
CProjectEditor::CProjectEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem)
    : CNameDescEditor(rMainWindow, rItem)
    , m_pUiProj(new CProjectEditorUI(m_pPanel, dynamic_cast<CProject&>(rItem)))
{
    m_pSizer->Add(m_pUiProj, 1, wxEXPAND, 5);
    m_pPanel->Layout();
}

/* static */ std::unique_ptr<ITreeItemEditor> CProjectEditor::Create(CMainWindow& rMainWindow, IProjTreeItem& rItem)
{
    return std::make_unique<CProjectEditor>(rMainWindow, rItem);
}