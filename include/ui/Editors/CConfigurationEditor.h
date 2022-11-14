#ifndef _CCONFIGURATIONEDITOR_
#define _CCONFIGURATIONEDITOR_

#include "ui/Editors/CNameDescEditor.h"

class CConfigurationEditorUI;

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
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif