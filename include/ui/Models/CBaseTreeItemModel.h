#ifndef _CBASETREEITEMMODEL_H_
#define _CBASETREEITEMMODEL_H_

#include <wx/dataview.h>
#include <map>

class IProjTreeItem;

//! @brief Base implementation of a model having IProjTreeItems as items
class CBaseTreeItemModel : virtual public wxDataViewModel
{
protected:
    //! Maps child (key) to parent (value)
    mutable std::map<wxDataViewItem, wxDataViewItem> m_mapParents;

    //! @brief Gets the root item of the tree view
    virtual const IProjTreeItem& GetRootItem() const = 0;

public:
    //! @brief Casts the DataViewItem into a ProjectItem
    static IProjTreeItem* GetItem(const wxDataViewItem& item);

    /* wxDataViewModel overrides */
    wxDataViewItem GetParent(const wxDataViewItem& item) const wxOVERRIDE;
    unsigned int GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const wxOVERRIDE;
};

#endif // _CBASETREEITEMMODEL_H_
