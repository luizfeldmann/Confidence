#ifndef _CCONFIGURATIONEDITOR_
#define _CCONFIGURATIONEDITOR_

#include "ui/Editors/CNameDescEditor.h"
#include "wxExport/IConfigurationEditor.h"

class CConfiguration;
class CConfigurationSummaryModel;

//! @brief Item editor for configurations
class CConfigurationEditorUI : public IConfigurationEditor, public INotifyItemOperation
{
protected:
    //! Pointer to the model managing the data view table for the configuration
    CConfigurationSummaryModel* m_pModel;

public:
    CConfigurationEditorUI(wxWindow* pParent, std::shared_ptr<const CProject> pProject, CConfiguration& rConfig);

    //! @copydoc INotifyItemOperation::OnItemCreated
    void OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent) override;

    //! @copydoc INotifyItemOperation::OnAnyItemErased
    void OnAnyItemErased(const IProjTreeItem& rItem) override;

    //! @copydoc INotifyItemOperation::OnAnyItemChanged
    void OnAnyItemRenamed(const IProjTreeItem& pItem) override;
};

//! @copydoc CConfigurationEditorUI
class CConfigurationEditor : public CNameDescEditor
{
protected:
    CConfigurationEditorUI* const m_pUi;

public:
    CConfigurationEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @copydoc INotifyItemOperation::OnItemCreated
    void OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent) override;

    //! @copydoc INotifyItemOperation::OnAnyItemErased
    void OnAnyItemErased(const IProjTreeItem& rItem) override;

    //! @copydoc INotifyItemOperation::OnAnyItemChanged
    void OnAnyItemRenamed(const IProjTreeItem& pItem) override;

    //! @brief Returns a new instance of this class
    static ITreeItemEditor* Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif