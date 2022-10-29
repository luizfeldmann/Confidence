#include "ui/Models/CProcessArgsModel.h"
#include "core/items/process/CRunProcess.h"

/* static */ CProcessArgument* CProcessArgsModel::GetPointer(const wxDataViewItem& item)
{
    return static_cast<CProcessArgument*>(item.GetID());
}

/* static */ const wxDataViewItem CProcessArgsModel::GetViewItem(const CProcessArgument* pArg)
{
    return wxDataViewItem((void*)pArg);
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
        const CProcessArgument* const pItem = CProcessArgsModel::GetPointer(rItem);
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
        CProcessArgument* const pItem = CProcessArgsModel::GetPointer(rItem);
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
        const CProcessArgument* const pItem = CProcessArgsModel::GetPointer(rItem);
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
        CProcessArgument* const pItem = CProcessArgsModel::GetPointer(rItem);
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

const wxDataViewItem CProcessArgsModel::NewItem()
{
    CRunProcess::vec_args_t& rvArgs = m_rEdit.GetArguments();
    rvArgs.emplace_back();

    CProcessArgument* pNewItem = &rvArgs.back();
    const wxDataViewItem cNewItem = GetViewItem(pNewItem);

    ItemAdded(GetParent(cNewItem), cNewItem);
    
    return cNewItem;
}

bool CProcessArgsModel::DeleteItem(const wxDataViewItem& rDeleteItem)
{
    bool bSuccess = false;

    const CProcessArgument* const pDeleteItem = GetPointer(rDeleteItem);

    if (nullptr != pDeleteItem)
    {
        CRunProcess::vec_args_t& rvArgs = m_rEdit.GetArguments();
        CRunProcess::vec_args_t::iterator itFound = FindArg(rvArgs, pDeleteItem);

        if (rvArgs.end() != itFound)
        {
            rvArgs.erase(itFound);
            ItemDeleted(GetParent(rDeleteItem), rDeleteItem);

            bSuccess = true;
        }
    }

    return bSuccess;
}

const wxDataViewItem CProcessArgsModel::MoveItem(const wxDataViewItem& rMoveItem, bool bUp)
{
    wxDataViewItem cSwapItem = GetViewItem(nullptr);

    const CProcessArgument* const pMoveItem = GetPointer(rMoveItem);
    if (nullptr != pMoveItem)
    {
        // Find the item on the collection
        CRunProcess::vec_args_t& rvArgs = m_rEdit.GetArguments();
        CRunProcess::vec_args_t::iterator itMove = FindArg(rvArgs, pMoveItem);

        if (rvArgs.end() != itMove)
        {
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
            wxDataViewItem cParent = GetParent(rMoveItem);

            ItemDeleted(cParent, rMoveItem);
            ItemAdded(cParent, rMoveItem);

            cSwapItem = GetViewItem(&*itSwap);
        }
    }

    return cSwapItem;
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
    return GetViewItem(nullptr);
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
        {
            array.push_back(GetViewItem(&rArg));
            uCount++;
        }
    }

    return uCount;
}