#ifndef _CBASEMODEL_H_
#define _CBASEMODEL_H_

#include <wx/dataview.h>

//! @brief Interface used to read/write data on each of a model's columns
class IModelColumnHandler
{
public:
    IModelColumnHandler() = default;
    virtual ~IModelColumnHandler() = default;

    //! @brief Gets type of renderer used to display the cell
    virtual wxString GetType() const = 0;

    //! @brief Acessor to retrieve the data on a cell of this column
    virtual void GetValue(wxVariant&, const wxDataViewItem&) const = 0;

    //! @brief Checks if a given cell is enabled
    virtual bool IsEnabled(const wxDataViewItem&) const = 0;

    //! @brief Checks if a given cell has a value
    virtual bool HasValue(const wxDataViewItem&) const;

    //! @brief Mutator write the data on a cell of this column
    virtual bool SetValue(const wxVariant&, const wxDataViewItem&) = 0;
};

//! @brief Basic implementation of wxDataViewModel that delegates column access to callbacks
class CBaseModel : public virtual wxDataViewModel
{
protected:
    //! @brief Reads the callbacks for a requested column
    virtual IModelColumnHandler* GetColumnInfo(unsigned int nModelColumn) = 0;

    //! @copydoc GetColumnInfo
    virtual const IModelColumnHandler* GetColumnInfo(unsigned int nModelColumn) const = 0;

public:
    /** @name wxDataViewModel overrides */
    //!@{
    
    //! @copybrief IModelColumnHandler::HasValue
    bool HasValue(const wxDataViewItem& item, unsigned col) const wxOVERRIDE;
    
    //! @copybrief IModelColumnHandler::GetType
    wxString GetColumnType(unsigned int col) const wxOVERRIDE;
    
    //! @copybrief IModelColumnHandler::GetValue
    void GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const wxOVERRIDE;
    
    //! @copybrief IModelColumnHandler::SetValue
    bool SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col) wxOVERRIDE;
    
    //! @copybrief IModelColumnHandler::IsEnabled
    bool IsEnabled(const wxDataViewItem& item, unsigned int col) const wxOVERRIDE;

    //! @brief Gets the number of columns of this model
    virtual unsigned int GetColumnCount() const wxOVERRIDE = 0;

    //! @brief Gets the parent item of the requested child item
    //virtual wxDataViewItem GetParent(const wxDataViewItem& item) const wxOVERRIDE = 0;

    //! @brief Checks if an item can have children
    virtual bool IsContainer(const wxDataViewItem& item) const wxOVERRIDE = 0;

    //! @brief Fills the array with the child items of the requested parent
    //virtual unsigned int GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const wxOVERRIDE = 0;

    //!@}
};

#endif