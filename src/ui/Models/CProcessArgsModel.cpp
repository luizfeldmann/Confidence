#include "ui/Models/CProcessArgsModel.h"
#include "core/CRunProcess.h"

//! @brief Casts the DataViewItem into a process argument
static inline CProcessArgument* GetItem(const wxDataViewItem& item)
{
    return static_cast<CProcessArgument*>(item.GetID());
}

class CArgumentDescriptionModelColumnHandler : public IModelColumnHandler
{
public:
    CArgumentDescriptionModelColumnHandler() = default;

    wxString GetType() const override
    {
        return "string";
    }

    void GetValue(wxVariant& value, const wxDataViewItem& rItem) const override
    {
        CProcessArgument* pItem = GetItem(rItem);
        assert(pItem);

        const wxString strDesc = pItem->GetDescription();
        value = strDesc;
    }

    bool IsEnabled(const wxDataViewItem&) const
    {
        return true;
    }

    bool SetValue(const wxVariant& value, const wxDataViewItem& rItem)
    {
        CProcessArgument* pItem = GetItem(rItem);
        assert(pItem);

        wxString strDesc = value.GetString();

        return pItem->SetDescription(strDesc.ToStdString());
    }
};

class CArgumentExpressionModelColumnHandler : public IModelColumnHandler
{
public:
    CArgumentExpressionModelColumnHandler() = default;

    wxString GetType() const override
    {
        return "string";
    }

    void GetValue(wxVariant& value, const wxDataViewItem& rItem) const override
    {
        CProcessArgument* pItem = GetItem(rItem);
        assert(pItem);

        const wxString strExpr = pItem->GetExpression();
        value = strExpr;
    }

    bool IsEnabled(const wxDataViewItem&) const
    {
        return true;
    }

    bool SetValue(const wxVariant& value, const wxDataViewItem& rItem)
    {
        CProcessArgument* pItem = GetItem(rItem);
        assert(pItem);

        wxString strExpr = value.GetString();

        return pItem->SetExpression(strExpr.ToStdString());
    }
};

/* CProcessArgsModel */
CProcessArgsModel::CProcessArgsModel(CRunProcess& rEdit)
    : m_rEdit(rEdit)
{
    // Create columns
    m_mapColumns[EProcessArgsColumn::Description] = std::make_unique<CArgumentDescriptionModelColumnHandler>();
    m_mapColumns[EProcessArgsColumn::Expression]  = std::make_unique<CArgumentExpressionModelColumnHandler>();
}

CProcessArgsModel::~CProcessArgsModel()
{

}

static CRunProcess::vec_args_t::iterator FindArg(CRunProcess::vec_args_t& rvArgs, const CProcessArgument* pFind)
{
    return std::find_if(rvArgs.begin(), rvArgs.end(), [pFind](const CProcessArgument& rTest)->bool
        {
            return &rTest == pFind;
        });
}

CProcessArgument* CProcessArgsModel::NewItem()
{
    CRunProcess::vec_args_t& rvArgs = m_rEdit.GetArguments();
    rvArgs.emplace_back();

    CProcessArgument* pNewItem = &rvArgs.back();

    ItemAdded(wxDataViewItem(nullptr), wxDataViewItem(pNewItem));
    
    return pNewItem;
}

bool CProcessArgsModel::DeleteItem(CProcessArgument* pDeleteItem)
{
    bool bSuccess = false;

    CRunProcess::vec_args_t& rvArgs = m_rEdit.GetArguments();
    CRunProcess::vec_args_t::iterator itFound = FindArg(rvArgs, pDeleteItem);

    if (rvArgs.end() != itFound)
    {
        rvArgs.erase(itFound);
        ItemDeleted(wxDataViewItem(nullptr), wxDataViewItem(pDeleteItem));
        bSuccess = true;
    }

    return bSuccess;
}

CProcessArgument* CProcessArgsModel::MoveItem(CProcessArgument* pMoveItem, bool bUp)
{
    // Find the item on the collection
    CRunProcess::vec_args_t& rvArgs = m_rEdit.GetArguments();
    CRunProcess::vec_args_t::iterator itMove = FindArg(rvArgs, pMoveItem);

    if (rvArgs.end() == itMove)
        return nullptr; // cannot move item - it's not on the collection

    // Get iterator to the swap item
    CRunProcess::vec_args_t::iterator itSwap;

    if (bUp)
    {
        itSwap = (rvArgs.begin() == itMove)
            ? std::prev(rvArgs.end())
            : std::prev(itMove);
    }
    else
    {
        itSwap = std::next(itMove);

        if (rvArgs.end() == itSwap)
            itSwap = rvArgs.begin();
    }

    // Swap in the collection
    std::iter_swap(itMove, itSwap);

    // Update the data list view
    wxDataViewItem cItem(pMoveItem);
    wxDataViewItem cParent = GetParent(cItem);

    ItemDeleted(cParent, cItem);
    ItemAdded(cParent, cItem);

    return &(*itSwap);
}

IModelColumnHandler* CProcessArgsModel::GetColumnInfo(unsigned int nModelColumn)
{
    IModelColumnHandler* pInfo = nullptr;

    const auto itFound = m_mapColumns.find((EProcessArgsColumn)nModelColumn);
    if (m_mapColumns.end() != itFound)
        pInfo = itFound->second.get();

    return pInfo;
}

const IModelColumnHandler* CProcessArgsModel::GetColumnInfo(unsigned int nModelColumn) const
{
    const IModelColumnHandler* pInfo = nullptr;

    const auto itFound = m_mapColumns.find((EProcessArgsColumn)nModelColumn);
    if (m_mapColumns.cend() != itFound)
        pInfo = itFound->second.get();

    return pInfo;
}

unsigned int CProcessArgsModel::GetColumnCount() const
{
    return (unsigned int)EProcessArgsColumn::Count;
}

wxDataViewItem CProcessArgsModel::GetParent(const wxDataViewItem& item) const
{
    return wxDataViewItem(nullptr);
}

bool CProcessArgsModel::IsContainer(const wxDataViewItem& item) const
{
    return false;
}

unsigned int CProcessArgsModel::GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const
{
    unsigned int uCount = 0;

    if (nullptr == parent.GetID())
    {
        for (CProcessArgument& rArg : m_rEdit.GetArguments())
            array.push_back( wxDataViewItem(&rArg) );
    }

    return uCount;
}