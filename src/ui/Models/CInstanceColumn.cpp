#include "ui/Models/CInstanceColumn.h"
#include "ui/STreeItemTypeInfo.h"
#include "core/items/CInstance.h"

CInstanceColumn::CInstanceColumn(std::shared_ptr<const CInstance> pInst, unsigned int nModelColumn, wxDataViewCellMode eMode)
    : wxDataViewColumn(GetIcon(true), new wxDataViewIconTextRenderer(wxDataViewIconTextRenderer::GetDefaultType(), eMode), nModelColumn)
    , m_pInstance(pInst)
{
    assert(pInst && "Column created from null instance");
    const std::string strInstanceName = pInst->GetName();
    SetTitle(strInstanceName);
}

CInstanceColumn::CInstanceColumn(unsigned int nModelColumn, wxDataViewCellMode eMode)
    : wxDataViewColumn(GetIcon(false), new wxDataViewIconTextRenderer(wxDataViewIconTextRenderer::GetDefaultType(), eMode), nModelColumn)
    , m_pInstance()
{
    SetTitle("Expression");
}

std::shared_ptr<const CInstance> CInstanceColumn::GetInstance() const
{
    return m_pInstance.lock();
}


/* static */ const wxIcon& CInstanceColumn::GetIcon(bool bHasInstances)
{
    static const wxIcon iconDefault("RES_ID_ICON_TREEITEM_INSTANCEFAV");

    return bHasInstances ? STreeItemTypeInfo::GetIcon(ETreeItemType::EInstance) : iconDefault;
}

/* static */ CInstanceColumn* CInstanceColumn::GetColumnByModelIndex(unsigned nModelColumn, const wxDataViewCtrl* const pCtrl)
{
    CInstanceColumn* pFound = nullptr;

    unsigned int uColCount = pCtrl->GetColumnCount();
    for (unsigned int i = 0; i < uColCount; i++)
    {
        CInstanceColumn* const pSearch = dynamic_cast<CInstanceColumn*>(pCtrl->GetColumnAt(i));
        if (pSearch && pSearch->GetModelColumn() == nModelColumn)
        {
            pFound = pSearch;
            break;
        }
    }

    return pFound;
}