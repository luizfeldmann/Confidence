#ifndef _CNAMEDESCEDITOR_
#define _CNAMEDESCEDITOR_

#include "ui/Editors/ITreeItemEditor.h"
#include "wxExport/INameDescEditor.h"

//! Fired by the editor when the owned item name has changed
//! The "client data" is a pointer to the relevant IProjTreeItem
wxDECLARE_EVENT(EVT_EDITOR_ITEM_CHANGED_NAME, wxCommandEvent);

//! Fired by the editor when the owned item description has changed
//! The "client data" is a pointer to the relevant IProjTreeItem
wxDECLARE_EVENT(EVT_EDITOR_ITEM_CHANGED_DESC, wxCommandEvent);

class CNameDescEditor;

//! @brief Item editor containing text boxes to edit the name and the description 
class CNameDescEditorUI : public INameDescEditor, public INotifyItemOperation
{
protected:
    //! Reference to the item being edited
    CNameDescEditor& m_rEditor;

    /* Overrides from INameDescEditor */
    void onChangedName(wxCommandEvent& event) override;
    void onChangedDesc(wxCommandEvent& event) override;

public:
    CNameDescEditorUI(wxWindow* pParent, CNameDescEditor& rEditor);

    //! @copydoc INotifyItemOperation::OnAnyItemRenamed
    void OnAnyItemRenamed(const IProjTreeItem& rChanged) override;
};

//! @copydoc CNameDescEditorUI
class CNameDescEditor : public ITreeItemEditor
{
protected:
    CNameDescEditorUI* const m_pUiNameDesc;

public:
    CNameDescEditor(wxAuiNotebook& rNotebook, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static ITreeItemEditor* Create(wxAuiNotebook& rNotebook, IProjTreeItem& rItem);

    //! @copydoc INotifyItemOperation::ItemChanged
    void OnAnyItemRenamed(const IProjTreeItem& rChanged) override;
};

#endif
