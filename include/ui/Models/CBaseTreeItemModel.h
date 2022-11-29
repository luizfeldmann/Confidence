#ifndef _CBASETREEITEMMODEL_H_
#define _CBASETREEITEMMODEL_H_

#include <wx/dataview.h>
#include <map>
#include "core/IProjTreeItem.h"

//! @brief Base implementation of a model having IProjTreeItems as items
class CBaseTreeItemModel : virtual public wxDataViewModel
{
protected:
    //! Type used to hold associations between the parent and child items in the tree model
    using parent_map_t = std::map<wxDataViewItem, wxDataViewItem>;
    
    //! Maps child (key) to parent (value)
    mutable parent_map_t m_mapParents;

    //! @brief Gets the root item of the tree view
    virtual const IProjTreeItem& GetRootItem() const = 0;

public:
    //! @brief Casts the DataViewItem into a ProjectItem pointer
    static IProjTreeItem* GetPointer(const wxDataViewItem& item);

    //! @brief Casts the project tree item into a data view item
    static wxDataViewItem GetViewItem(const IProjTreeItem* pItem);

    /** @name wxDataViewModel overrides */
    //!@{
    
    //! @copydoc CBaseModel::GetParent
    wxDataViewItem GetParent(const wxDataViewItem& item) const wxOVERRIDE;

    //! @copydoc CBaseModel::GetChildren
    unsigned int GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const wxOVERRIDE;

    //!@}
};

#endif // _CBASETREEITEMMODEL_H_
