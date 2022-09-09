#ifndef _CTREEBROWSERMODEL_H_
#define _CTREEBROWSERMODEL_H_

#include <wx/event.h>
#include <wx/dataview.h>
#include <map>

class IProjTreeItem;

//! Fired by the model when the item has been renamed on the tree itself
wxDECLARE_EVENT(EVT_TREE_ITEM_RENAME, wxCommandEvent);

//! @brief Model to connect the GUI to the backend project structure
class CTreeBrowserModel : public wxEvtHandler, public wxDataViewModel
{
public:
    CTreeBrowserModel();
    ~CTreeBrowserModel();

    /* Model logic */
    void GetNameAndIcon(wxVariant&, const IProjTreeItem*) const;
    bool SetName(const wxVariant&, IProjTreeItem*);
    void GetDescription(wxVariant& value, const IProjTreeItem* pItem) const;

    /* wxDataViewModel overrides */
    unsigned int GetColumnCount() const wxOVERRIDE;
    bool HasValue(const wxDataViewItem& item, unsigned col) const wxOVERRIDE;
    wxString GetColumnType(unsigned int col) const wxOVERRIDE;
    void GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const wxOVERRIDE;
    bool SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col) wxOVERRIDE;
    bool IsEnabled(const wxDataViewItem& item, unsigned int col) const wxOVERRIDE;
    wxDataViewItem GetParent(const wxDataViewItem& item) const wxOVERRIDE;
    bool IsContainer(const wxDataViewItem& item) const wxOVERRIDE;
    unsigned int GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const wxOVERRIDE;
};

#endif