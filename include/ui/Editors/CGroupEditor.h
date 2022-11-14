#ifndef _CGROUPEDITOR_
#define _CGROUPEDITOR_

#include "ui/Editors/CNameDescEditor.h"

class CGroupEditorUI;

//! @copydoc CGroupEditorUI
class CGroupEditor : public CNameDescEditor
{
protected:
    CGroupEditorUI* const m_pUiGroup;

public:
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