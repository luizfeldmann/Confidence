#ifndef _CCONFIGURATIONEDITOR_
#define _CCONFIGURATIONEDITOR_

#include "ui/Editors/CNameDescEditor.h"

class CConfigurationEditorUI;

//! @copydoc CConfigurationEditorUI
class CConfigurationEditor : public CNameDescEditor
{
protected:
    //! Pointer to the private implementation of the editor UI
    CConfigurationEditorUI* const m_pUi;

public:
    //! @brief Constructs a new #CConfigurationEditor
    //! @param[in] rMainWindow Reference to the main application window
    //! @param[in] rItem Reference to #CConfiguration
    CConfigurationEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @copydoc INotifyItemOperation::OnItemCreated
    void OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent) override;

    //! @copydoc INotifyItemOperation::OnAnyItemErased
    void OnAnyItemErased(const IProjTreeItem& rItem) override;

    //! @copydoc INotifyItemOperation::OnAnyItemRenamed
    void OnAnyItemRenamed(const IProjTreeItem& pItem) override;

    //! @brief Returns a new instance of this class
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif