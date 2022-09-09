#include "ui/CMainWindow.h"
#include "ui/CTreeBrowserModel.h"
#include "ui/STreeItemTypeInfo.h"
#include "util/Log.h"
#include <wx/filedlg.h>
#include <wx/menu.h>
#include "core/CProject.h"

const static wxString g_szProjectFileFilter = "Confidence projects (*.cfx)|*.cfx";

CMainWindow::CMainWindow()
    : IMainWindow(nullptr)
    , m_editorManager(*m_notebookEditor)
{
    // Let the logger know we are using GUI
    CLog::m_bShowMessageBox = true;

    // Configure the main window's icon
    SetIcon(wxIcon("RES_ID_ICON_APPLICATION"));

    // Configure the project tree view
    CTreeBrowserModel* pTreeModel = new CTreeBrowserModel;
    pTreeModel->Bind(EVT_TREE_ITEM_RENAME, &CMainWindow::onTreeItemRenamed, this);
    m_dataViewCtrlBrowser->AssociateModel(pTreeModel);
    pTreeModel->DecRef();
    m_dataViewCtrlBrowser->ExpandChildren(m_dataViewCtrlBrowser->GetTopItem());

    // Configure the editor tabs
    m_notebookEditor->Bind(EVT_EDITOR_ITEM_CHANGED_NAME, &CMainWindow::onEditorItemNameDescChanged, this);
    m_notebookEditor->Bind(EVT_EDITOR_ITEM_CHANGED_DESC, &CMainWindow::onEditorItemNameDescChanged, this);

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
    CProject::TheProject() = std::move( CProject() );
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
        CProject::TheProject() = std::move( openProj );
        ReloadProject();
    }
}

void CMainWindow::onBtnSaveProject(wxCommandEvent& event)
{
    const std::string& szCurrentPath = CProject::TheProject().GetCurrentPath();
    if (szCurrentPath.empty())
    {
        onBtnSaveAsProject(event);
    }
    else
    {
        CProject::TheProject().SaveToFile(szCurrentPath);
    }
}

void CMainWindow::onBtnSaveAsProject(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(this, wxFileSelectorPromptStr, wxEmptyString, wxEmptyString,
        g_szProjectFileFilter, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (wxID_CANCEL == saveFileDialog.ShowModal())
        return;

    CProject::TheProject().SaveToFile(saveFileDialog.GetPath().ToStdString());
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
    assert(nullptr != pSelected);

    IProjTreeItem::OptVecPtrItem optSubitems = pSelected->GetItems();
    assert(optSubitems.has_value());

    IProjTreeItem::VecPtrItem& rvSubitems = optSubitems.value().get();

    // Get information about the class of subitem we wish to append
    const ETreeItemType eType = (ETreeItemType)evt.GetId();
    const STreeItemTypeInfo& rInfo = STreeItemTypeInfo::GetInfo(eType);

    assert(nullptr != rInfo.m_fnFactory);

    // Create new child and add to collection
    IProjTreeItem* pNewChild = rInfo.m_fnFactory();
    assert(nullptr != pNewChild);

    rvSubitems.emplace_back(pNewChild);

    // Notify the view about a new item was created
    wxDataViewItem parentItem(pSelected);
    wxDataViewItem newItem(pNewChild);

    m_dataViewCtrlBrowser->GetModel()->ItemAdded(parentItem, newItem);

    m_dataViewCtrlBrowser->Expand(parentItem);
    m_dataViewCtrlBrowser->SetSelections(wxDataViewItemArray(1, newItem));
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

void CMainWindow::OnItemOperation(EItemOperation eOper)
{
    // Ensure there is a selected item for us to operate
    // If the parent is root, then the project itself is selected
    // We cannot move/cut/delete the project itself
    IProjTreeItem* pChild = GetSelectedTreeItem();
    IProjTreeItem* pParent = GetTreeItemParent(pChild);

    if (!pChild || !pParent)
        return;

    // Find the iterator to this item inside it's parent vector
    IProjTreeItem::OptVecPtrItem optParentSubitems = pParent->GetItems();
    IProjTreeItem::VecPtrItem& vParentSubitems = optParentSubitems.value().get();

    IProjTreeItem::VecPtrItem::iterator iterChild = std::find_if(vParentSubitems.begin(), vParentSubitems.end(),
        [pChild](const IProjTreeItem::PtrItem& pSearch)->bool {
            return (pSearch.get() == pChild);
        });
    assert(vParentSubitems.cend() != iterChild);

    bool bItemDelete = false;
    bool bItemCreate = false;

    switch (eOper)
    {
        default:
        case EItemOperation::Delete:
            m_editorManager.ItemDeleted(*pChild);
            vParentSubitems.erase(iterChild);
            bItemDelete = true;
        break;

        case EItemOperation::Cut:
            m_editorManager.ItemDeleted(*pChild);
            m_pCutClipboard.reset( iterChild->release() );
            vParentSubitems.erase(iterChild);
            bItemDelete = true;
        break;

        case EItemOperation::MoveUp:
            if (vParentSubitems.cbegin() != iterChild) // Can only move up if it's not the first
            {
                IProjTreeItem::VecPtrItem::iterator iterPrev = std::prev(iterChild);
                std::iter_swap(iterChild, iterPrev);
                bItemDelete = true;
                bItemCreate = true;
            }
            break;

        case EItemOperation::MoveDown:
            IProjTreeItem::VecPtrItem::iterator iterNext = std::next(iterChild);
            if (vParentSubitems.cend() != iterNext) // Can only move up if it's not the first
            {
                std::iter_swap(iterChild, iterNext);
                bItemDelete = true;
                bItemCreate = true;
            }
            break;
    }

    if (bItemDelete)
        m_dataViewCtrlBrowser->GetModel()->ItemDeleted(wxDataViewItem(pParent), wxDataViewItem(pChild));

    if (bItemCreate)
        m_dataViewCtrlBrowser->GetModel()->ItemAdded(wxDataViewItem(pParent), wxDataViewItem(pChild));
}

void CMainWindow::onBtnDeleteItem(wxCommandEvent& event)
{
    OnItemOperation(EItemOperation::Delete);
}

void CMainWindow::onBtnItemUp(wxCommandEvent& event)
{
    OnItemOperation(EItemOperation::MoveUp);
}

void CMainWindow::onBtnItemDown(wxCommandEvent& event)
{
    OnItemOperation(EItemOperation::MoveDown);
}

void CMainWindow::onBtnItemCut(wxCommandEvent& event)
{
    OnItemOperation(EItemOperation::Cut);
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
        ITreeItemCollection::OptVecPtrItem optSubitems = pSelected->GetItems();
        assert(optSubitems.has_value());
        ITreeItemCollection::VecPtrItem& rVecSubitems = optSubitems.value().get();

        wxDataViewItem pasteItem(m_pCutClipboard.get()); // We must get this reference before std::move destroys the m_pCutClipboard
        wxDataViewItem parentItem(pSelected);

        // Move the clipboard item into the new parent's vector
        rVecSubitems.emplace_back( std::move(m_pCutClipboard) );

        // Update the GUI
        m_dataViewCtrlBrowser->GetModel()->ItemAdded(parentItem, pasteItem);
        m_dataViewCtrlBrowser->Expand(parentItem);
        m_dataViewCtrlBrowser->SetSelections(wxDataViewItemArray(1, pasteItem));
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
    wxDataViewItem project(&CProject::TheProject());
    m_dataViewCtrlBrowser->GetModel()->ItemDeleted(root, project);
    m_dataViewCtrlBrowser->GetModel()->ItemAdded(root, project);
}