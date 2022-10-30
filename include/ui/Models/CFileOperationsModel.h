#ifndef _CFILEOPERATIONSMODEL_H_
#define _CFILEOPERATIONSMODEL_H_

#include "ui/Models/CBaseModel.h"
#include <unordered_map>

class CFileOperations;
class CFileOperation;

//! @brief Model to connect the GUI to the backend project structure
class CFileOperationsModel : public CBaseModel
{
protected:
    //! @brief The underlying operations being edited
    CFileOperations& m_rEdit;

    //! @brief Defines the columns shown in the operations list
    enum class EColumn
    {
        Direction = 0,  //!< If the operation is R or W
        Location,       //!< String representation of the data locator
        Expression,     //!< Data to write or variable to store on read
        Description,    //!< Description of the operation

        /* MUST BE LAST */
        Count
    };

    //! Stores the callbacks to read/write each column
    std::unordered_map<EColumn, std::unique_ptr<IModelColumnHandler>> m_mapColumns;

    //! @copydoc CBaseModel::GetColumnInfo
    IModelColumnHandler* GetColumnInfo(unsigned int nModelColumn) override;

    //! @copydoc CBaseModel::GetColumnInfo
    const IModelColumnHandler* GetColumnInfo(unsigned int nModelColumn) const override;

public:
    CFileOperationsModel(CFileOperations& rEdit);
    ~CFileOperationsModel();

    /* Public interface */

    //! @brief Casts the DataViewItem into an operation pointer
    static CFileOperation* GetPointer(const wxDataViewItem& item);

    //! @brief Casts an operation pointer into a DataViewItem
    static const wxDataViewItem GetViewItem(const CFileOperation* pOper);

    //! @brief Creates a new operation item
    //! @returns Handle to the new item
    const wxDataViewItem NewItem();

    //! @brief Deletes the item from the list
    //! @returns True if success
    bool DeleteItem(const wxDataViewItem&);

    //! @brief Moves the given item up or down in the list
    //! @returns Handle to the swapped item
    const wxDataViewItem MoveItem(const wxDataViewItem& rMoveItem, bool bUp);

    /* wxDataViewModel overrides */
    unsigned int GetColumnCount() const wxOVERRIDE;
    wxDataViewItem GetParent(const wxDataViewItem& item) const wxOVERRIDE;
    bool IsContainer(const wxDataViewItem& item) const wxOVERRIDE;
    unsigned int GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const wxOVERRIDE;
};

#endif