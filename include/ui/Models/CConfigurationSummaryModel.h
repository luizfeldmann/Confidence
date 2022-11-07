#ifndef _CCONFIGURATIONSUMMARYMODEL_H_
#define _CCONFIGURATIONSUMMARYMODEL_H_

#include <wx/dataview.h>
#include <memory>
#include "core/INotifyItemOperation.h"

class CConfiguration;
class CProject;

//! @brief Table with configurations as rows and instances as columns
class CConfigurationSummaryModel : public wxDataViewModel, public INotifyItemOperation
{
protected:
    //! The configuration being edited
    CConfiguration& m_rEdit;

    //! Pointer to the project
    //! This is required to query for all instances and variables
    std::weak_ptr<const CProject> m_pProj;

    //! The data view control using this model
    wxDataViewCtrl* const m_pCtrl;

public:
    CConfigurationSummaryModel(CConfiguration& rConfig, std::shared_ptr<const CProject> pProj, wxDataViewCtrl* pCtrl);
    ~CConfigurationSummaryModel();

    //! Clears the columns and recreates based on the project's instances
    void ReloadColumns();

    //! @copydoc INotifyItemOperation::OnItemCreated
    void OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent) override;

    //! @copydoc INotifyItemOperation::OnAnyItemErased
    void OnAnyItemErased(const IProjTreeItem& rItem) override;

    //! @copydoc INotifyItemOperation::OnAnyItemRenamed
    void OnAnyItemRenamed(const IProjTreeItem& rItem) override;

    /* wxDataViewModel overrides */
    unsigned int GetColumnCount() const wxOVERRIDE;
    bool HasValue(const wxDataViewItem& item, unsigned col) const wxOVERRIDE;
    wxString GetColumnType(unsigned int col) const wxOVERRIDE;
    void GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const wxOVERRIDE;
    bool SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col) wxOVERRIDE;
    bool IsEnabled(const wxDataViewItem& item, unsigned int col) const wxOVERRIDE;
    bool IsContainer(const wxDataViewItem& item) const wxOVERRIDE;
    wxDataViewItem GetParent(const wxDataViewItem& item) const wxOVERRIDE;
    unsigned int GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const wxOVERRIDE;
};

#endif