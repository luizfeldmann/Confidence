/**
 * Copyright (c) 2022 Luiz Gustavo Pfitscher e Feldmann
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 */

#include "ui/Models/CVariableTableModel.h"
#include "core/items/CProject.h"
#include "ui/STreeItemTypeInfo.h"
#include "core/items/CInstance.h"
#include "core/items/CInstanceGroup.h"
#include "core/items/CConfiguration.h"
#include "core/items/variable/CVariable.h"
#include "core/CParentalMap.h"
#include "ui/Models/CBaseTreeItemModel.h"
#include "ui/Models/CInstanceColumn.h"
#include "ui/Models/CVariableCell.h"

/* CVariableTableModel */

//! @brief The model column indices
enum class EVariableColumns : int
{
    Configuration   = 0,    //!< The 0-th column lists the configurations
    InstanceFirst   = 1     //!< The 1-st to last columns are instances 
};

CVariableTableModel::CVariableTableModel(CVariable& rVar, std::shared_ptr<const CProject> pProj, wxDataViewCtrl* pCtrl)
    : m_rVar(rVar)
    , m_pProj(pProj)
    , m_pCtrl(pCtrl)
{
    ReloadColumns();
}

CVariableTableModel::~CVariableTableModel()
{

}

const IProjTreeItem& CVariableTableModel::GetRootItem() const
{
    std::shared_ptr<const CProject> pProj = m_pProj.lock();
    assert(pProj);

    return *pProj->GetConfigurations();
}

void CVariableTableModel::OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent)
{
    // If an instance was created, reload the columns
    // If a configuration was created, append the row
    const ETreeItemType eType = rItem.GetType();
    if (EInstance == eType)
        ReloadColumns();
    else if (EConfig == eType)
    {
        ItemAdded(CBaseTreeItemModel::GetViewItem(&rParent),
            CBaseTreeItemModel::GetViewItem(&rItem));
    }
}

void CVariableTableModel::OnAnyItemErased(const IProjTreeItem& rItem)
{
    if (EConfig == rItem.GetType())
    {
        wxDataViewItem deletedItem = GetViewItem(&rItem);
        ItemDeleted(GetParent(deletedItem), deletedItem);
    }
}

void CVariableTableModel::OnAnyItemRenamed(const IProjTreeItem& rItem)
{
    // Rename the columns if any instance was renamed
    const CInstance* const pInst = dynamic_cast<const CInstance*>(&rItem);
    if (pInst)
    {
        unsigned int uColCount = m_pCtrl->GetColumnCount();
        for (unsigned int i = 0; i < uColCount; i++)
        {
            CInstanceColumn* const pColumn = dynamic_cast<CInstanceColumn*>(m_pCtrl->GetColumnAt(i));

            if (pColumn && pColumn->GetInstance().get() == pInst)
                pColumn->SetTitle(pInst->GetName());
        }
    }

    // Rename the row if a configuration was renamed
    const CConfiguration* const pConfig = dynamic_cast<const CConfiguration*>(&rItem);
    if (pConfig)
    {
        // Calling ItemChanged() on an item that was never expanded may cause an 'invalid item' error
        // The chain of parents until the relevant item is reached
        std::list<const IProjTreeItem*> parentBranch;
        CParentalMap::GetParentBranch(m_pProj.lock(), pConfig, std::front_inserter(parentBranch));

        for (const IProjTreeItem* pItem : parentBranch)
            m_pCtrl->Expand(CBaseTreeItemModel::GetViewItem(pItem));

        const auto aItem = CBaseTreeItemModel::GetViewItem(pConfig);
        ItemChanged(aItem);
        m_pCtrl->Select(aItem);
    }
}

void CVariableTableModel::ReloadColumns()
{
    m_pCtrl->ClearColumns();

    // Create first column for configurations
    const STreeItemTypeInfo& rHeaderInfo = STreeItemTypeInfo::GetInfo(ETreeItemType::EConfGroup);
    wxDataViewColumn* pColConfig = m_pCtrl->AppendIconTextColumn(rHeaderInfo.m_icon, (unsigned int)EVariableColumns::Configuration);
    pColConfig->SetTitle(rHeaderInfo.m_strTypeName);

    // Create one column for each instance
    unsigned int nModelColumn = (unsigned int)EVariableColumns::InstanceFirst;

    if (m_rVar.GetPerInstance())
    {
        std::shared_ptr<const CProject> pProj = m_pProj.lock();
        assert(pProj);

        const ITreeItemCollection::vec_cptr_t vSubItems = pProj->GetInstances()->SubItems();

        for (const ITreeItemCollection::cptr_t& pItem : vSubItems)
        {
            assert(pItem && ETreeItemType::EInstance == pItem->GetType());

            m_pCtrl->AppendColumn(new CInstanceColumn(std::dynamic_pointer_cast<const CInstance>(pItem), nModelColumn++, wxDATAVIEW_CELL_EDITABLE));
        }
    }
    else
    {
        // Create a single column for default expression
        m_pCtrl->AppendColumn(new CInstanceColumn(nModelColumn++, wxDATAVIEW_CELL_EDITABLE));
    }
}

/* wxDataViewModel overrides */
unsigned int CVariableTableModel::GetColumnCount() const
{
    return m_pCtrl->GetColumnCount();
}

wxString CVariableTableModel::GetColumnType(unsigned int col) const
{
    return "wxDataViewIconText";
}

bool CVariableTableModel::HasValue(const wxDataViewItem& item, unsigned col) const
{
    return true;
}

bool CVariableTableModel::IsEnabled(const wxDataViewItem& item, unsigned int col) const
{
    return true;
}

bool CVariableTableModel::IsContainer(const wxDataViewItem& item) const
{
    return true;
}

void CVariableTableModel::GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const
{
    wxDataViewIconText value;

    if ((unsigned)EVariableColumns::Configuration == col)
    {
        // First column - name of the configuration
        const IProjTreeItem* const pItem = GetPointer(item);
        assert(nullptr != pItem);

        const std::string strName = pItem->GetName();
        value.SetText(strName);

        const wxIcon& rIcon = STreeItemTypeInfo::GetIcon(pItem->GetType());
        value.SetIcon(rIcon);
    }
    else
    {
        // Other columns - associated to instances
        
        std::shared_ptr<const CInstance> pInstance; 
        {
            const CInstanceColumn* const pColumn = CInstanceColumn::GetColumnByModelIndex(col, m_pCtrl);
            if (pColumn)
                pInstance = pColumn->GetInstance();
        }

        if (!pInstance && m_rVar.GetPerInstance())
        {
            // *Required* Instance/Column does not exitst
            CVariableCell::SetCellStatus(CVariableCell::ECellStatus::Error, value);
        }
        else
        {
            const IExpression* pExpression = nullptr;
            wxDataViewItem currentItem = item;

            do
            {
                const CConfiguration* pConfig = dynamic_cast<const CConfiguration*>(GetPointer(currentItem));

                if (!pConfig)
                    break; // Reached root item, no more parents
                else
                {
                    pExpression = m_rVar.GetRule(pConfig, pInstance.get());

                    if (!pExpression)
                        currentItem = GetParent(currentItem);
                }

            } while (!pExpression);

            if (!pExpression)
                CVariableCell::SetCellStatus(CVariableCell::ECellStatus::Undefined, value);
            else
            {
                CVariableCell::ECellStatus eStatus = (currentItem == item)
                    ? CVariableCell::ECellStatus::Assign
                    : CVariableCell::ECellStatus::Inherit;

                CVariableCell::SetCellStatus(eStatus, value, pExpression->GetExpression());
            }
        }
    }

    variant = wxVariant(value);
}

bool CVariableTableModel::SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col)
{
    bool bSuccess = false;

    const CInstanceColumn* const pColumn = CInstanceColumn::GetColumnByModelIndex(col, m_pCtrl);

    if (pColumn)
    {
        std::shared_ptr<const CInstance> const pInstance = pColumn->GetInstance();
        std::shared_ptr<const CConfiguration> const pConfig = std::dynamic_pointer_cast<const CConfiguration>( GetPointer(item)->shared_from_this() );

        if (pConfig && (pInstance || !m_rVar.GetPerInstance()))
        {
            IExpression* pExpression = m_rVar.GetRule(pConfig.get(), pInstance.get());
            
            if (!pExpression)
                pExpression = &m_rVar.AddRule(pConfig, pInstance);

            wxDataViewIconText value;
            value << variant;

            bSuccess = pExpression->SetExpression(value.GetText().ToStdString());
        }
    }

    return bSuccess;
}

bool CVariableTableModel::DeleteCell(const wxDataViewItem& rCellRow, const wxDataViewColumn* pCellColumn)
{
    bool bResult = false;

    const CInstanceColumn* const pColumn = dynamic_cast<const CInstanceColumn*>(pCellColumn);

    if (pColumn)
    {
        std::shared_ptr<const CInstance> const pInstance = pColumn->GetInstance();
        const CConfiguration* const pConfig = dynamic_cast<CConfiguration*>(GetPointer(rCellRow));

        if (pConfig && pInstance)
        {
            IExpression* const pExpression = m_rVar.GetRule(pConfig, pInstance.get());

            if (pExpression)
            {
                bResult = m_rVar.EraseRule(pExpression);
                
                if (bResult)
                    ItemChanged(rCellRow);
            }
        }
    }

    return bResult;
}