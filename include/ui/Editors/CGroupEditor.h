#ifndef _CGROUPEDITOR_
#define _CGROUPEDITOR_

#include "ui/Editors/CNameDescEditor.h"

class CGroupEditorUI;

//! @copydoc CGroupEditorUI
class CGroupEditor : public CNameDescEditor
{
protected:
    //! Pointer to the private implementation of the editor UI
    CGroupEditorUI* const m_pUiGroup;

public:
    //! @brief Constructs a new #CGroupEditor
    //! @param[in] rMainWindow Reference to the main application window
    //! @param[in] rItem Reference to #CGroup
    CGroupEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @copydoc INotifyItemOperation::OnItemCreated
    void OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent) override;

    //! @copydoc INotifyItemOperation::OnAnyItemErased
    void OnAnyItemErased(const IProjTreeItem& rItem) override;

    //! @copydoc INotifyItemOperation::OnAnyItemRenamed
    void OnAnyItemRenamed(const IProjTreeItem& rChanged) override;
};

#endif