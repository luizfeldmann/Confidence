/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CVARIABLETABLEMODEL_H_
#define _CVARIABLETABLEMODEL_H_

#include "CBaseTreeItemModel.h"
#include "core/INotifyItemOperation.h"

class CVariable;
class CProject;

//! @brief Table with #CConfiguration as rows and #CInstance as columns
class CVariableTableModel : public CBaseTreeItemModel, public INotifyItemOperation
{
protected:
    //! The variable being edited
    CVariable& m_rVar;

    //! @brief Pointer to the project
    //! @details This is required to query for all instances and configurations
    std::weak_ptr<const CProject> m_pProj;

    //! The data view control using this model
    wxDataViewCtrl* const m_pCtrl;

    //! @copydoc CBaseTreeItemModel::GetRootItem
    const IProjTreeItem& GetRootItem() const override;

public:
    //! @brief Constructs a table model to view/edit the rules defining a particular #CVariable
    //! @param[in] rVar @copybrief m_rVar
    //! @param[in] pProj @copybrief m_pProj
    //! @param[in] pCtrl @copybrief m_pCtrl
    CVariableTableModel(CVariable& rVar, std::shared_ptr<const CProject> pProj, wxDataViewCtrl* pCtrl);

    ~CVariableTableModel();

    //! Clears the columns and recreates based on the project's instances
    void ReloadColumns();

    //! @brief Deletes the expression in the provided cell
    //! @return True if success
    bool DeleteCell(const wxDataViewItem& rCellRow, const wxDataViewColumn* pCellColumn);

    //! @copydoc INotifyItemOperation::OnItemCreated
    void OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent) override;

    //! @copydoc INotifyItemOperation::OnAnyItemErased
    void OnAnyItemErased(const IProjTreeItem& rItem) override;

    //! @copydoc INotifyItemOperation::OnAnyItemRenamed
    void OnAnyItemRenamed(const IProjTreeItem& rItem) override;

    /** @name wxDataViewModel overrides */
    //!@{

    //! @copydoc CBaseModel::GetColumnCount
    unsigned int GetColumnCount() const wxOVERRIDE;
    
    //! @copydoc CBaseModel::HasValue
    bool HasValue(const wxDataViewItem& item, unsigned col) const wxOVERRIDE;
    
    //! @copydoc CBaseModel::GetColumnType
    wxString GetColumnType(unsigned int col) const wxOVERRIDE;
    
    //! @copydoc CBaseModel::GetValue
    void GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const wxOVERRIDE;
    
    //! @copydoc CBaseModel::SetValue
    bool SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col) wxOVERRIDE;
    
    //! @copydoc CBaseModel::IsEnabled
    bool IsEnabled(const wxDataViewItem& item, unsigned int col) const wxOVERRIDE;
    
    //! @copydoc CBaseModel::IsContainer
    bool IsContainer(const wxDataViewItem& item) const wxOVERRIDE;

    //!@}
};

#endif