#ifndef _ITREEITEMEDITOR_H_
#define _ITREEITEMEDITOR_H_

#include <wx/panel.h>
#include <wx/aui/auibook.h>
#include "core/IProjTreeItem.h"

//! @brief Custom content of the editor tab opened for a specific project tree item
class ITreeItemEditor
{
private:
    ITreeItemEditor(const ITreeItemEditor&) = delete;               //! Non copy-constructible
    ITreeItemEditor& operator=(const ITreeItemEditor&) = delete;    //! Non copy-assignable

protected:
    //! Reference to the tabbed notebook where the editor is displayed
    wxAuiNotebook& m_rNotebook;

    //! Reference to the item being edited
    IProjTreeItem& m_rItem;

    //! The tab page containing the editor
    wxPanel* const m_pPanel;

    //! The layout sizer that will receive the controls for the editor
    wxSizer* const m_pSizer;

public:
    ITreeItemEditor(wxAuiNotebook& rNotebook, IProjTreeItem& rItem);
    virtual ~ITreeItemEditor();

    ITreeItemEditor(ITreeItemEditor&&) = default;
    ITreeItemEditor& operator=(ITreeItemEditor&&) = default;

    //! Prototype callback used to create a new editor for the provided item
    using fnFactory_t = std::function<ITreeItemEditor* (wxAuiNotebook&, IProjTreeItem&)>;

    //! @brief Gets the item being edited
    IProjTreeItem& GetItem();

    //! @brief Gets the notebook tabbed control that contains this editor
    wxAuiNotebook& GetNotebook();

    //! @brief Gets the tab page used by the editor
    wxPanel* GetPage();

    //! @brief Notifies the editor the item was changed and must be reloaded
    virtual void ItemChanged();

    //! @brief Sets this editor as the current selection of the tabbed notebook
    virtual void Reactivate();
};

#endif _ITREEITEMEDITOR_H_