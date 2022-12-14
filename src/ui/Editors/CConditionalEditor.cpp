/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "ui/Editors/CConditionalEditor.h"
#include "ui/Models/CConditionalTreeModel.h"
#include "ui/CAcceleratorEntry.h"
#include "core/items/condition/CLogicalOperator.h"
#include "core/items/condition/CLogicalComparator.h"
#include "core/items/condition/CConditional.h"
#include "wxExport/IConditionalEditor.h"
#include <wx/menu.h>
#include <functional>
#include <array>

/* CConditionalEditorUI */

//! @brief Item editor for conditionals
class CConditionalEditorUI : public IConditionalEditor
{
protected:
    //! The underlying conditional item being edited
    CConditional& m_rEdit;

    //! The model used to manage the table of conditions
    CConditionalTreeModel* const m_pModel;

    //! Manages keyboard shortcuts
    wxAcceleratorTable m_cAccTbl;

    //! @brief Creates a new condition of the type selected by the user on the popup menu
    void onBtnNewSelected(wxCommandEvent& event);

    /** @name IConditionalEditor overrides */
    //!@{
    
    //! @brief Opens a popup menu for the user to chose a type of condition to create
    void onBtnNew(wxCommandEvent& event) override;
    
    //! @brief Deletes the selected item
    void onBtnDelete(wxCommandEvent& event) override;

    //! @brief Moves the selected item up
    void onBtnUp(wxCommandEvent& event) override;

    //! @brief Moves the selected item down
    void onBtnDown(wxCommandEvent& event) override;

    //!@}

public:
    //! @brief Constructs a new editor UI associated with the specified #CConditional
    CConditionalEditorUI(wxWindow* pParent, CConditional& rEdit);

};

CConditionalEditorUI::CConditionalEditorUI(wxWindow* pParent, CConditional& rEdit)
    : IConditionalEditor(pParent)
    , m_rEdit(rEdit)
    , m_pModel(new CConditionalTreeModel(m_rEdit))
{
    // Manually create the string/numeric column
    wxArrayString arrDataMode;
    arrDataMode.Add("Number");  // Option 0: GetStringMode() == false
    arrDataMode.Add("Text");    // Option 1: GetStringMode() == true

    m_dataViewCtrl->InsertColumn(m_dataViewCtrl->GetColumnCount(), // Make last
        new wxDataViewColumn("Data", new wxDataViewChoiceByIndexRenderer(arrDataMode),
            (unsigned int)CConditionalTreeModel::EColumn::DataMode, -1, wxALIGN_LEFT, wxDATAVIEW_COL_RESIZABLE));

    // Associate the model
    m_dataViewCtrl->AssociateModel(m_pModel);
    m_pModel->DecRef();

    // Configure accelerator
    using acc_entry_t = CAcceleratorEntry;
    const std::array<acc_entry_t, 4> arrAccEntry{
        acc_entry_t(wxACCEL_CTRL,    (int)'N',   m_toolNew),
        acc_entry_t(wxACCEL_NORMAL,  WXK_DELETE, m_toolDelete),
        acc_entry_t(wxACCEL_CTRL,    WXK_UP,     m_toolUp),
        acc_entry_t(wxACCEL_CTRL,    WXK_DOWN,   m_toolDown),
    };

    m_cAccTbl = wxAcceleratorTable(arrAccEntry.size(), arrAccEntry.data());
    m_dataViewCtrl->SetAcceleratorTable(m_cAccTbl);
}

//! @brief Stores display and factory information about the available types of #ICondition
//! @internal
struct SConditionalCreationInfo
{
    //! Type of factory function to create a new #ICondition
    using fnFactory_t = std::function<std::shared_ptr<ICondition>()>;

    //! The display name of a particular condition type
    wxString m_strName;

    //! The display icon of a particular condition type
    wxIcon m_cIcon;

    //! The factory function to instantiate a particular condition type
    fnFactory_t m_fnFactory;

    //! Gets a collection of #SConditionalCreationInfo corresponding to all the available implementations of #ICondition
    static const std::vector<SConditionalCreationInfo>& Get()
    {
        static const std::vector<SConditionalCreationInfo> c_vInfo {
            {
                "AND", 
                CConditionalTreeModel::GetIcon(ELogicalOperator::And),
                std::bind(&CLogicalOperator::Create, ELogicalOperator::And)
            },
            {   "NOT",
                CConditionalTreeModel::GetIcon(ELogicalOperator::Not),
                std::bind(&CLogicalOperator::Create, ELogicalOperator::Not)
            },
            {
                "OR ", 
                CConditionalTreeModel::GetIcon(ELogicalOperator::Or ),
                std::bind(&CLogicalOperator::Create, ELogicalOperator::Or)
            },
            {
                "Equal", 
                CConditionalTreeModel::GetIcon(ELogicalComparator::Equal),
                std::bind(&CLogicalComparator::Create, ELogicalComparator::Equal)
            },
            {
                "Not Equal", 
                CConditionalTreeModel::GetIcon(ELogicalComparator::NotEqual),
                std::bind(&CLogicalComparator::Create, ELogicalComparator::NotEqual)
            },
            {
                "Less", 
                CConditionalTreeModel::GetIcon(ELogicalComparator::Less),
                std::bind(&CLogicalComparator::Create, ELogicalComparator::Less)
            },
            {
                "Less/Equal", 
                CConditionalTreeModel::GetIcon(ELogicalComparator::LessOrEqual),
                std::bind(&CLogicalComparator::Create, ELogicalComparator::LessOrEqual)
            },
            {
                "Greater", 
                CConditionalTreeModel::GetIcon(ELogicalComparator::Greater),
                std::bind(&CLogicalComparator::Create, ELogicalComparator::Greater)
            },
            {
                "Greater/Equal", 
                CConditionalTreeModel::GetIcon(ELogicalComparator::GreaterOrEqual),
                std::bind(&CLogicalComparator::Create, ELogicalComparator::GreaterOrEqual)
            },
        };

        return c_vInfo;
    }
};

void CConditionalEditorUI::onBtnNew(wxCommandEvent& event)
{
    wxMenu menu;
    menu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CConditionalEditorUI::onBtnNewSelected), NULL, this);

    size_t iIndex = 0;
    for (const SConditionalCreationInfo& rInfo : SConditionalCreationInfo::Get())
    {
        wxMenuItem* pMenuItem = menu.Append(iIndex++, rInfo.m_strName);
        pMenuItem->SetBitmap(rInfo.m_cIcon);

        enum { ESeparatorPosition = 3 };
        if (iIndex == ESeparatorPosition)
            menu.AppendSeparator();
    }

    PopupMenu(&menu);
}

void CConditionalEditorUI::onBtnNewSelected(wxCommandEvent& event)
{
    const SConditionalCreationInfo& rInfo = SConditionalCreationInfo::Get().at(event.GetId());
    std::shared_ptr<ICondition> pCondition = rInfo.m_fnFactory();

    wxDataViewItem wxParent = m_dataViewCtrl->GetSelection();
    wxDataViewItem wxItem = m_pModel->NewItem(pCondition, wxParent);
    m_dataViewCtrl->Expand(wxParent);
    m_dataViewCtrl->Select(wxItem);
}

void CConditionalEditorUI::onBtnDelete(wxCommandEvent& event)
{
    if (m_dataViewCtrl->HasSelection())
    {
        const wxDataViewItem cSelection = m_dataViewCtrl->GetSelection();
        m_pModel->DeleteItem(cSelection);
    }
}

void CConditionalEditorUI::onBtnUp(wxCommandEvent& event)
{
    if (m_dataViewCtrl->HasSelection())
    {
        const wxDataViewItem cSelection = m_dataViewCtrl->GetSelection();
        
        if (m_pModel->MoveItem(cSelection, true))
            m_dataViewCtrl->Select(cSelection);
    }
}

void CConditionalEditorUI::onBtnDown(wxCommandEvent& event)
{
    if (m_dataViewCtrl->HasSelection())
    {
        const wxDataViewItem cSelection = m_dataViewCtrl->GetSelection();
        
        if (m_pModel->MoveItem(cSelection, false))
            m_dataViewCtrl->Select(cSelection);
    }
}

/* CConditionalEditor */

CConditionalEditor::CConditionalEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem)
    : CNameDescEditor(rMainWindow, rItem)
    , m_pUi(new CConditionalEditorUI(m_pPanel, dynamic_cast<CConditional&>(rItem) ))
{
    m_pSizer->Add(m_pUi, 1, wxEXPAND, 5);
    m_pPanel->Layout();
}

/* static */ std::unique_ptr<ITreeItemEditor> CConditionalEditor::Create(CMainWindow& rMainWindow, IProjTreeItem& rItem)
{
    return std::make_unique<CConditionalEditor>(rMainWindow, rItem);
}