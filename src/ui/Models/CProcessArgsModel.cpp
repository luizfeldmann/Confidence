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
        value = "no expression";
    }

    bool IsEnabled(const wxDataViewItem&) const
    {
        return true;
    }

    bool SetValue(const wxVariant& value, const wxDataViewItem& rItem)
    {
        return false;
    }
};

/* CProcessArgsModel */
CProcessArgsModel::CProcessArgsModel(CRunProcess& rEdit)
    : m_rEdit(rEdit)
{
    // Create columns
    m_mapColumns[EProcessArgsColumn::Description] = std::make_unique<CArgumentDescriptionModelColumnHandler>();
    m_mapColumns[EProcessArgsColumn::Value]       = std::make_unique<CArgumentExpressionModelColumnHandler>();
}

CProcessArgsModel::~CProcessArgsModel()
{

}

static CRunProcess::vec_args_t::iterator FindArg(CRunProcess::vec_args_t& rvArgs, CProcessArgument* pFind)
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