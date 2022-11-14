#ifndef _CVARIABLEEDITOR_
#define _CVARIABLEEDITOR_

#include "ui/Editors/CNameDescEditor.h"

class CVariableEditorUI;

//! @copydoc CVariableEditorUI
class CVariableEditor : public CNameDescEditor
{
protected:
    CVariableEditorUI* const m_pUiVar;

public:
    CVariableEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @copydoc INotifyItemOperation::OnItemCreated
    void OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent) override;

    //! @copydoc INotifyItemOperation::OnAnyItemErased
    void OnAnyItemErased(const IProjTreeItem& rItem) override;

    //! @copydoc INotifyItemOperation::OnAnyItemRenamed
    void OnAnyItemRenamed(const IProjTreeItem& rChanged) override;

    //! @brief Returns a new instance of this class
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);
};

#endif