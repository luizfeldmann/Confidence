#include "ui/CTreeItemEditorManager.h"
#include "ui/STreeItemTypeInfo.h"
#include "ui/CMainWindow.h"

CTreeItemEditorManager::CTreeItemEditorManager(CMainWindow& rMainWindow)
    : m_rMainWindow(rMainWindow)
{
    assert(m_rMainWindow.m_notebookEditor);
    m_rMainWindow.m_notebookEditor->Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, &CTreeItemEditorManager::OnPageClose, this);
}

CTreeItemEditorManager::~CTreeItemEditorManager()
{
    assert(m_rMainWindow.m_notebookEditor);
    m_rMainWindow.m_notebookEditor->Unbind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, &CTreeItemEditorManager::OnPageClose, this);
}

CTreeItemEditorManager::VecPtrEditor::iterator CTreeItemEditorManager::FindEditor(IProjTreeItem& rItemFind)
{
    return std::find_if(m_vEditors.begin(), m_vEditors.end(), [&](const PtrEditor& pSearch)->bool
        {
            return (&(pSearch->GetItem()) == &rItemFind);
        });
}

CTreeItemEditorManager::VecPtrEditor::const_iterator CTreeItemEditorManager::FindEditor(const IProjTreeItem& rItemFind)
{
    return std::find_if(m_vEditors.cbegin(), m_vEditors.cend(), [&](const PtrEditor& pSearch)->bool
        {
            return (&(pSearch->GetItem()) == &rItemFind);
        });
}

CTreeItemEditorManager::VecPtrEditor::iterator CTreeItemEditorManager::FindEditor(wxWindow* pFindWindow)
{
    return std::find_if(m_vEditors.begin(), m_vEditors.end(), [pFindWindow](const PtrEditor& pSearch)->bool
        {
            return (pSearch->GetPage() == pFindWindow);
        });
}

void CTreeItemEditorManager::ActivateItem(IProjTreeItem& pEditItem)
{
    VecPtrEditor::iterator itFoundEditor = FindEditor(pEditItem);

    if (m_vEditors.end() == itFoundEditor)
    {
        const STreeItemTypeInfo& rInfo = STreeItemTypeInfo::GetInfo(pEditItem.GetType());
        
        if (rInfo.m_fnNewEditor)
            m_vEditors.emplace_back( rInfo.m_fnNewEditor( m_rMainWindow, pEditItem) );
    }
    else
    {
        itFoundEditor->get()->Reactivate();
    }
}

void CTreeItemEditorManager::OnAnyItemErased(const IProjTreeItem& pEditItem)
{
    // Close editors of child items
    for (IProjTreeItem::cref_t& rChild : pEditItem.SubItems())
        OnAnyItemErased(rChild.get());

    // Notify all editors the item has been erased
    for (const PtrEditor& pEditor : m_vEditors)
        pEditor->OnAnyItemErased(pEditItem);

    // Close own editor
    VecPtrEditor::const_iterator itFoundEditor = FindEditor(pEditItem);
    if (m_vEditors.cend() != itFoundEditor)
    {
        m_vEditors.erase(itFoundEditor);
    }
}

void CTreeItemEditorManager::OnAnyItemRenamed(const IProjTreeItem& pEditItem)
{
    for (const PtrEditor& pEditor : m_vEditors)
        pEditor->OnAnyItemRenamed(pEditItem);
}

void CTreeItemEditorManager::OnItemCreated(const IProjTreeItem& pEditItem, const IProjTreeItem& rParent)
{
    for (const PtrEditor& pEditor : m_vEditors)
        pEditor->OnItemCreated(pEditItem, rParent);
}

void CTreeItemEditorManager::OnPageClose(wxAuiNotebookEvent& event)
{
    assert(m_rMainWindow.m_notebookEditor);

    const int iPageIndex = event.GetSelection();
    wxWindow* const pPage = m_rMainWindow.m_notebookEditor->GetPage(iPageIndex);

    VecPtrEditor::iterator itFoundEditor = FindEditor(pPage);
    if (m_vEditors.end() != itFoundEditor)
    {
        m_vEditors.erase(itFoundEditor);
        event.Veto(); // Do not let the system close the page - let the editor close itself
    }
}

void CTreeItemEditorManager::Clear()
{
    m_vEditors.clear();
}