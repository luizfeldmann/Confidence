/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#ifndef _CCONFIGURATIONSUMMARYMODEL_H_
#define _CCONFIGURATIONSUMMARYMODEL_H_

#include <wx/dataview.h>
#include <memory>
#include "core/INotifyItemOperation.h"
#include "core/CParentalMap.h"

class CConfiguration;
class CProject;
class CVariable;

//! @brief Table with #CConfiguration as rows, #CInstance as columns, #CVariable or #CConstant as cells
class CConfigurationSummaryModel : public wxDataViewModel, public CParentalMap, public INotifyItemOperation
{
protected:
    //! The configuration being edited
    CConfiguration& m_rEdit;

    //! @brief Pointer to the project
    //! @details This is required to query for all instances and variables
    std::weak_ptr<const CProject> m_pProj;

    //! The data view control using this model
    wxDataViewCtrl* const m_pCtrl;

public:
    //! @brief Constructs a table model associated to the specified configuration and data view control
    //! @param[in] rConfig @copybrief m_rEdit
    //! @param[in] pProj @copybrief m_pProj
    //! @param[in] pCtrl @copybrief m_pCtrl
    CConfigurationSummaryModel(CConfiguration& rConfig, std::shared_ptr<const CProject> pProj, wxDataViewCtrl* pCtrl);

    ~CConfigurationSummaryModel();

    //! @brief Obtains a pointer to a variable from the provided data view item
    static CVariable* GetPointer(const wxDataViewItem& item);

    //! @brief Obtains a data view item from the provided pointer to a variable
    static wxDataViewItem GetViewItem(const CVariable* pItem);

    //! Clears the columns and recreates based on the project's instances
    void ReloadColumns();

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

    //! @copydoc CBaseModel::GetParent
    wxDataViewItem GetParent(const wxDataViewItem& item) const wxOVERRIDE;

    //! @copydoc CBaseModel::GetChildren
    unsigned int GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const wxOVERRIDE;

    //!@}
};

#endif