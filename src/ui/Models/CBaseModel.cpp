#include "ui/Models/CBaseModel.h"

CBaseModel::CBaseModel()
{

}

CBaseModel::~CBaseModel()
{

}

/* wxDataViewModel overrides */
bool CBaseModel::HasValue(const wxDataViewItem& item, unsigned col) const
{
    return col < GetColumnCount();
}

wxString CBaseModel::GetColumnType(unsigned int col) const
{
    const IModelColumnHandler* pInfo = GetColumnInfo(col);
    assert(nullptr != pInfo);

    return pInfo->GetType();
}

void CBaseModel::GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const
{
    const IModelColumnHandler* pInfo = GetColumnInfo(col);
    assert(nullptr != pInfo);

    pInfo->GetValue(variant, item);
}

bool CBaseModel::SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col)
{
    IModelColumnHandler* pInfo = GetColumnInfo(col);
    assert(nullptr != pInfo);

    return pInfo->SetValue(variant, item);
}

bool CBaseModel::IsEnabled(const wxDataViewItem& item, unsigned int col) const
{
    const IModelColumnHandler* rInfo = GetColumnInfo(col);
    assert(nullptr != rInfo);

    return rInfo->IsEnabled(item);
}