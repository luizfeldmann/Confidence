#include "ui/Models/CVariableTableModel.h"
#include "core/CProject.h"
#include "ui/STreeItemTypeInfo.h"
#include "core/CInstance.h"
#include "core/CVariable.h"

/* CInstanceColumn */

//! @brief Implementation of wxDataViewColumn where the column is associated to a CInstance
class CInstanceColumn : public wxDataViewColumn
{
protected:
    //! @brief ID of the instance associated to this column
    CGuid m_gInstance;

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

CVariableTableModel::CVariableTableModel(CVariable& rVar, const CProject& rProj, wxDataViewCtrl* pCtrl)
    : m_rVar(rVar)
    , m_rProj(rProj)
    , m_pCtrl(pCtrl)
{
    ReloadColumns();
}

CVariableTableModel::~CVariableTableModel()
{

}

const IProjTreeItem& CVariableTableModel::GetRootItem() const
{
    return m_rProj.GetConfigurations();
}

void CVariableTableModel::ReloadColumns()
{
    m_pCtrl->ClearColumns();

    // Create first column for configurations
    const wxIcon& rIconConfig = STreeItemTypeInfo::GetIcon(ETreeItemType::EConfGroup);
    wxDataViewColumn* pColConfig = m_pCtrl->AppendIconTextColumn(rIconConfig, (unsigned int)EVariableColumns::Configuration);
    pColConfig->SetTitle("Configuration");

    // Create one column for each instance
    const CInstanceGroup& rInstances = m_rProj.GetInstances();
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
        const IProjTreeItem* const pItem = GetItem(item);
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

        if (pColumn)
        {
            const CInstance* const pInstance = pColumn->GetInstance();
            const CConfiguration* const pConfig = dynamic_cast<CConfiguration*>((IProjTreeItem*)item.GetID());

            if (pConfig && pInstance)
            {
                const IExpression* const pExpression = m_rVar.GetRule(*pConfig, *pInstance);
                if (pExpression)
                    value.SetText( pExpression->GetExpression() );
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