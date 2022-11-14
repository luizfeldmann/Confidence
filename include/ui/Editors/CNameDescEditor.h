#ifndef _CNAMEDESCEDITOR_
#define _CNAMEDESCEDITOR_

#include "ui/Editors/ITreeItemEditor.h"

//! Fired by the editor when the owned item name has changed
//! The "client data" is a pointer to the relevant IProjTreeItem
wxDECLARE_EVENT(EVT_EDITOR_ITEM_CHANGED_NAME, wxCommandEvent);

//! Fired by the editor when the owned item description has changed
//! The "client data" is a pointer to the relevant IProjTreeItem
wxDECLARE_EVENT(EVT_EDITOR_ITEM_CHANGED_DESC, wxCommandEvent);

class CNameDescEditorUI;

//! @copydoc CNameDescEditorUI
class CNameDescEditor : public ITreeItemEditor
{
protected:
    CNameDescEditorUI* const m_pUiNameDesc;

public:
    CNameDescEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @copydoc INotifyItemOperation::ItemChanged
    void OnAnyItemRenamed(const IProjTreeItem& rChanged) override;
};

#endif
