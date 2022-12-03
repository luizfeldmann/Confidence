/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CPROCESSARGSMODEL_H_
#define _CPROCESSARGSMODEL_H_

#include "ui/Models/CBaseModelMapped.h"
#include <unordered_map>

class CRunProcess;
class CProcessArgument;

//! @brief Manages the table of arguments of a process starter
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
    //! @brief Constructs a table model to view/edit the arguments of a #CRunProcess
    //! @param[in] rEdit @copybrief m_rEdit
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