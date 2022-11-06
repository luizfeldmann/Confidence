#include "ui/Models/CConditionalTreeModel.h"
#include "core/items/condition/CLogicalComparator.h"
#include "core/items/condition/CLogicalOperator.h"
#include "core/items/condition/CConditional.h"
#include "util/Log.h"

/* Column handlers */
class CIconColumnHandler : public IModelColumnHandler
{
public:
    CIconColumnHandler() = default;

    wxString GetType() const override
    {
        return "icon";
    }

    void GetValue(wxVariant& value, const wxDataViewItem& rItem) const override
    {
        const ICondition* const pCond = CConditionalTreeModel::GetPointer(rItem);
        value << CConditionalTreeModel::GetIcon(pCond);
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

static CLogicalComparator* GetComparator(const wxDataViewItem& rItem)
{
    return dynamic_cast<CLogicalComparator*>(CConditionalTreeModel::GetPointer(rItem));
}

class CExpressionColumnHandler : public IModelColumnHandler
{
protected:
    const bool m_bIsRight;

public:
    CExpressionColumnHandler(bool bRightSide)
        : m_bIsRight(bRightSide)
    {

    }

    wxString GetType() const override
    {
        return "text";
    }

    void GetValue(wxVariant & value, const wxDataViewItem & rItem) const override
    {
        const CLogicalComparator* const pComparator = GetComparator(rItem);

        if (pComparator)
        {
            std::string strExpression;

            if (m_bIsRight)
                strExpression = pComparator->GetRHS();
            else
                strExpression = pComparator->GetLHS();

            value = wxString(strExpression);
        }
    }

    bool IsEnabled(const wxDataViewItem& rItem) const
    {
        return (nullptr != GetComparator(rItem));
    }

    bool SetValue(const wxVariant& value, const wxDataViewItem & rItem)
    {
        bool bStatus = false;
        CLogicalComparator* const pComparator = GetComparator(rItem);

        if (pComparator)
        {
            const std::string strExpression = value.GetString();

            if (m_bIsRight)
                bStatus = pComparator->SetRHS(strExpression);
            else
                bStatus = pComparator->SetLHS(strExpression);
        }

        return bStatus;
    }
};

class CInfixColumnHandler : public IModelColumnHandler
{
public:
    CInfixColumnHandler() = default;

    wxString GetType() const override
    {
        return "text";
    }

    void GetValue(wxVariant& value, const wxDataViewItem& rItem) const override
    {
        const ICondition* const pCondition = CConditionalTreeModel::GetPointer(rItem);

        if (pCondition)
            value = wxString(pCondition->GetInfix());
    }

    bool IsEnabled(const wxDataViewItem&) const
    {
        return false;
    }

    bool SetValue(const wxVariant& value, const wxDataViewItem& rItem)
    {
        return false;
    }
};

/* CConditionalTreeModel */
CConditionalTreeModel::CConditionalTreeModel(CConditional& rEdit)
    : m_rEdit(rEdit)
{
    m_mapColumns[(unsigned int)EColumn::Icon] = std::make_unique<CIconColumnHandler>();
    m_mapColumns[(unsigned int)EColumn::LHS] = std::make_unique<CExpressionColumnHandler>(false);
    m_mapColumns[(unsigned int)EColumn::Infix] = std::make_unique<CInfixColumnHandler>();
    m_mapColumns[(unsigned int)EColumn::RHS] = std::make_unique<CExpressionColumnHandler>(true);
}

CConditionalTreeModel::~CConditionalTreeModel()
{

}

/* static */ ICondition* CConditionalTreeModel::GetPointer(const wxDataViewItem& item)
{
    return static_cast<ICondition*>(item.GetID());
}

/* static */ const wxDataViewItem CConditionalTreeModel::GetViewItem(const ICondition* pCond)
{
    return wxDataViewItem((void*)pCond);
}

/* static */ wxIcon CConditionalTreeModel::GetIcon(const ICondition* pCond)
{
    wxIcon icon = wxNullIcon;

    if (pCond)
    {
        const ICondition::ECategory eCategory = pCond->GetCategory();

        if (eCategory == ICondition::ECategory::Comparator)
            icon = GetIcon(dynamic_cast<const CLogicalComparator*>(pCond)->GetType());
        else if (eCategory == ICondition::ECategory::Operator)
            icon = GetIcon(dynamic_cast<const CLogicalOperator*>(pCond)->GetType());
    }

    return icon;
}

#define RESICON(RESNAME) wxIcon(RESNAME, wxBITMAP_TYPE_ICO_RESOURCE, 24, 24)

/* static */ wxIcon CConditionalTreeModel::GetIcon(const ELogicalComparator& eComparator)
{
    static const std::map<ELogicalComparator, wxIcon> c_map {
        { ELogicalComparator::Equal,            RESICON("RES_ID_ICON_COMP_EQUAL") },
        { ELogicalComparator::NotEqual,         RESICON("RES_ID_ICON_COMP_NOTEQUAL") },
        { ELogicalComparator::Less,             RESICON("RES_ID_ICON_COMP_LESS") },
        { ELogicalComparator::LessOrEqual,      RESICON("RES_ID_ICON_COMP_LESSEQUAL") },
        { ELogicalComparator::Greater,          RESICON("RES_ID_ICON_COMP_GREATER") },
        { ELogicalComparator::GreaterOrEqual,   RESICON("RES_ID_ICON_COMP_GREATEREQUAL") },
    };

    return c_map.at(eComparator);
}

/* static */ wxIcon CConditionalTreeModel::GetIcon(const ELogicalOperator& eOperator)
{
    static const std::map<ELogicalOperator, wxIcon> c_map {
        { ELogicalOperator::And, RESICON("RES_ID_ICON_GATE_AND") },
        { ELogicalOperator::Or,  RESICON("RES_ID_ICON_GATE_OR" ) },
        { ELogicalOperator::Not, RESICON("RES_ID_ICON_GATE_NOT") },
    };

    return c_map.at(eOperator);
}

/* Public interface */

wxDataViewItem CConditionalTreeModel::NewItem(std::shared_ptr<ICondition> pNewCondition, const wxDataViewItem& parent)
{
    wxDataViewItem pAddItem;
    ICondition* const pParent = GetPointer(parent);
    
    if (!pParent)
    {
        if (m_rEdit.GetCondition())
        {
            CWARNING("A top-level condition is already defined");
        }
        else
        {
            m_rEdit.SetCondition(pNewCondition);
            pAddItem = GetViewItem(pNewCondition.get());
            ItemAdded(parent, pAddItem);
        }
    }
    else
    {
        if (pParent->GetCategory() != ICondition::ECategory::Operator)
        {
            CWARNING("The selected conditional type cannot receive subitems");
        }
        else
        {
            CLogicalOperator* const pOperator = dynamic_cast<CLogicalOperator*>(pParent);
            assert(pOperator);

            if (!pOperator->AddItem(pNewCondition))
            {
                CWARNING("Failed to insert item on parent");
            }
            else
            {
                pAddItem = GetViewItem(pNewCondition.get());
                ItemAdded(parent, pAddItem);
            }
        }
    }

    return pAddItem;
}

bool CConditionalTreeModel::DeleteItem(const wxDataViewItem& item)
{
    bool bStatus = false;
    const wxDataViewItem parent = GetParent(item);

    if (nullptr == parent.GetID())
    {
        m_rEdit.SetCondition(nullptr);
        ItemDeleted(parent, item);
        bStatus = true;
    }
    else
    {
        ICondition* pCondition = GetPointer(item);
        CLogicalOperator* const pOperator = dynamic_cast<CLogicalOperator*>(GetPointer(parent));
        assert(pOperator && pCondition);

        bStatus = pOperator->EraseItem(pCondition->shared_from_this());

        if (bStatus)
            ItemDeleted(parent, item);
    }

    return bStatus;
}

bool CConditionalTreeModel::MoveItem(const wxDataViewItem& rMoveItem, bool bUp)
{
    bool bStatus = false;

    const wxDataViewItem parent = GetParent(rMoveItem);
    ICondition* const pChild = GetPointer(rMoveItem);
    CLogicalOperator* const pParent = dynamic_cast<CLogicalOperator*>(GetPointer(parent));
   
    if (pChild && pParent)
    {
        CLogicalOperator::vec_cond_t vSubitems = pParent->SubItems();

        CLogicalOperator::vec_cond_t::iterator itMove = 
            std::find(vSubitems.begin(), vSubitems.end(), pChild->shared_from_this());

        if (vSubitems.end() != itMove)
        {
            // Get iterator to the swap item
            CLogicalOperator::vec_cond_t::iterator itSwap;

            if (bUp)
            {
                itSwap = (vSubitems.begin() == itMove)
                    ? std::prev(vSubitems.end())
                    : std::prev(itMove);
            }
            else
            {
                itSwap = std::next(itMove);

                if (vSubitems.end() == itSwap)
                    itSwap = vSubitems.begin();
            }

            pParent->SwapItems(*itMove, *itSwap);

            ItemDeleted(parent, rMoveItem);
            ItemAdded(parent, rMoveItem);

            bStatus = true;
        }
    }

    return bStatus;
}

/* wxDataViewModel overrides */

unsigned int CConditionalTreeModel::GetColumnCount() const
{
    return (unsigned int)EColumn::Count;
}

wxDataViewItem CConditionalTreeModel::GetParent(const wxDataViewItem& item) const
{
    parent_map_t::const_iterator itFind = m_mapParents.find(item);

    return (m_mapParents.cend() == itFind)
        ? GetViewItem(nullptr)
        : itFind->second;
}

bool CConditionalTreeModel::IsContainer(const wxDataViewItem& item) const
{
    bool bContainer = true;
    const ICondition* pCond = GetPointer(item);

    if (pCond)
        bContainer = pCond->GetCategory() == ICondition::ECategory::Operator;

    return bContainer;
}

unsigned int CConditionalTreeModel::GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const
{
    unsigned int uCount = 0;
    const ICondition* pCondParent = GetPointer(parent);
    
    if (!pCondParent)
    {
        std::shared_ptr<const ICondition> pCondChild = m_rEdit.GetCondition();
        if (pCondChild)
        {
            wxDataViewItem item = GetViewItem(pCondChild.get());
            array.push_back(item);
            uCount = 1;

            m_mapParents[item] = parent;
        }
    }
    else
    {
        if (pCondParent->GetCategory() == ICondition::ECategory::Operator)
        {
            const CLogicalOperator* pOperator = dynamic_cast<const CLogicalOperator*>(pCondParent);
            assert(pOperator);

            CLogicalOperator::vec_ccond_t vSubitems = pOperator->SubItems();
            for (const CLogicalOperator::const_condition_ptr_t& pChild : vSubitems)
            {
                wxDataViewItem cChild = GetViewItem(pChild.get());
                array.push_back(cChild);
                uCount++;

                m_mapParents[cChild] = parent;
            }
        }
    }

    return uCount;
}