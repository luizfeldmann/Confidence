#ifndef _CPROCESSARGSMODEL_H_
#define _CPROCESSARGSMODEL_H_

#include "ui/Models/CBaseModelMapped.h"
#include <unordered_map>

class CRunProcess;
class CProcessArgument;

//! @brief Model to connect the GUI to the backend project structure
class CProcessArgsModel : public CBaseModelMapped
{
protected:
    //! @brief The underlying process being edited
    CRunProcess& m_rEdit;

    //! @brief Defines the columns shown in the arguments list
    enum class EColumn : unsigned int
    {
        Description = 0,
        Expression,

        /* MUST BE LAST */
        Count
    };

public:
    CProcessArgsModel(CRunProcess& rEdit);
    ~CProcessArgsModel();

    /* Public interface */

    //! @brief Casts the DataViewItem into a process argument pointer
    static CProcessArgument* GetPointer(const wxDataViewItem& item);

    //! @brief Casts a process argument pointer into a DataViewItem
    static const wxDataViewItem GetViewItem(const CProcessArgument* pArg);

    //! @brief Creates a new argument item
    //! @returns Handle to the new item
    const wxDataViewItem NewItem();

    //! @brief Deletes the argument item from the list
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