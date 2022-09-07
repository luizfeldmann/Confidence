#ifndef _CTREEBROWSERMODEL_H_
#define _CTREEBROWSERMODEL_H_

#include <wx/dataview.h>
#include <map>

class IProjTreeItem;

//! @brief Model to connect the GUI to the backend project structure
class CTreeBrowserModel : public wxDataViewModel
{
public:
    CTreeBrowserModel();
    ~CTreeBrowserModel();

    /* Model logic */
    void GetNameAndIcon(wxVariant&, const IProjTreeItem*) const;
    bool SetName(const wxVariant&, IProjTreeItem*);

    /* wxDataViewModel overrides */
    unsigned int GetColumnCount() const wxOVERRIDE;
    wxString GetColumnType(unsigned int col) const wxOVERRIDE;
    void GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const wxOVERRIDE;
    bool SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col) wxOVERRIDE;
    bool IsEnabled(const wxDataViewItem& item, unsigned int col) const wxOVERRIDE;
    wxDataViewItem GetParent(const wxDataViewItem& item) const wxOVERRIDE;
    bool IsContainer(const wxDataViewItem& item) const wxOVERRIDE;
    unsigned int GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const wxOVERRIDE;
};

#endif