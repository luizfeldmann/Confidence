#ifndef _CTREEBROWSERMODEL_H_
#define _CTREEBROWSERMODEL_H_

#include "ui/Models/CBaseModel.h"
#include "ui/Models/CBaseTreeItemModel.h"
#include <wx/event.h>

#pragma warning(disable: 4250)

class IProjTreeItem;

//! Fired by the model when the item has been renamed on the tree itself
wxDECLARE_EVENT(EVT_TREE_ITEM_RENAME, wxCommandEvent);

//! @brief Model to connect the GUI to the backend project structure
class CTreeBrowserModel : public wxEvtHandler, public CBaseModel, public CBaseTreeItemModel
{
protected:
    //! Reference to the project whose tree is shown/edited
    IProjTreeItem& m_rRootItem;

    //! @brief Defines the columns shown in the tree browser
    enum class ETreeBrowserColumn
    {
        IconAndName = 0,
        Description,

        /* MUST BE LAST */
        Count
    };

    //! Stores the callbacks to read/write each column
    std::unordered_map<ETreeBrowserColumn, std::unique_ptr<IModelColumnHandler>> m_mapColumns;

    //! @copydoc CBaseModel::GetColumnInfo
    IModelColumnHandler* GetColumnInfo(unsigned int nModelColumn) override;

    //! @copydoc CBaseModel::GetColumnInfo
    const IModelColumnHandler* GetColumnInfo(unsigned int nModelColumn) const override;

    //! @copydoc CBaseTreeItemModel::GetRootItem
    const IProjTreeItem& GetRootItem() const override;

public:
    //! @brief Creates a model reflecting the tree structure of a project item
    //! @param[in] rRootItem The root item of the tree view
    CTreeBrowserModel(IProjTreeItem& rRootItem);

    ~CTreeBrowserModel();

    /* wxDataViewModel overrides */
    unsigned int GetColumnCount() const wxOVERRIDE;
    bool IsContainer(const wxDataViewItem& item) const wxOVERRIDE;
};

#endif