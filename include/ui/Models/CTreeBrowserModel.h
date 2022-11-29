#ifndef _CTREEBROWSERMODEL_H_
#define _CTREEBROWSERMODEL_H_

#include "ui/Models/CBaseModelMapped.h"
#include "ui/Models/CBaseTreeItemModel.h"
#include <wx/event.h>

#pragma warning(disable: 4250)

class IProjTreeItem;

//! Fired by the model when the item has been renamed on the tree itself
wxDECLARE_EVENT(EVT_TREE_ITEM_RENAME, wxCommandEvent);

//! @brief Model to connect the GUI to the backend project structure
class CTreeBrowserModel : public wxEvtHandler, public CBaseModelMapped, public CBaseTreeItemModel
{
protected:
    //! Pointer to the project whose tree is shown/edited
    std::weak_ptr<const IProjTreeItem> m_pRootItem;

    //! @brief Defines the columns shown in the tree browser
    enum class EColumn : unsigned int
    {
        IconAndName = 0,
        Description,

        /* MUST BE LAST */
        Count
    };

    //! @copydoc CBaseTreeItemModel::GetRootItem
    const IProjTreeItem& GetRootItem() const override;

public:
    //! @brief Creates a model reflecting the tree structure of a project item
    CTreeBrowserModel(std::weak_ptr<const IProjTreeItem> pNewRoot);

    ~CTreeBrowserModel();

    //! @brief Changes the root item of the tree model and reload the tree
    void SetRoot(std::weak_ptr<const IProjTreeItem> pNewRoot);

    //! @brief Removes the currently selected item from the tree and takes it's ownership
    //! @param[in] rItem The item to remove from the collection
    ITreeItemCollection::ptr_t TakeItem(const wxDataViewItem& rItem);

    //! @brief Moves the specified item up or down in the tree
    //! @param[in] rItem The item to move
    //! @param[in] bUp The direction to move
    //! @return True if success
    bool MoveItem(const wxDataViewItem& rItem, bool bUp);

    //! @brief Inserts a new item into the parent node
    //! @return True if success - the parent object takes ownership of the pointer \p pInsertItem
    bool InsertItem(const wxDataViewItem& rParent, IProjTreeItem::ptr_t pInsertItem);

    /** @name wxDataViewModel overrides */
    //!@{

    //! @copydoc CBaseModel::GetColumnCount
    unsigned int GetColumnCount() const wxOVERRIDE;

    //! @copydoc CBaseModel::IsContainer
    bool IsContainer(const wxDataViewItem& item) const wxOVERRIDE;

    //!@}
};

#endif