#include "ui/Editors/CConditionalEditor.h"
#include "ui/Models/CConditionalTreeModel.h"
#include "core/items/condition/CLogicalOperator.h"
#include "core/items/condition/CLogicalComparator.h"
#include "core/items/condition/CConditional.h"
#include <wx/menu.h>
#include <functional>

/* CConditionalEditorUI */

CConditionalEditorUI::CConditionalEditorUI(wxWindow* pParent, CConditional& rEdit)
    : IConditionalEditor(pParent)
    , m_rEdit(rEdit)
    , m_pModel(new CConditionalTreeModel(m_rEdit))
{
    m_dataViewCtrl->AssociateModel(m_pModel);
    m_pModel->DecRef();
}

struct SConditionalCreationInfo
{
    using fnFactory_t = std::function<std::shared_ptr<ICondition>()>;

    wxString m_strName;
    wxIcon m_cIcon;
    fnFactory_t m_fnFactory;

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

ITreeItemEditor* CConditionalEditor::Create(CMainWindow& rMainWindow, IProjTreeItem& rItem)
{
    return new CConditionalEditor(rMainWindow, rItem);
}