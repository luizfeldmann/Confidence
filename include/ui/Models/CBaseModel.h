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

    //! @brief Mutator write the data on a cell of this column
    virtual bool SetValue(const wxVariant&, const wxDataViewItem&) = 0;
};

//! @brief Basic implementation of wxDataViewModel that delegates column access to callbacks
class CBaseModel : public wxDataViewModel
{
protected:
    // @brief Reads the callbacks for a requested column
    virtual IModelColumnHandler* GetColumnInfo(unsigned int nModelColumn) = 0;

    // @copydoc GetColumnInfo
    virtual const IModelColumnHandler* GetColumnInfo(unsigned int nModelColumn) const = 0;

public:
    CBaseModel();
    virtual ~CBaseModel();

    /* wxDataViewModel overrides */
    virtual bool HasValue(const wxDataViewItem& item, unsigned col) const wxOVERRIDE;
    virtual wxString GetColumnType(unsigned int col) const wxOVERRIDE;
    virtual void GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const wxOVERRIDE;
    virtual bool SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col) wxOVERRIDE;
    virtual bool IsEnabled(const wxDataViewItem& item, unsigned int col) const wxOVERRIDE;
};

#endif