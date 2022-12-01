#ifndef _CMAINWINDOW_H_
#define _CMAINWINDOW_H_

#include "ui/CAcceleratorEntry.h"
#include "wxExport/IMainWindow.h"
#include "ui/CTreeItemEditorManager.h"
#include "ui/Editors/CNameDescEditor.h"
#include "core/ITreeItemCollection.h"

class CProject;
class CTreeBrowserModel;

//! @brief The top level window of the application: the project editor.
class CMainWindow : public IMainWindow
{
public:
	//! @brief Creates an application window editing the provided project
	//! @param[in] pProject Pointer to the working project
    CMainWindow(std::shared_ptr<CProject> pProject);

    ~CMainWindow();

	//! @brief Gets a pointer to the project being edited
	std::shared_ptr<const CProject> GetProject() const;

	//! @brief Creates a tab for the item, or focuses it if already exists
	void ActivateItem(IProjTreeItem& rEditItem);

protected:
	//! Holds the project being edited
	std::shared_ptr<CProject> m_pProject;

	//! Model managing the project editor tree
	CTreeBrowserModel* const m_pTreeModel;

	//! Manages shortcuts
	wxAcceleratorTable m_cAccTbl;

	//! Manages the tabs on the editor tab view
	CTreeItemEditorManager m_editorManager;
	friend class ITreeItemEditor;

	//! Stores the item in clipboard, which was "cut" and is waiting to be "pasted"
	ITreeItemCollection::ptr_t m_pCutClipboard;

	//! @brief Updates all the interface whenever a new project is loaded
	void ReloadProject();

	//! @brief Performs the requested move operation on the currently selected project tree item
	bool OnItemMove(bool bUp);

	//! @brief When the user clicked the popup menu of new item creation
	void onBtnNewItemMenu(wxCommandEvent& evt);

	//! @brief When the tree item has been renamed on the tree itself
	void onTreeItemRenamed(wxCommandEvent& evt);

	//! @brief When an item's name or description was changed on the editor
	void onEditorItemNameDescChanged(wxCommandEvent& evt);

	//! @brief Called when the user choses a configuration to run on the popup menu
	void onBtnRunSelected(wxCommandEvent& evt);

	//! @brief Enables/disables UI elements according to the run state of the project
	//! @param[in] bRun True if the project is currently running
	void SetRunMode(bool bRun);

	//! @name Overrides from IMainWindow
	//!@{
	
	//! @brief Starts a new blank project
	void onBtnNewProject(wxCommandEvent& event) override;
	
	//! @brief Shows a dialog to select and open a project file 
	void onBtnOpenProject(wxCommandEvent& event) override;
	
	//! @brief Saves the project to the current path
	void onBtnSaveProject(wxCommandEvent& event) override;
	
	//! @brief Shows a dialog to select and save a project file
	void onBtnSaveAsProject(wxCommandEvent& event) override;
	
	//! @brief Exports the documentation of the project
	void onBtnDocumentation(wxCommandEvent& event) override;
	
	//! @brief Executes the project
	void onBtnRunProject(wxCommandEvent& event) override;
	
	//! @brief Stops the current project execution
	void onBtnStopProject(wxCommandEvent& event) override;
	
	//! @brief Shows a pop-up menu with options of new items to create
	void onBtnNewItem(wxCommandEvent& event) override;
	
	//! @brief Deletes the selected tree item
	void onBtnDeleteItem(wxCommandEvent& event) override;
	
	//! @brief Move up the selected tree item
	void onBtnItemUp(wxCommandEvent& event) override;
	
	//! @brief Move down the selected tree item
	void onBtnItemDown(wxCommandEvent& event) override;
	
	//! @brief Cuts the selected tree item
	void onBtnItemCut(wxCommandEvent& event) override;
	
	//! @brief Pastes the selected tree item
	void onBtnItemPaste(wxCommandEvent& event) override;
	
	//! @brief Toggles show/hide description in the tree view
	void onToolViewDesc(wxCommandEvent& event) override;
	
	//! @brief Opens a tab to edit a project item
	void onTreeItemActivate(wxDataViewEvent& event) override;
	
	//! @brief Opens an HTML link in the default browser
	void onClickHtmlLink(wxHtmlLinkEvent& event) override;
	//!@}
};

#endif