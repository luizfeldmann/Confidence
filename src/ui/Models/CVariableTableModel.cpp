#include "ui/Models/CVariableTableModel.h"
#include "core/CProject.h"
#include "ui/STreeItemTypeInfo.h"
#include "core/CInstance.h"
#include "core/CVariable.h"
#include "ui/Models/CBaseTreeItemModel.h"

/* CInstanceColumn */

//! @brief Implementation of wxDataViewColumn where the column is associated to a CInstance
class CInstanceColumn : public wxDataViewColumn
{
protected:
    //! @brief ID of the instance associated to this column
    CInstance::id_type m_gInstance;

    //! @brief Returns the icon of the column
    inline static const wxIcon& GetIcon()
    {
        return STreeItemTypeInfo::GetIcon(ETreeItemType::EInstance);
    }

public:
    //! @brief Creates a column related to a given instance
    //! @param[in] rInst The instance associated to this column
    //! @param[in] nModelColumn Index used to track this column in the model
    CInstanceColumn(const CInstance& rInst, unsigned int nModelColumn)
        : wxDataViewColumn(GetIcon(), new wxDataViewIconTextRenderer(wxDataViewIconTextRenderer::GetDefaultType(), wxDATAVIEW_CELL_EDITABLE), nModelColumn)
        , m_gInstance(rInst.GetID())
    {
        const std::string strInstanceName = rInst.GetName();
        SetTitle(strInstanceName);
    }

    //! @brief Gets a pointer to the associated instance, if any
    CInstance* GetInstance() const
    {
        return CInstance::FindByID(m_gInstance);
    }

    //! @brief Gets the ID of the associated instance
    CInstance::id_type GetID() const
    {
        return m_gInstance;
    }

    //! @brief Searches the wxDataViewCtrl for the column with the given model index
    //! @param[in] nModelColumn Index in the model of the column being searched
    //! @param[in] pCtrl Pointer to the control containing the searched column
    static CInstanceColumn* GetColumnByModelIndex(unsigned nModelColumn, const wxDataViewCtrl* const pCtrl)
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
};

/* CVariableTableModel */

//! @brief The model column indices
enum class EVariableColumns : int
{
    Configuration   = 0,    //!< The 0-th column lists the configurations
    InstanceFirst   = 1     //!< The 1-st to last columns are instances 
};

//! @brief Possible status for the expression cells of the variable
enum class ECellStatus {
    Assign,     //!< An expression is set for the cell
    Inherit,    //!< The cell inherits the expression from the parent configuration
    Undefined,  //!< The expression is undefined
    Error,      //!< The expression rule is invalid
};

//! @brief Updates a table view cell text and icon
static void SetCellStatus(ECellStatus eStatus, wxDataViewIconText& rCell, const wxString strLabel = wxEmptyString)
{
    struct SCellStyle
    {
        const wxString strText;
        const wxIcon cIcon;
    };
    
    #define CELL_ICON(NAME) wxIcon(NAME, wxBITMAP_TYPE_ICO_RESOURCE, 16, 16)

    static const std::map<ECellStatus, SCellStyle> c_mIcons {
        {ECellStatus::Assign,    SCellStyle{"<set empty>",     CELL_ICON("RES_ID_ICON_EXPR_ASSIGN")} },
        {ECellStatus::Inherit,   SCellStyle{"<inherit empty>", CELL_ICON("RES_ID_ICON_EXPR_INHERIT")} },
        {ECellStatus::Undefined, SCellStyle{"<undefined>",     CELL_ICON("RES_ID_ICON_EXPR_UNDEFINED")} },
        {ECellStatus::Error,     SCellStyle{"<error>",         CELL_ICON("RES_ID_ICON_EXPR_ERROR")} },
    };

    const SCellStyle& rStyle = c_mIcons.at(eStatus);

    rCell.SetText(strLabel.empty() ? rStyle.strText : strLabel);
    rCell.SetIcon(rStyle.cIcon);
}

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

    return pProj->GetConfigurations();
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

            if (pColumn && pColumn->GetID() == pInst->GetID())
                pColumn->SetTitle(pInst->GetName());
        }
    }

    // Rename the row if a configuration was renamed
    const CConfiguration* const pConfig = dynamic_cast<const CConfiguration*>(&rItem);
    if (pConfig)
        ItemChanged(CBaseTreeItemModel::GetViewItem(pConfig));
}

void CVariableTableModel::ReloadColumns()
{
    m_pCtrl->ClearColumns();

    // Create first column for configurations
    const STreeItemTypeInfo& rHeaderInfo = STreeItemTypeInfo::GetInfo(ETreeItemType::EConfGroup);
    wxDataViewColumn* pColConfig = m_pCtrl->AppendIconTextColumn(rHeaderInfo.m_icon, (unsigned int)EVariableColumns::Configuration);
    pColConfig->SetTitle(rHeaderInfo.m_strTypeName);

    // Create one column for each instance
    std::shared_ptr<const CProject> pProj = m_pProj.lock();
    assert(pProj);

    const CInstanceGroup& rInstances = pProj->GetInstances();
    const ITreeItemCollection::vec_cref_t vSubItems = rInstances.SubItems();

    unsigned int nModelColumn = (unsigned int)EVariableColumns::InstanceFirst;
    for (const ITreeItemCollection::cref_t& rItem : vSubItems)
    {
        assert(ETreeItemType::EInstance == rItem.get().GetType());
        const CInstance& rInst = dynamic_cast<const CInstance&>(rItem.get());

        m_pCtrl->AppendColumn( new CInstanceColumn(rInst, nModelColumn++) );
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
        const CInstanceColumn* const pColumn = CInstanceColumn::GetColumnByModelIndex(col, m_pCtrl);
        const CInstance* const pInstance = pColumn ? pColumn->GetInstance() : nullptr;

        if (!pInstance)
        {
            // Instance/Column does not exitst
            SetCellStatus(ECellStatus::Error, value);
        }
        else
        {
            const IExpression* pExpression = nullptr;
            wxDataViewItem currentItem = item;

            do
            {
                const CConfiguration* pConfig = dynamic_cast<CConfiguration*>((IProjTreeItem*)currentItem.GetID());

                if (!pConfig)
                    break; // Reached root item, no more parents
                else
                {
                    pExpression = m_rVar.GetRule(*pConfig, *pInstance);

                    if (!pExpression)
                        currentItem = GetParent(currentItem);
                }

            } while (!pExpression);

            if (!pExpression)
                SetCellStatus(ECellStatus::Undefined, value);
            else
                SetCellStatus(currentItem == item ? ECellStatus::Assign : ECellStatus::Inherit, value, pExpression->GetExpression());
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
        const CInstance* const pInstance = pColumn->GetInstance();
        const CConfiguration* const pConfig = dynamic_cast<CConfiguration*>((IProjTreeItem*)item.GetID());

        if (pConfig && pInstance)
        {
            IExpression* pExpression = m_rVar.GetRule(*pConfig, *pInstance);
            
            if (!pExpression)
                pExpression = &m_rVar.AddRule(*pConfig, *pInstance);

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
        const CInstance* const pInstance = pColumn->GetInstance();
        const CConfiguration* const pConfig = dynamic_cast<CConfiguration*>((IProjTreeItem*)rCellRow.GetID());

        if (pConfig && pInstance)
        {
            IExpression* const pExpression = m_rVar.GetRule(*pConfig, *pInstance);

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