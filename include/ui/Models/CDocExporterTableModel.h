#ifndef _CDOCEXPORTERTABLEMODEL_H_
#define _CDOCEXPORTERTABLEMODEL_H_

#include "ui/Models/CBaseModelMapped.h"
#include "docs/EDocExporter.h"
#include <unordered_map>

class CProject;
class IDocExporter;

//! @brief Manages a table of documentation exporters for the project
class CDocExporterTableModel : public CBaseModelMapped
{
protected:
    //! @brief The underlying project being edited
    CProject& m_rEdit;

    //! @brief Defines the columns shown in the table
    enum class EColumn : unsigned int
    {
        Type = 0, //!< Display the type of exporter
        Destination,     //!< The path to export the document

        /* MUST BE LAST */
        Count
    };

public:
    CDocExporterTableModel(CProject& rEdit);
    ~CDocExporterTableModel();

    /* Public interface */

    //! @brief Casts the DataViewItem into a pointer to the table item
    static IDocExporter* GetPointer(const wxDataViewItem& item);

    //! @brief Casts a table item pointer into a DataViewItem
    static wxDataViewItem GetViewItem(const IDocExporter* pItem);

    //! @brief Creates a new exporter in the list
    //! @returns Handle to the new item
    const wxDataViewItem NewItem(EDocExporter eType);

    //! @brief Deletes the item from the list
    //! @returns True if success
    bool DeleteItem(const wxDataViewItem&);

    /** @name wxDataViewModel overrides */
    //!@{
    
    //! @copydoc CBaseModel::GetColumnCount
    unsigned int GetColumnCount() const wxOVERRIDE;

    //! @copydoc CBaseModel::GetParent
    wxDataViewItem GetParent(const wxDataViewItem& item) const wxOVERRIDE;
    
    //! @copydoc CBaseModel::IsContainer
    bool IsContainer(const wxDataViewItem& item) const wxOVERRIDE;

    //! @copydoc CBaseModel::GetChildren
    unsigned int GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const wxOVERRIDE;

    //!@}
};

#endif