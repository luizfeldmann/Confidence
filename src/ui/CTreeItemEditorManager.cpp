#include "ui/CTreeItemEditorManager.h"
#include "ui/STreeItemTypeInfo.h"

CTreeItemEditorManager::CTreeItemEditorManager(wxAuiNotebook& rNotebook)
    : m_rNotebook(rNotebook)
{
    m_rNotebook.Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, &CTreeItemEditorManager::OnPageClose, this);
}

CTreeItemEditorManager::~CTreeItemEditorManager()
{
    m_rNotebook.Unbind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, &CTreeItemEditorManager::OnPageClose, this);
}

CTreeItemEditorManager::VecPtrEditor::iterator CTreeItemEditorManager::FindEditor(IProjTreeItem& rItemFind)
{
    return std::find_if(m_vEditors.begin(), m_vEditors.end(), [&](const PtrEditor& pSearch)->bool
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
            m_vEditors.emplace_back( rInfo.m_fnNewEditor( m_rNotebook, pEditItem) );
    }
    else
    {
        itFoundEditor->get()->Reactivate();
    }
}

void CTreeItemEditorManager::ItemDeleted(IProjTreeItem& pEditItem)
{
    // Close editors of child items
    for (IProjTreeItem::ref_t& rChild : pEditItem.SubItems())
        ItemDeleted(rChild.get());

    // Close own editor
    VecPtrEditor::iterator itFoundEditor = FindEditor(pEditItem);
    if (m_vEditors.end() != itFoundEditor)
    {
        m_vEditors.erase(itFoundEditor);
    }
}

void CTreeItemEditorManager::ItemRenamed(IProjTreeItem& pEditItem)
{
    VecPtrEditor::iterator itFoundEditor = FindEditor(pEditItem);
    if (m_vEditors.end() != itFoundEditor)
    {
        itFoundEditor->get()->ItemChanged();
    }
}

void CTreeItemEditorManager::OnPageClose(wxAuiNotebookEvent& event)
{
    const int iPageIndex = event.GetSelection();
    wxWindow* const pPage = m_rNotebook.GetPage(iPageIndex);

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