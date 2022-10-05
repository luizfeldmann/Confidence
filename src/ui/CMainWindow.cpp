#include "ui/CMainWindow.h"
#include "ui/Models/CTreeBrowserModel.h"
#include "ui/STreeItemTypeInfo.h"
#include "util/Log.h"
#include <wx/filedlg.h>
#include <wx/menu.h>
#include "core/CProject.h"
#include <array>

const static wxString g_szProjectFileFilter = "Confidence projects (*.cfx)|*.cfx";

CMainWindow::CMainWindow(CProject& rProject)
    : IMainWindow(nullptr)
    , m_rProject(rProject)
    , m_pTreeModel( new CTreeBrowserModel(m_rProject) )
    , m_editorManager(*m_notebookEditor)
{
    // Let the logger know we are using GUI
    CLog::m_bShowMessageBox = true;

    // Configure the main window's icon
    SetIcon(wxIcon("RES_ID_ICON_APPLICATION"));

    // Configure the project tree view
    m_pTreeModel->Bind(EVT_TREE_ITEM_RENAME, &CMainWindow::onTreeItemRenamed, this);
    m_dataViewCtrlBrowser->AssociateModel(m_pTreeModel);
    m_pTreeModel->DecRef();
    m_dataViewCtrlBrowser->ExpandChildren(m_dataViewCtrlBrowser->GetTopItem());

    // Configure the editor tabs
    m_notebookEditor->Bind(EVT_EDITOR_ITEM_CHANGED_NAME, &CMainWindow::onEditorItemNameDescChanged, this);
    m_notebookEditor->Bind(EVT_EDITOR_ITEM_CHANGED_DESC, &CMainWindow::onEditorItemNameDescChanged, this);

    // Build accelerator table
    using acc_entry_t = CAcceleratorEntry;
    const std::array<acc_entry_t, 11> arrAccEntry {
        // Main menu
        acc_entry_t(wxACCEL_CTRL,    (int)'O',   m_toolOpenProject),
        acc_entry_t(wxACCEL_CTRL,    (int)'S',   m_toolSaveProject),
        acc_entry_t(wxACCEL_CTRL |
                     wxACCEL_SHIFT,  (int)'S',   m_toolSaveAsProject),
        acc_entry_t(wxACCEL_NORMAL,  WXK_F5,     m_toolRunProject),
        acc_entry_t(wxACCEL_NORMAL,  WXK_ESCAPE, m_toolStopProject),
        // Tree browser
        acc_entry_t(wxACCEL_CTRL,    (int)'N',   m_toolNewItem),
        acc_entry_t(wxACCEL_NORMAL,  WXK_DELETE, m_toolDeleteItem),
        acc_entry_t(wxACCEL_CTRL,    WXK_UP,     m_toolMoveUpItem),
        acc_entry_t(wxACCEL_CTRL,    WXK_DOWN,   m_toolMoveDownItem),
        acc_entry_t(wxACCEL_CTRL,    (int)'X',   m_toolCutItem),
        acc_entry_t(wxACCEL_CTRL,    (int)'V',   m_toolPasteItem),
    };

    m_cAccTbl = wxAcceleratorTable(arrAccEntry.size(), arrAccEntry.data());
    SetAcceleratorTable(m_cAccTbl);

    // Load the current project
    ReloadProject();
}

CMainWindow::~CMainWindow()
{
    CLog::m_bShowMessageBox = false;
}

/// =======================================================
/// Top toolbar
/// =======================================================

void CMainWindow::onBtnNewProject(wxCommandEvent& event)
{
    m_rProject = std::move( CProject() );
    ReloadProject();
}

void CMainWindow::onBtnOpenProject(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString,
            g_szProjectFileFilter, wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (wxID_CANCEL == openFileDialog.ShowModal())
        return;

    CProject openProj;
    if (openProj.OpenFile(openFileDialog.GetPath().ToStdString()))
    {
        m_rProject = std::move( openProj );
        ReloadProject();
    }
}

void CMainWindow::onBtnSaveProject(wxCommandEvent& event)
{
    const std::string& szCurrentPath = m_rProject.GetCurrentPath();
    if (szCurrentPath.empty())
    {
        onBtnSaveAsProject(event);
    }
    else
    {
        m_rProject.SaveToFile(szCurrentPath);
    }
}

void CMainWindow::onBtnSaveAsProject(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString,
        g_szProjectFileFilter, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (wxID_CANCEL == saveFileDialog.ShowModal())
        return;

    m_rProject.SaveToFile(saveFileDialog.GetPath().ToStdString());
}

void CMainWindow::onBtnDocumentation(wxCommandEvent& event)
{
    m_rProject.ExportDocumentation();
}

void CMainWindow::onBtnRunProject(wxCommandEvent& event)
{

}

void CMainWindow::onBtnStopProject(wxCommandEvent& event)
{

}

/// =======================================================
/// Tree browser
/// =======================================================

void CMainWindow::onTreeItemRenamed(wxCommandEvent& evt)
{
    IProjTreeItem* pItem = static_cast<IProjTreeItem*>( evt.GetClientData() );
    assert(nullptr != pItem);

    m_editorManager.OnAnyItemRenamed(*pItem);
}

void CMainWindow::onEditorItemNameDescChanged(wxCommandEvent& evt)
{
    IProjTreeItem* pItem = static_cast<IProjTreeItem*>(evt.GetClientData());
    assert(nullptr != pItem);

    m_pTreeModel->ItemChanged(CBaseTreeItemModel::GetViewItem(pItem));

    m_editorManager.OnAnyItemRenamed(*pItem);
}

void CMainWindow::onTreeItemActivate(wxDataViewEvent& event)
{
    IProjTreeItem* pSelected = CTreeBrowserModel::GetPointer(m_dataViewCtrlBrowser->GetSelection());

    if (nullptr != pSelected)
        m_editorManager.ActivateItem(*pSelected);
}

void CMainWindow::onBtnNewItemMenu(wxCommandEvent& evt)
{
    // Get parent collection of subitems
    const wxDataViewItem cSelected = m_dataViewCtrlBrowser->GetSelection();
    IProjTreeItem* const pSelected = CTreeBrowserModel::GetPointer(cSelected);
    assert((nullptr != pSelected) && "Parent item must be selected before creating it's child");

    // Get information about the class of subitem we wish to append
    const ETreeItemType eType = (ETreeItemType)evt.GetId();
    const STreeItemTypeInfo& rInfo = STreeItemTypeInfo::GetInfo(eType);

    assert((nullptr != rInfo.m_fnFactory) && "New item type does not have a factory function");

    // Create new child and add to collection
    IProjTreeItem* pNewChild = rInfo.m_fnFactory();
    assert((nullptr != pNewChild) && "Factory failed to create instance of new item");

    if (pSelected->AddItem(pNewChild))
    {
        wxDataViewItem newItem = CBaseTreeItemModel::GetViewItem(pNewChild);

        m_pTreeModel->ItemAdded(cSelected, newItem);

        m_dataViewCtrlBrowser->Expand(cSelected);
        m_dataViewCtrlBrowser->SetSelections(wxDataViewItemArray(1, newItem));

        m_editorManager.OnItemCreated(*pNewChild, *pSelected);
    }
    else
    {
        delete pNewChild;
        pNewChild = nullptr;

        wxScopedCharBuffer wxTypeName = rInfo.m_strTypeName.utf8_str();
        CERROR("Failed adding item '%s' to collection", wxTypeName.data());
    }
}

void CMainWindow::onBtnNewItem(wxCommandEvent& event)
{
    IProjTreeItem* pSelected = CTreeBrowserModel::GetPointer(m_dataViewCtrlBrowser->GetSelection());

    // You may only add new items inside an existing parent
    if (nullptr == pSelected)
        return;

    // Populate the menu with a list of all possible children types
    const ETreeItemType eSupported = pSelected->GetSupportedChildTypes();

    wxMenu menu;
    menu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CMainWindow::onBtnNewItemMenu), NULL, this);
    
    for (ETreeItemType eType : STreeItemTypeInfo::GetTypesList())
    {
        if (0 == (eType & eSupported))
            continue;

        const STreeItemTypeInfo& rInfo = STreeItemTypeInfo::GetInfo(eType);

        wxMenuItem* pMenuItem = menu.Append(eType, rInfo.m_strTypeName);
        pMenuItem->SetBitmap(rInfo.m_icon);
    }

    // Display the menu only if there are relevant options
    if (0 < menu.GetMenuItemCount())
        PopupMenu(&menu);
}

bool CMainWindow::OnItemMove(bool bUp)
{
    const wxDataViewItem selectedItem = m_dataViewCtrlBrowser->GetSelection();
    const bool bSuccess = m_pTreeModel->MoveItem(selectedItem, bUp);

    if (bSuccess)
        m_dataViewCtrlBrowser->Select(selectedItem);

    return bSuccess;
}

void CMainWindow::onBtnDeleteItem(wxCommandEvent& event)
{
    const wxDataViewItem selectedItem = m_dataViewCtrlBrowser->GetSelection();
    ITreeItemCollection::ptr_t pTakeItem = m_pTreeModel->TakeItem(selectedItem);

    if (pTakeItem)
        m_editorManager.OnAnyItemErased(*pTakeItem);
}

void CMainWindow::onBtnItemUp(wxCommandEvent& event)
{
    OnItemMove(true);
}

void CMainWindow::onBtnItemDown(wxCommandEvent& event)
{
    OnItemMove(false);
}

void CMainWindow::onBtnItemCut(wxCommandEvent& event)
{
    const wxDataViewItem selectedItem = m_dataViewCtrlBrowser->GetSelection();
    ITreeItemCollection::ptr_t pTakeItem = m_pTreeModel->TakeItem(selectedItem);

    if (pTakeItem)
    {
        m_editorManager.OnAnyItemErased(*pTakeItem);
        m_pCutClipboard = std::move(pTakeItem);
    }
}

void CMainWindow::onBtnItemPaste(wxCommandEvent& event)
{
    if (m_pCutClipboard)
    {
        const wxDataViewItem parentItem = m_dataViewCtrlBrowser->GetSelection();

        if (m_pTreeModel->InsertItem(parentItem, m_pCutClipboard.get()))
        {
            wxDataViewItem pasteItem( CBaseTreeItemModel::GetViewItem(m_pCutClipboard.get()) );

            m_dataViewCtrlBrowser->Expand(parentItem);
            m_dataViewCtrlBrowser->SetSelections(wxDataViewItemArray(1, pasteItem));

            m_editorManager.OnItemCreated(*m_pCutClipboard, *CTreeBrowserModel::GetPointer(parentItem));

            m_pCutClipboard.release();
        }
    }
}

void CMainWindow::onToolViewDesc(wxCommandEvent& event)
{
    const bool bShowDesc = m_toolViewDesc->IsToggled();
    m_dataViewColumnDescription->SetHidden(!bShowDesc);
}

/// =======================================================
/// General
/// =======================================================

void CMainWindow::ReloadProject()
{
    // Close all tabs on the editor notebook
    m_editorManager.Clear();

    // Update tree: delete and re-add the project
    wxDataViewItem root = CBaseTreeItemModel::GetViewItem(nullptr);
    wxDataViewItem project = CBaseTreeItemModel::GetViewItem(&m_rProject);
    m_pTreeModel->ItemDeleted(root, project);
    m_pTreeModel->ItemAdded(root, project);
}