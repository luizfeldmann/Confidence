/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CTREEITEMEDITORMANAGER_H_
#define _CTREEITEMEDITORMANAGER_H_

#include "ui/Editors/ITreeItemEditor.h"

//! @brief Manages a collection of "editors" / a tab corresponding to the tree item
class CTreeItemEditorManager : public wxEvtHandler, public INotifyItemOperation
{
public:
    //! @brief Constructs a manager of editors
    //! @param[in] rMainWindow @copybrief m_rMainWindow
    CTreeItemEditorManager(CMainWindow& rMainWindow);

    ~CTreeItemEditorManager();

    //! @brief Creates a tab for the item, or focuses it if already exists
    void ActivateItem(IProjTreeItem& pEditItem);

    //! @copydoc INotifyItemOperation::OnItemCreated
    void OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent) override;

    //! @brief Closes the editor tab when an item is deleted
    void OnAnyItemErased(const IProjTreeItem& pEditItem) override;

    //! @brief The item was renamed outside the editor (renamed on the tree)
    //!        The tab title must be updated
    void OnAnyItemRenamed(const IProjTreeItem& pEditItem) override;

    //! @brief Closes all tabs
    void Clear();

protected:
    //! The main GUI window
    CMainWindow& m_rMainWindow;

    //! Type of pointer used to pass/store the managed editors
    using PtrEditor = std::unique_ptr<ITreeItemEditor>;

    //! Collection used to hold the open editors
    using VecPtrEditor = std::vector<PtrEditor>;

    //! Collection of all the editors/tabs currently open
    VecPtrEditor m_vEditors;

    //! @brief Finds the editor correspondig to the provided GUI control
    VecPtrEditor::iterator FindEditor(wxWindow*);

    //! @brief Finds the editor corresponding to the provided item
    VecPtrEditor::iterator FindEditor(IProjTreeItem&);

    //! @copydoc FindEditor
    VecPtrEditor::const_iterator FindEditor(const IProjTreeItem&);

    //! @brief Event handler for when user clicks the close button on the editor tab
    void OnPageClose(wxAuiNotebookEvent& event);
};

#endif