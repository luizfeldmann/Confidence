#include "ui/Editors/ITreeItemEditor.h"
#include "ui/STreeItemTypeInfo.h"

ITreeItemEditor::ITreeItemEditor(wxAuiNotebook& rNotebook, IProjTreeItem& rItem)
    : m_rNotebook(rNotebook)
    , m_rItem(rItem)
    , m_pPanel( new wxPanel(&m_rNotebook) )
    , m_pSizer( new wxBoxSizer(wxVERTICAL) )
{
    m_pPanel->SetSizer(m_pSizer);

    const wxIcon& rIcon = STreeItemTypeInfo::GetIcon(m_rItem.GetType());
    m_rNotebook.AddPage(m_pPanel, m_rItem.GetName(), true, rIcon);
}

ITreeItemEditor::~ITreeItemEditor()
{
    const int iPageIndex = m_rNotebook.FindPage(m_pPanel);
    m_rNotebook.DeletePage(iPageIndex);
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
    return m_rNotebook;
}

void ITreeItemEditor::OnAnyItemRenamed(const IProjTreeItem& rChanged)
{
    // If the changed item is associated to this tab, then reload the title
    if (&m_rItem == &rChanged)
    {
        const int iPageIndex = m_rNotebook.FindPage(m_pPanel);
        m_rNotebook.SetPageText(iPageIndex, m_rItem.GetName());
    }
}

void ITreeItemEditor::Reactivate()
{
    const int iPageIndex = m_rNotebook.FindPage(m_pPanel);
    m_rNotebook.SetSelection(iPageIndex);
}