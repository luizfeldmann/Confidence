#include "ui/Models/CFileOperationsModel.h"
#include "core/items/file_operation/CFileOperations.h"
#include "core/items/file_operation/CFileOperation.h"
#include "core/items/file_operation/IFileDataLocator.h"
#include "core/items/file_operation/IFileOperationHandler.h"
#include "util/Log.h"

/* static */ CFileOperation* CFileOperationsModel::GetPointer(const wxDataViewItem& item)
{
    return static_cast<CFileOperation*>(item.GetID());
}

/* static */ const wxDataViewItem CFileOperationsModel::GetViewItem(const CFileOperation* pOper)
{
    return wxDataViewItem((void*)pOper);
}

class COperationDirectionColumnHandler : public IModelColumnHandler
{
public:
    COperationDirectionColumnHandler() = default;

    wxString GetType() const override
    {
        return "int";
    }

    void GetValue(wxVariant& value, const wxDataViewItem& rItem) const override
    {
        const CFileOperation* const pItem = CFileOperationsModel::GetPointer(rItem);
        assert(pItem);

        long nIndex = (long)pItem->GetType();

        value = nIndex;
    }

    bool IsEnabled(const wxDataViewItem&) const
    {
        return true;
    }

    bool SetValue(const wxVariant& value, const wxDataViewItem& rItem)
    {
        CFileOperation* const pItem = CFileOperationsModel::GetPointer(rItem);
        assert(pItem);

        pItem->SetType( (EFileOperationType)value.GetInteger() );

        return true;
    }
};

class COperationLocatorColumnHandler : public IModelColumnHandler
{
public:
    COperationLocatorColumnHandler() = default;

    wxString GetType() const override
    {
        return "string";
    }

    void GetValue(wxVariant& value, const wxDataViewItem& rItem) const override
    {
        const CFileOperation* const pItem = CFileOperationsModel::GetPointer(rItem);
        assert(pItem);

        const IFileDataLocator* const pLocator = pItem->GetLocator();
        assert(pLocator);

        value = pLocator->to_string();
    }

    bool IsEnabled(const wxDataViewItem&) const
    {
        return true;
    }

    bool SetValue(const wxVariant& value, const wxDataViewItem& rItem)
    {
        CFileOperation* const pItem = CFileOperationsModel::GetPointer(rItem);
        assert(pItem);

        IFileDataLocator* const pLocator = pItem->GetLocator();
        assert(pLocator);

        const std::string strValue = value.GetString().ToStdString();

        return pLocator->from_string(strValue);
    }
};

class COperationExpressionColumnHandler : public IModelColumnHandler
{
public:
    COperationExpressionColumnHandler() = default;

    wxString GetType() const override
    {
        return "string";
    }

    void GetValue(wxVariant& value, const wxDataViewItem& rItem) const override
    {
        const CFileOperation* const pItem = CFileOperationsModel::GetPointer(rItem);
        assert(pItem);

        value = pItem->GetExpression();
    }

    bool IsEnabled(const wxDataViewItem&) const
    {
        return true;
    }

    bool SetValue(const wxVariant& value, const wxDataViewItem& rItem)
    {
        CFileOperation* const pItem = CFileOperationsModel::GetPointer(rItem);
        assert(pItem);

        const std::string strValue = value.GetString().ToStdString();

        return pItem->SetExpression(strValue);
    }
};

class COperationDescriptionColumnHandler : public IModelColumnHandler
{
public:
    COperationDescriptionColumnHandler() = default;

    wxString GetType() const override
    {
        return "string";
    }

    void GetValue(wxVariant& value, const wxDataViewItem& rItem) const override
    {
        const CFileOperation* const pItem = CFileOperationsModel::GetPointer(rItem);
        assert(pItem);

        value = pItem->GetDescription();
    }

    bool IsEnabled(const wxDataViewItem&) const
    {
        return true;
    }

    bool SetValue(const wxVariant& value, const wxDataViewItem& rItem)
    {
        CFileOperation* const pItem = CFileOperationsModel::GetPointer(rItem);
        assert(pItem);

        const std::string strValue = value.GetString().ToStdString();

        return pItem->SetDescription(strValue);
    }
};

/* CFileOperationsModel */

CFileOperationsModel::CFileOperationsModel(CFileOperations& rEdit)
    : m_rEdit(rEdit)
{
    m_mapColumns[(unsigned int)EColumn::Direction] = std::make_unique<COperationDirectionColumnHandler>();
    m_mapColumns[(unsigned int)EColumn::Location] = std::make_unique<COperationLocatorColumnHandler>();
    m_mapColumns[(unsigned int)EColumn::Expression] = std::make_unique<COperationExpressionColumnHandler>();
    m_mapColumns[(unsigned int)EColumn::Description] = std::make_unique<COperationDescriptionColumnHandler>();
}

CFileOperationsModel::~CFileOperationsModel()
{

}

const wxDataViewItem CFileOperationsModel::NewItem()
{
    wxDataViewItem wxItem(nullptr);

    IFileOperationHandler* pHandler = m_rEdit.GetHandler();
    if (!pHandler)
        CWARNING("A file type handler must be specified first");
    else
    {
        CFileOperations::vec_oper_t& rvOper = m_rEdit.GetItems();
        rvOper.emplace_back();

        CFileOperation* pOper = &rvOper.back();
        pOper->SetLocator( pHandler->CreateLocator() );

        wxItem = GetViewItem( pOper );
        ItemAdded(GetParent(wxItem), wxItem);
    }

    return wxItem;
}

static CFileOperations::vec_oper_t::iterator findOper(CFileOperations::vec_oper_t& vOper, const CFileOperation* pOper)
{
    return std::find_if(vOper.begin(), vOper.end(),
        [pOper](const CFileOperation& rOper)->bool
        {
            return (&rOper == pOper);
        }
    );
}

bool CFileOperationsModel::DeleteItem(const wxDataViewItem& item)
{
    CFileOperations::vec_oper_t& rvOper = m_rEdit.GetItems();
    CFileOperations::vec_oper_t::const_iterator itFind = findOper(rvOper, GetPointer(item));

    bool bStatus = false;

    if (rvOper.cend() != itFind)
    {
        rvOper.erase(itFind);
        ItemDeleted(GetParent(item), item);
        bStatus = true;
    }

    return bStatus;
}

const wxDataViewItem CFileOperationsModel::MoveItem(const wxDataViewItem& rMoveItem, bool bUp)
{
    CFileOperations::vec_oper_t& rvOper = m_rEdit.GetItems();
    CFileOperations::vec_oper_t::iterator itMove = findOper(rvOper, GetPointer(rMoveItem));

    wxDataViewItem wxSwapItem(nullptr);

    if (rvOper.cend() != itMove)
    {
        CFileOperations::vec_oper_t::iterator itSwap;

        if (bUp)
        {
            itSwap = (rvOper.begin() == itMove)
                ? std::prev(rvOper.end())
                : std::prev(itMove);
        }
        else
        {
            itSwap = std::next(itMove);

            if (rvOper.end() == itSwap)
                itSwap = rvOper.begin();
        }

        // Swap in the collection
        std::iter_swap(itMove, itSwap);

        // Update the data list view
        wxDataViewItem cParent = GetParent(rMoveItem);

        ItemDeleted(cParent, rMoveItem);
        ItemAdded(cParent, rMoveItem);

        wxSwapItem = GetViewItem(&*itSwap);
    }

    return wxSwapItem;
}

unsigned int CFileOperationsModel::GetColumnCount() const
{
    return (unsigned int)EColumn::Count;
}

wxDataViewItem CFileOperationsModel::GetParent(const wxDataViewItem& item) const
{
    return GetViewItem(nullptr);
}

bool CFileOperationsModel::IsContainer(const wxDataViewItem& item) const
{
    return false;
}

unsigned int CFileOperationsModel::GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const
{
    unsigned int uCount = 0;

    if (nullptr == parent.GetID())
    {
        for (const CFileOperation& rOper : m_rEdit.GetItems())
        {
            array.push_back(GetViewItem(&rOper));
            uCount++;
        }
    }

    return uCount;
}