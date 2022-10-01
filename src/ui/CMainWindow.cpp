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
    , m_editorManager(*m_notebookEditor)
{
    // Let the logger know we are using GUI
    CLog::m_bShowMessageBox = true;

    // Configure the main window's icon
    SetIcon(wxIcon("RES_ID_ICON_APPLICATION"));

    // Configure the project tree view
    CTreeBrowserModel* pTreeModel = new CTreeBrowserModel(m_rProject);
    pTreeModel->Bind(EVT_TREE_ITEM_RENAME, &CMainWindow::onTreeItemRenamed, this);
    m_dataViewCtrlBrowser->AssociateModel(pTreeModel);
    pTreeModel->DecRef();
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

    m_editorManager.ItemRenamed(*pItem);
}

void CMainWindow::onEditorItemNameDescChanged(wxCommandEvent& evt)
{
    IProjTreeItem* pItem = static_cast<IProjTreeItem*>(evt.GetClientData());
    assert(nullptr != pItem);

    wxDataViewModel* pModel = m_dataViewCtrlBrowser->GetModel();
    pModel->ItemChanged(wxDataViewItem(pItem));
}

void CMainWindow::onTreeItemActivate(wxDataViewEvent& event)
{
    IProjTreeItem* pSelected = GetSelectedTreeItem();

    if (nullptr != pSelected)
        m_editorManager.ActivateItem(*pSelected);
}

void CMainWindow::onBtnNewItemMenu(wxCommandEvent& evt)
{
    // Get parent collection of subitems
    IProjTreeItem* pSelected = GetSelectedTreeItem();
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
        wxDataViewItem parentItem(pSelected);
        wxDataViewItem newItem(pNewChild);

        m_dataViewCtrlBrowser->GetModel()->ItemAdded(parentItem, newItem);

        m_dataViewCtrlBrowser->Expand(parentItem);
        m_dataViewCtrlBrowser->SetSelections(wxDataViewItemArray(1, newItem));
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
    IProjTreeItem* pSelected = GetSelectedTreeItem();

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
    bool bSuccess = false;

    // Ensure there is a selected item for us to operate
    // If the parent is root, then the project itself is selected
    // We cannot move/cut/delete the project itself
    IProjTreeItem* const pChild = GetSelectedTreeItem();
    IProjTreeItem* const pParent = GetTreeItemParent(pChild);

    if (!pChild || !pParent)
        CWARNING("Cannot move the root item");
    else
    {
        // Find the iterator to this item inside it's parent vector
        IProjTreeItem::vec_ref_t& vSubitems = pParent->SubItems();

        IProjTreeItem::vec_ref_t::iterator iterChild = std::find_if(vSubitems.begin(), vSubitems.end(),
            [pChild](const IProjTreeItem::ref_t pSearch)->bool {
                return (&pSearch.get() == pChild);
            });

        assert(vSubitems.cend() != iterChild);

        IProjTreeItem::vec_ref_t::iterator iterSwap;
        if (bUp)
        {
            iterSwap = (vSubitems.begin() == iterChild)
                ? std::prev(vSubitems.end()) // Move up 1st item = wrap around to the end
                : std::prev(iterChild);
        }
        else
        {
            iterSwap = std::next(iterChild);

            // If move down last item - wrap around to the front
            if (vSubitems.cend() == iterSwap)
                iterSwap = vSubitems.begin();
        }

        if (pParent->SwapItems(*iterSwap, *iterChild))
        {
            wxDataViewModel* pModel = m_dataViewCtrlBrowser->GetModel();
            wxDataViewItem wxParent(pParent), wxChild(pChild);

            pModel->ItemDeleted(wxParent, wxChild);
            pModel->ItemAdded(wxParent, wxChild);
            m_dataViewCtrlBrowser->Select(wxChild);

            bSuccess = true;
        }
    }

    return bSuccess;
}

ITreeItemCollection::ptr_t CMainWindow::TreeTakeSelectedItem()
{
    ITreeItemCollection::ptr_t pTakeItem;

    IProjTreeItem* pChild = GetSelectedTreeItem();
    IProjTreeItem* pParent = GetTreeItemParent(pChild);

    if (!pChild || !pParent)
        CWARNING("You must select a child item for this operation");
    else
    {
        pTakeItem = pParent->TakeItem(*pChild);

        if (pTakeItem)
        {
            m_dataViewCtrlBrowser->GetModel()->ItemDeleted(wxDataViewItem(pParent), wxDataViewItem(pChild));
            m_editorManager.ItemDeleted(*pTakeItem);
        }
        else
        {
            const std::string strName = pChild->GetName();
            CWARNING("Item '%s' cannot be removed from the collection", strName.c_str());
        }
    }

    return pTakeItem;
}

void CMainWindow::onBtnDeleteItem(wxCommandEvent& event)
{
    TreeTakeSelectedItem();
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
    ITreeItemCollection::ptr_t pTakeItem = TreeTakeSelectedItem();

    if (pTakeItem)
        m_pCutClipboard = std::move(pTakeItem);
}

void CMainWindow::onBtnItemPaste(wxCommandEvent& event)
{
    // Make sure there is an item in clipboard to paste
    if (!m_pCutClipboard)
        return;

    // Make sure there is a selected item to receive the paste
    IProjTreeItem* pSelected = GetSelectedTreeItem();
    if (nullptr == pSelected)
        return;

    // Make sure the parent can accept the child
    const ETreeItemType eSupportedTypes = pSelected->GetSupportedChildTypes();
    const ETreeItemType eParentItemType = pSelected->GetType();
    const ETreeItemType ePasteItemType = m_pCutClipboard->GetType();

    if (0 == (eSupportedTypes & ePasteItemType))
    {
        const wxScopedCharBuffer childTypeName  = STreeItemTypeInfo::GetName(ePasteItemType).ToUTF8();
        const wxScopedCharBuffer parentTypeName = STreeItemTypeInfo::GetName(eParentItemType).ToUTF8();

        CWARNING("Cannot paste an item of type '%s' inside a parent of type '%s'", 
            childTypeName.data(),
            parentTypeName.data());
    }
    else
    {
        wxDataViewItem pasteItem(m_pCutClipboard.get()); // We must get this reference before std::move destroys the m_pCutClipboard
        wxDataViewItem parentItem(pSelected);

        // Move the clipboard item into the new parent's vector
        if (pSelected->AddItem(m_pCutClipboard.get()))
        {
            m_pCutClipboard.release(); // the ownership was transfered to the parent tree item

            // Update the GUI
            m_dataViewCtrlBrowser->GetModel()->ItemAdded(parentItem, pasteItem);
            m_dataViewCtrlBrowser->Expand(parentItem);
            m_dataViewCtrlBrowser->SetSelections(wxDataViewItemArray(1, pasteItem));
        }
        else
        {
            CERROR("Parent item refused to accept new child");
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

IProjTreeItem* CMainWindow::GetSelectedTreeItem() const
{
    return static_cast<IProjTreeItem*>(m_dataViewCtrlBrowser->GetSelection().GetID());
}

IProjTreeItem* CMainWindow::GetTreeItemParent(IProjTreeItem* pChild) const
{
    IProjTreeItem* pParent = nullptr;

    if (nullptr != pChild)
    {
        wxDataViewItem itParent = m_dataViewCtrlBrowser->GetModel()->GetParent(wxDataViewItem(pChild));
        pParent = static_cast<IProjTreeItem*>(itParent.GetID());
    }

    return pParent;
}

void CMainWindow::ReloadProject()
{
    // Close all tabs on the editor notebook
    m_editorManager.Clear();

    // Update tree: delete and re-add the project
    wxDataViewItem root(0);
    wxDataViewItem project(&m_rProject);
    m_dataViewCtrlBrowser->GetModel()->ItemDeleted(root, project);
    m_dataViewCtrlBrowser->GetModel()->ItemAdded(root, project);
}