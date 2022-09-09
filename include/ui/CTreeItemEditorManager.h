#ifndef _CTREEITEMEDITORMANAGER_H_
#define _CTREEITEMEDITORMANAGER_H_

#include "ui/ITreeItemEditor.h"

//! @brief Manages a collection of "editors" / a tab corresponding to the tree item
class CTreeItemEditorManager : public wxEvtHandler
{
public:
    CTreeItemEditorManager(wxAuiNotebook& rNotebook);
    ~CTreeItemEditorManager();

    //! @brief Creates a tab for the item, or focuses it if already exists
    void ActivateItem(IProjTreeItem& pEditItem);

    //! @brief Closes the editor tab when an item is deleted
    void ItemDeleted(IProjTreeItem& pEditItem);

    //! @brief The item was renamed outside the editor (renamed on the tree)
    //!        The tab title must be updated
    void ItemRenamed(IProjTreeItem& pEditItem);

    //! @brief Closes all tabs
    void Clear();

protected:
    //! The tabbed control on the GUI
    wxAuiNotebook& m_rNotebook;

    using PtrEditor = std::unique_ptr<ITreeItemEditor>;
    using VecPtrEditor = std::vector<PtrEditor>;

    //! Collection of all the editors/tabs currently open
    VecPtrEditor m_vEditors;

    //! @brief Finds the editor correspondig to the provided GUI control
    VecPtrEditor::iterator FindEditor(wxWindow*);

    //! @brief Finds the editor corresponding to the provided item
    VecPtrEditor::iterator FindEditor(IProjTreeItem&);

    //! @brief Event handler for when user clicks the close button on the editor tab
    void OnPageClose(wxAuiNotebookEvent& event);
};

#endif