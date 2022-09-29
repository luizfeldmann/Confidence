#ifndef _CTREEBROWSERMODEL_H_
#define _CTREEBROWSERMODEL_H_

#include "ui/Models/CBaseModel.h"
#include <wx/event.h>
#include <map>
#include <unordered_map>

class IProjTreeItem;

//! Fired by the model when the item has been renamed on the tree itself
wxDECLARE_EVENT(EVT_TREE_ITEM_RENAME, wxCommandEvent);

//! @brief Model to connect the GUI to the backend project structure
class CTreeBrowserModel : public wxEvtHandler, public CBaseModel
{
protected:
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

public:
    CTreeBrowserModel();
    ~CTreeBrowserModel();

    /* wxDataViewModel overrides */
    unsigned int GetColumnCount() const wxOVERRIDE;
    wxDataViewItem GetParent(const wxDataViewItem& item) const wxOVERRIDE;
    bool IsContainer(const wxDataViewItem& item) const wxOVERRIDE;
    unsigned int GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const wxOVERRIDE;
};

#endif