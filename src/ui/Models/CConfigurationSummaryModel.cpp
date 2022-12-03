/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "ui/Models/CConfigurationSummaryModel.h"
#include "ui/Models/CInstanceColumn.h"
#include "ui/Models/CVariableCell.h"
#include "ui/STreeItemTypeInfo.h"
#include "core/items/CProject.h"
#include "core/items/CInstance.h"
#include "core/items/CInstanceGroup.h"
#include "core/items/CConfiguration.h"
#include "core/items/variable/CVariable.h"

CConfigurationSummaryModel::CConfigurationSummaryModel(CConfiguration& rConfig, std::shared_ptr<const CProject> pProj, wxDataViewCtrl* pCtrl)
    : m_rEdit(rConfig)
    , m_pProj(pProj)
    , m_pCtrl(pCtrl)
{
    ReloadColumns();
}

//! @brief Enumeration of columns shown in the summary of the configuration
enum class ESummaryModelColumns : unsigned int
{
    VariableName       = 0, //!< Displays the name of the variable
    DefaultExpression  = 1, //!< Displays the default expression if not "per instance"
    InstanceFirst      = 2, //!< First column index associated to a specific instance
};

/* static */ CVariable* CConfigurationSummaryModel::GetPointer(const wxDataViewItem& item)
{
    return static_cast<CVariable*>(item.GetID());
}

/* static */ wxDataViewItem CConfigurationSummaryModel::GetViewItem(const CVariable* pItem)
{
    return wxDataViewItem((void*)pItem);
}

CConfigurationSummaryModel::~CConfigurationSummaryModel()
{

}

void CConfigurationSummaryModel::ReloadColumns()
{
    Remap(m_pProj.lock());
    m_pCtrl->ClearColumns();

    // Create first column for the variable names
    const STreeItemTypeInfo& rHeaderInfo = STreeItemTypeInfo::GetInfo(ETreeItemType::EVariable);
    wxDataViewColumn* pColConfig = m_pCtrl->AppendIconTextColumn(rHeaderInfo.m_icon, (unsigned int)ESummaryModelColumns::VariableName);
    pColConfig->SetTitle(rHeaderInfo.m_strTypeName);

    // Create generic column for the expressions
    m_pCtrl->AppendColumn(new CInstanceColumn((unsigned int)ESummaryModelColumns::DefaultExpression, wxDATAVIEW_CELL_ACTIVATABLE));

    // Create per-instance columns
    unsigned int nModelColumn = (unsigned int)ESummaryModelColumns::InstanceFirst;
    {
        std::shared_ptr<const CProject> pProj = m_pProj.lock();
        assert(pProj);

        const ITreeItemCollection::vec_cptr_t vSubItems = pProj->GetInstances()->SubItems();

        for (const ITreeItemCollection::cptr_t& pItem : vSubItems)
        {
            assert(pItem && ETreeItemType::EInstance == pItem->GetType());

            m_pCtrl->AppendColumn(new CInstanceColumn(std::dynamic_pointer_cast<const CInstance>(pItem), nModelColumn++, wxDATAVIEW_CELL_ACTIVATABLE));
        }
    }
}

void CConfigurationSummaryModel::OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent)
{
    ETreeItemType eType = rItem.GetType();

    if (EInstance == eType)
        ReloadColumns();
    else if (EVariable == eType)
    {
        m_mParents[&rItem] = rParent.shared_from_this();

        wxDataViewItem item = GetViewItem(dynamic_cast<const CVariable*>(&rItem));
        ItemAdded(GetParent(item), item);
    }
}

void CConfigurationSummaryModel::OnAnyItemErased(const IProjTreeItem& rItem)
{
    ETreeItemType eType = rItem.GetType();

    if (EVariable == eType)
    {
        wxDataViewItem item = GetViewItem(dynamic_cast<const CVariable*>(&rItem));
        ItemDeleted(GetParent(item), item);
    }
}

void CConfigurationSummaryModel::OnAnyItemRenamed(const IProjTreeItem& rItem)
{
    // Rename the columns if any instance was renamed
    const CInstance* const pInst = dynamic_cast<const CInstance*>(&rItem);
    if (pInst)
    {
        unsigned int uColCount = m_pCtrl->GetColumnCount();
        for (unsigned int i = (unsigned int)ESummaryModelColumns::InstanceFirst; i < uColCount; i++)
        {
            CInstanceColumn* const pColumn = dynamic_cast<CInstanceColumn*>(m_pCtrl->GetColumnAt(i));

            if (pColumn && pColumn->GetInstance().get() == pInst)
                pColumn->SetTitle(pInst->GetName());
        }
    }

    // Update the row if any variable was renamed
    const CVariable* const pVariable = dynamic_cast<const CVariable*>(&rItem);
    if (pVariable)
        ItemChanged(GetViewItem(pVariable));
}

unsigned int CConfigurationSummaryModel::GetColumnCount() const
{
    return m_pCtrl->GetColumnCount();
}

bool CConfigurationSummaryModel::HasValue(const wxDataViewItem& item, unsigned col) const
{
    bool bHasValue;
    const CVariable* const pVariable = GetPointer(item);

    switch ((ESummaryModelColumns)col)
    {
    case ESummaryModelColumns::VariableName:
        bHasValue = true;
        break;

    case ESummaryModelColumns::DefaultExpression:
        bHasValue = (pVariable && !pVariable->GetPerInstance());
        break;

    case ESummaryModelColumns::InstanceFirst:
    default:
        bHasValue = (pVariable && pVariable->GetPerInstance());
        break;
    }

    return bHasValue;
}

wxString CConfigurationSummaryModel::GetColumnType(unsigned int col) const
{
    return "wxDataViewIconText";
}

void CConfigurationSummaryModel::GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const
{
    wxDataViewIconText value;
    const CVariable* const pVariable = GetPointer(item);

    if (!pVariable)
        return;

    if (col == (unsigned int)ESummaryModelColumns::VariableName)
    {
        value.SetText(pVariable->GetName());
        value.SetIcon(STreeItemTypeInfo::GetIcon(pVariable->GetType()));
    }
    else
    {
        std::shared_ptr<const CInstance> pInstance;
            
        if (col >= (unsigned int)ESummaryModelColumns::InstanceFirst)
        {
            const CInstanceColumn* const pColumn = CInstanceColumn::GetColumnByModelIndex(col, m_pCtrl);
            if (pColumn)
                pInstance = pColumn->GetInstance();
        }

        if (!pInstance && pVariable->GetPerInstance())
        {
            // *Required* Instance/Column does not exitst
            CVariableCell::SetCellStatus(CVariableCell::ECellStatus::Error, value);
        }
        else
        {
            const IExpression* pExpression = nullptr;
            const CConfiguration* pCurrentConfig = &m_rEdit;

            do
            {
                if (!pCurrentConfig)
                    break; // Reached root item, no more parents
                else
                {
                    pExpression = pVariable->GetRule(pCurrentConfig, pInstance.get());

                    if (!pExpression)
                        pCurrentConfig = std::dynamic_pointer_cast<const CConfiguration>(CParentalMap::GetParent(pCurrentConfig)).get();
                }

            } while (!pExpression);

            if (!pExpression)
                CVariableCell::SetCellStatus(CVariableCell::ECellStatus::Undefined, value);
            else
            {
                CVariableCell::ECellStatus eStatus = (pCurrentConfig == &m_rEdit)
                    ? CVariableCell::ECellStatus::Assign
                    : CVariableCell::ECellStatus::Inherit;

                CVariableCell::SetCellStatus(eStatus, value, pExpression->GetExpression());
            }
        }
    }

    variant << value;
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
    return GetViewItem(nullptr);
}

unsigned int CConfigurationSummaryModel::GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const
{
    unsigned int uCount = 0;

    if (parent.GetID() == nullptr)
    {
        for (const parent_map_t::value_type& rPair : m_mParents)
        {
            const CVariable* pVariable = dynamic_cast<const CVariable*>(rPair.first);

            if (!pVariable)
                continue;

            array.push_back(GetViewItem(pVariable));
            uCount++;
        }
    }

    return uCount;
}