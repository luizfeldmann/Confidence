#ifndef _CVARIABLETABLEMODEL_H_
#define _CVARIABLETABLEMODEL_H_

#include "CBaseTreeItemModel.h"

class CVariable;
class CProject;

//! @brief Table with configurations as rows and instances as columns
class CVariableTableModel : public CBaseTreeItemModel
{
protected:
    //! The variable being edited
    CVariable& m_rVar;

    //! Reference to the project
    //! This is required to query for all instances and configurations
    const CProject& m_rProj;

    //! The data view control using this model
    wxDataViewCtrl* const m_pCtrl;

    //! Clears the columns and recreates based on the project's instances
    void ReloadColumns();

    //! @copydoc CBaseTreeItemModel::GetRootItem
    const IProjTreeItem& GetRootItem() const override;

public:
    CVariableTableModel(CVariable& rVar, const CProject& rProj, wxDataViewCtrl* pCtrl);
    ~CVariableTableModel();

    //! @brief Deletes the expression in the provided cell
    //! @return True if success
    bool DeleteCell(const wxDataViewItem& rCellRow, const wxDataViewColumn* pCellColumn);

    /* wxDataViewModel overrides */
    unsigned int GetColumnCount() const wxOVERRIDE;
    bool HasValue(const wxDataViewItem& item, unsigned col) const wxOVERRIDE;
    wxString GetColumnType(unsigned int col) const wxOVERRIDE;
    void GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const wxOVERRIDE;
    bool SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col) wxOVERRIDE;
    bool IsEnabled(const wxDataViewItem& item, unsigned int col) const wxOVERRIDE;
    bool IsContainer(const wxDataViewItem& item) const wxOVERRIDE;
};

#endif