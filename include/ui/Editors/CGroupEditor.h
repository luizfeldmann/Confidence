#ifndef _CGROUPEDITOR_
#define _CGROUPEDITOR_

#include "ui/Editors/CNameDescEditor.h"
#include "wxExport/IGroupEditor.h"

class CGroup;
class IGroupFilter;

//! @brief Item editor for groups
class CGroupEditorUI : public IGroupEditor, public INotifyItemOperation
{
public:
    CGroupEditorUI(wxWindow* pParent, CGroup& rEdit, std::shared_ptr<const CProject> pProject);

    //! @copydoc INotifyItemOperation::OnItemCreated
    void OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent) override;

    //! @copydoc INotifyItemOperation::OnAnyItemErased
    void OnAnyItemErased(const IProjTreeItem& rItem) override;

    //! @copydoc INotifyItemOperation::OnAnyItemRenamed
    void OnAnyItemRenamed(const IProjTreeItem& rChanged) override;

protected:
    //! Reference to the edit group
    CGroup& m_rEdit;

    //! Reference to the current project
    std::shared_ptr<const CProject> const m_pProject;

    //! @brief Convergence of all INotifyItemOperation events
    void OnItemOperation(const IProjTreeItem& rItem);

    //! @brief Updates UI to reflect changes in the current filter type
    void ReloadFilterType();

    //! @brief Reloads specific info of each filter type
    void ReloadFilterDetails(std::shared_ptr<const IGroupFilter> pFilter);

    //! @brief Updates the list of filtered instances
    void PopulateInstancesList(std::shared_ptr<const IGroupFilter> pFilter);

    //! @brief Updates the list of filtered configurations 
    void PopulateConfigurationsList(std::shared_ptr<const IGroupFilter> pFilter);

    //! @brief Appends an item to the provided filter list, and (un)-checks it according to the filter
    void AppendFilterListItem(std::shared_ptr<const IGroupFilter> pFilter, IProjTreeItem::cptr_t pItem, wxCheckListBox* pListBox);

    void OnListToggle(wxCommandEvent& event, wxCheckListBox* pListBox);

    /*OVERRIDES FROM IGroupEditor */
    void onChangeFilter(wxCommandEvent& event) override;
    void onConfigListToggle(wxCommandEvent& event) override;
    void onInstanceListToggle(wxCommandEvent& event) override;
};

//! @copydoc CGroupEditorUI
class CGroupEditor : public CNameDescEditor
{
protected:
    CGroupEditorUI* const m_pUiGroup;

public:
    CGroupEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @brief Returns a new instance of this class
    static std::unique_ptr<ITreeItemEditor> Create(CMainWindow& rMainWindow, IProjTreeItem& rItem);

    //! @copydoc INotifyItemOperation::OnItemCreated
    void OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent) override;

    //! @copydoc INotifyItemOperation::OnAnyItemErased
    void OnAnyItemErased(const IProjTreeItem& rItem) override;

    //! @copydoc INotifyItemOperation::OnAnyItemRenamed
    void OnAnyItemRenamed(const IProjTreeItem& rChanged) override;
};

#endif