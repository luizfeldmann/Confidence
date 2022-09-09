#ifndef _CMAINWINDOW_H_
#define _CMAINWINDOW_H_

#include "wxExport/IMainWindow.h"
#include "ui/CTreeItemEditorManager.h"
#include "ui/CNameDescEditor.h"

class CMainWindow : public IMainWindow
{
public:
    CMainWindow();
    ~CMainWindow();

protected:
	//! Manages the tabs on the editor tab view
	CTreeItemEditorManager m_editorManager;
	friend class ITreeItemEditor;

	//! Stores the item in clipboard, which was "cut" and is waiting to be "pasted"
	ITreeItemCollection::PtrItem m_pCutClipboard;

	//! @brief Updates all the interface whenever a new project is loaded
	void ReloadProject();

	//! @brief Get pointer to the selected item in project tree
	IProjTreeItem* GetSelectedTreeItem() const;

	//! @brief Get parent of the provided item from the project tree
	IProjTreeItem* GetTreeItemParent(IProjTreeItem*) const;

	//! @brief Possible edit operations on project tree items
	enum class EItemOperation {
		MoveUp,
		MoveDown,
		Delete,
		Cut
	};

	//! @brief Performs the requested operation on the currently selected project tree item
	void OnItemOperation(EItemOperation eOper);

	//! @brief When the user clicked the popup menu of new item creation
	void onBtnNewItemMenu(wxCommandEvent& evt);

	//! @brief When the tree item has been renamed on the tree itself
	void onTreeItemRenamed(wxCommandEvent& evt);

	//! @brief When an item's name or description was changed on the editor
	void onEditorItemNameDescChanged(wxCommandEvent& evt);

	/* Auto generated GUI event handlers */
	void onBtnNewProject(wxCommandEvent& event) override;
	void onBtnOpenProject(wxCommandEvent& event) override;
	void onBtnSaveProject(wxCommandEvent& event) override;
	void onBtnSaveAsProject(wxCommandEvent& event) override;
	void onBtnRunProject(wxCommandEvent& event) override;
	void onBtnStopProject(wxCommandEvent& event) override;
	void onBtnNewItem(wxCommandEvent& event) override;
	void onBtnDeleteItem(wxCommandEvent& event) override;
	void onBtnItemUp(wxCommandEvent& event) override;
	void onBtnItemDown(wxCommandEvent& event) override;
	void onBtnItemCut(wxCommandEvent& event) override;
	void onBtnItemPaste(wxCommandEvent& event) override;
	void onToolViewDesc(wxCommandEvent& event) override;
	void onTreeItemActivate(wxDataViewEvent& event) override;
};

#endif