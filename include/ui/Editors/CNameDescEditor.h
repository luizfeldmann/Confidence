/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

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
    //! Pointer to the private implementation of the editor UI
    CNameDescEditorUI* const m_pUiNameDesc;

public:
    //! @brief Constructs a new #CNameDescEditor
    //! @param[in] rMainWindow Reference to the main application window
    //! @param[in] rItem Reference to any #IProjTreeItem
    CNameDescEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @copydoc INotifyItemOperation::OnAnyItemRenamed
    void OnAnyItemRenamed(const IProjTreeItem& rChanged) override;
};

#endif
