/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

/**
 * @file ITreeItemEditor.h
 * @brief Defines interface #ITreeItemEditor
 */

#ifndef _ITREEITEMEDITOR_H_
#define _ITREEITEMEDITOR_H_

#include <wx/panel.h>
#include <wx/aui/auibook.h>
#include "core/INotifyItemOperation.h"

class IProjTreeItem;
class CMainWindow;
class CProject;

//! @brief Custom content of the editor tab opened for a specific project tree item
class ITreeItemEditor : public INotifyItemOperation
{
private:
    ITreeItemEditor(const ITreeItemEditor&) = delete;               //! Non copy-constructible
    ITreeItemEditor& operator=(const ITreeItemEditor&) = delete;    //! Non copy-assignable

protected:
    //! Reference to the main editor window
    CMainWindow& m_rMainWindow;

    //! Reference to the item being edited
    IProjTreeItem& m_rItem;

    //! The tab page containing the editor
    wxPanel* const m_pPanel;

    //! The layout sizer that will receive the controls for the editor
    wxSizer* const m_pSizer;

public:
    //! Constructs an editor tab for the specified project item
    //! @param[in] rMainWindow @copybrief m_rMainWindow
    //! @param[in] rItem @copybrief m_rItem
    ITreeItemEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    virtual ~ITreeItemEditor();

    //! @brief Default move-constructor
    ITreeItemEditor(ITreeItemEditor&&) = default;

    //! @brief Default move-assignment operator
    ITreeItemEditor& operator=(ITreeItemEditor&&) = default;

    //! Prototype callback used to create a new editor for the provided item
    using fnFactory_t = std::function<std::unique_ptr<ITreeItemEditor>(CMainWindow&, IProjTreeItem&)>;

    //! @brief Gets the item being edited
    IProjTreeItem& GetItem();

    //! @brief Get project being editted
    std::shared_ptr<const CProject> GetProject();

    //! @brief Gets the notebook tabbed control that contains this editor
    wxAuiNotebook& GetNotebook();

    //! @brief Gets the tab page used by the editor
    wxPanel* GetPage();

    //! @copydoc INotifyItemOperation::OnAnyItemRenamed
    void OnAnyItemRenamed(const IProjTreeItem& rChanged) override;

    //! @brief Sets this editor as the current selection of the tabbed notebook
    virtual void Reactivate();
};

#endif _ITREEITEMEDITOR_H_