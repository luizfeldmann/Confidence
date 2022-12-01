#include "ui/Editors/CConfigurationEditor.h"
#include "core/items/CConfiguration.h"
#include "ui/Models/CConfigurationSummaryModel.h"
#include "ui/CMainWindow.h"
#include "core/items/variable/CVariable.h"
#include "wxExport/IConfigurationEditor.h"

/* CConfigurationEditorUI */

//! @brief Item editor for configurations
class CConfigurationEditorUI : public IConfigurationEditor, public INotifyItemOperation
{
protected:
    //! Reference to the application's main window
    CMainWindow& m_rMainWindow;

    //! Pointer to the model managing the data view table for the configuration
    CConfigurationSummaryModel* m_pModel;

    /** @name IConfigurationEditor overrides */
    //!@{
    
    //! @brief When the user activates an item (a configuration) it's dedicated editor opens in a tab
    void onItemActivated(wxDataViewEvent& event) override;

    //!@}

public:
    //! @brief Constructs a new editor UI associated with the specified #CConfiguration
    //! @param[in] rMainWindow Reference to the main window of the application
    //! @param[in] pParent Pointer to the window where this editor will be created
    //! @param[in] pProject Pointer to the current project which owns \p rVar
    //! @param[in] rConfig Reference to the configuration to edit
    CConfigurationEditorUI(CMainWindow& rMainWindow, wxWindow* pParent, std::shared_ptr<const CProject> pProject, CConfiguration& rConfig);

    //! @copydoc INotifyItemOperation::OnItemCreated
    void OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent) override;

    //! @copydoc INotifyItemOperation::OnAnyItemErased
    void OnAnyItemErased(const IProjTreeItem& rItem) override;

    //! @copydoc INotifyItemOperation::OnAnyItemRenamed
    void OnAnyItemRenamed(const IProjTreeItem& pItem) override;
};


CConfigurationEditorUI::CConfigurationEditorUI(CMainWindow& rMainWindow, wxWindow* pParent, std::shared_ptr<const CProject> pProject, CConfiguration& rConfig)
    : IConfigurationEditor(pParent)
    , m_rMainWindow(rMainWindow)
    , m_pModel(new CConfigurationSummaryModel(rConfig, pProject, m_dataViewCtrl))
{
    // Associate model
    m_dataViewCtrl->AssociateModel(m_pModel);
    m_pModel->DecRef();

    // Initial items
    wxDataViewItem wxRoot = CConfigurationSummaryModel::GetViewItem(nullptr);
    wxDataViewItemArray arrItems;

    m_pModel->GetChildren(wxRoot, arrItems);
    m_pModel->ItemsAdded(wxRoot, arrItems);
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

void CConfigurationEditorUI::onItemActivated(wxDataViewEvent& event)
{
    CVariable* const pVariable = m_pModel->GetPointer( m_dataViewCtrl->GetSelection() );
    if (pVariable)
        m_rMainWindow.ActivateItem(*pVariable);
}

/* CConfigurationEditor */

CConfigurationEditor::CConfigurationEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem)
    : CNameDescEditor(rMainWindow, rItem)
    , m_pUi(new CConfigurationEditorUI(rMainWindow, m_pPanel, GetProject(), dynamic_cast<CConfiguration&>(rItem)))
{
    m_pSizer->Add(m_pUi, 1, wxEXPAND, 5);
    m_pPanel->Layout();
}

/* static */ std::unique_ptr<ITreeItemEditor> CConfigurationEditor::Create(CMainWindow& rMainWindow, IProjTreeItem& rItem)
{
    return std::make_unique<CConfigurationEditor>(rMainWindow, rItem);
}

void CConfigurationEditor::OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent)
{
    CNameDescEditor::OnItemCreated(rItem, rParent);
    m_pUi->OnItemCreated(rItem, rParent);
}

void CConfigurationEditor::OnAnyItemErased(const IProjTreeItem& rItem)
{
    CNameDescEditor::OnAnyItemErased(rItem);
    m_pUi->OnAnyItemErased(rItem);
}

void CConfigurationEditor::OnAnyItemRenamed(const IProjTreeItem& pItem)
{
    CNameDescEditor::OnAnyItemRenamed(pItem);
    m_pUi->OnAnyItemRenamed(pItem);
}