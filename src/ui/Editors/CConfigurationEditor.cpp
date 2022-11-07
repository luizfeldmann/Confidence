#include "ui/Editors/CConfigurationEditor.h"
#include "core/items/CConfiguration.h"
#include "ui/Models/CConfigurationSummaryModel.h"

/* CConfigurationEditorUI */

CConfigurationEditorUI::CConfigurationEditorUI(wxWindow* pParent, std::shared_ptr<const CProject> pProject, CConfiguration& rConfig)
    : IConfigurationEditor(pParent)
    , m_pModel(new CConfigurationSummaryModel(rConfig, pProject, m_dataViewCtrl))
{
    m_dataViewCtrl->AssociateModel(m_pModel);
    m_pModel->DecRef();
}

void CConfigurationEditorUI::OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent)
{
    m_pModel->OnItemCreated(rItem, rParent);
}

void CConfigurationEditorUI::OnAnyItemErased(const IProjTreeItem& rItem)
{
    m_pModel->OnAnyItemErased(rItem);
}

void CConfigurationEditorUI::OnAnyItemRenamed(const IProjTreeItem& pItem)
{
    m_pModel->OnAnyItemRenamed(pItem);
}

/* CConfigurationEditor */

CConfigurationEditor::CConfigurationEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem)
    : CNameDescEditor(rMainWindow, rItem)
    , m_pUi(new CConfigurationEditorUI(m_pPanel, GetProject(), dynamic_cast<CConfiguration&>(rItem)))
{
    m_pSizer->Add(m_pUi, 1, wxEXPAND, 5);
    m_pPanel->Layout();
}

ITreeItemEditor* CConfigurationEditor::Create(CMainWindow& rMainWindow, IProjTreeItem& rItem)
{
    return new CConfigurationEditor(rMainWindow, rItem);
}

void CConfigurationEditor::OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent)
{
    m_pUi->OnItemCreated(rItem, rParent);
}

void CConfigurationEditor::OnAnyItemErased(const IProjTreeItem& rItem)
{
    m_pUi->OnAnyItemErased(rItem);
}

void CConfigurationEditor::OnAnyItemRenamed(const IProjTreeItem& pItem)
{
    m_pUi->OnAnyItemRenamed(pItem);
}