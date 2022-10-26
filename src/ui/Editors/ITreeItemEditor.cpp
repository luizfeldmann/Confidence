#include "ui/Editors/ITreeItemEditor.h"
#include "ui/STreeItemTypeInfo.h"
#include "ui/CMainWindow.h"

ITreeItemEditor::ITreeItemEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem)
    : m_rMainWindow(rMainWindow)
    , m_rItem(rItem)
    , m_pPanel( new wxPanel(m_rMainWindow.m_notebookEditor) )
    , m_pSizer( new wxBoxSizer(wxVERTICAL) )
{
    m_pPanel->SetSizer(m_pSizer);

    const wxIcon& rIcon = STreeItemTypeInfo::GetIcon(m_rItem.GetType());
    GetNotebook().AddPage(m_pPanel, m_rItem.GetName(), true, rIcon);
}

ITreeItemEditor::~ITreeItemEditor()
{
    wxAuiNotebook& rNotebook = GetNotebook();

    const int iPageIndex = rNotebook.FindPage(m_pPanel);
    rNotebook.DeletePage(iPageIndex);
}

IProjTreeItem& ITreeItemEditor::GetItem()
{
    return m_rItem;
}

wxPanel* ITreeItemEditor::GetPage()
{
    return m_pPanel;
}

wxAuiNotebook& ITreeItemEditor::GetNotebook()
{
    assert(m_rMainWindow.m_notebookEditor);
    return *m_rMainWindow.m_notebookEditor;
}

std::shared_ptr<const CProject> ITreeItemEditor::GetProject()
{
    return m_rMainWindow.GetProject();
}

void ITreeItemEditor::OnAnyItemRenamed(const IProjTreeItem& rChanged)
{
    // If the changed item is associated to this tab, then reload the title
    if (&m_rItem == &rChanged)
    {
        wxAuiNotebook& rNotebook = GetNotebook();

        const int iPageIndex = rNotebook.FindPage(m_pPanel);
        rNotebook.SetPageText(iPageIndex, m_rItem.GetName());
    }
}

void ITreeItemEditor::Reactivate()
{
    wxAuiNotebook& rNotebook = GetNotebook();

    const int iPageIndex = rNotebook.FindPage(m_pPanel);
    rNotebook.SetSelection(iPageIndex);
}