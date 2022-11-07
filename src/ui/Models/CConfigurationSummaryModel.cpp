#include "ui/Models/CConfigurationSummaryModel.h"

CConfigurationSummaryModel::CConfigurationSummaryModel(CConfiguration& rConfig, std::shared_ptr<const CProject> pProj, wxDataViewCtrl* pCtrl)
    : m_rEdit(rConfig)
    , m_pProj(pProj)
    , m_pCtrl(pCtrl)
{

}

CConfigurationSummaryModel::~CConfigurationSummaryModel()
{

}

void CConfigurationSummaryModel::ReloadColumns()
{

}

void CConfigurationSummaryModel::OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent)
{

}

void CConfigurationSummaryModel::OnAnyItemErased(const IProjTreeItem& rItem)
{

}

void CConfigurationSummaryModel::OnAnyItemRenamed(const IProjTreeItem& rItem)
{

}

unsigned int CConfigurationSummaryModel::GetColumnCount() const
{
    return m_pCtrl->GetColumnCount();
}

bool CConfigurationSummaryModel::HasValue(const wxDataViewItem& item, unsigned col) const
{
    return true;
}

wxString CConfigurationSummaryModel::GetColumnType(unsigned int col) const
{
    return "wxDataViewIconText";
}

void CConfigurationSummaryModel::GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const
{

}

bool CConfigurationSummaryModel::SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col)
{
    return false;
}

bool CConfigurationSummaryModel::IsEnabled(const wxDataViewItem& item, unsigned int col) const
{
    return true;
}

bool CConfigurationSummaryModel::IsContainer(const wxDataViewItem& item) const
{
    return false;
}

wxDataViewItem CConfigurationSummaryModel::GetParent(const wxDataViewItem& item) const
{
    return wxDataViewItem(nullptr);
}

unsigned int CConfigurationSummaryModel::GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const
{
    return 0;
}