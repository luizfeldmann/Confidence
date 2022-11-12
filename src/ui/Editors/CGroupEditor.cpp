#include "ui/Editors/CGroupEditor.h"
#include "ui/CMainWindow.h"
#include "core/items/CProject.h"
#include "core/items/group/CGroup.h"
#include "core/items/CConfigurationGroup.h"
#include "core/items/CInstanceGroup.h"
#include "core/items/group/CGroupFilterConfigurationAndInstance.h"

//! @brief Utility to create filters by provided type
class CFilterFactory
{
public:
    using ptrFilter_t = std::shared_ptr<IGroupFilter>;
    using fnFactory_t = std::function<ptrFilter_t()>;
    using pair_t = std::pair<ETreeItemType, fnFactory_t>;
    using collection_t = std::vector<pair_t>;

    //! @brief Creates a new filter of the requested type
    static ptrFilter_t Create(ETreeItemType eType)
    {
        return Find(GetCollection(), eType)->second();
    }

    //! @brief Converts the filter type to an index
    static unsigned int GetIndex(ETreeItemType eType)
    {
        const collection_t& rCol = GetCollection();
        return std::distance(rCol.cbegin(), Find(rCol, eType));
    }

    //! @brief Converts an index to the filter type
    static ETreeItemType FromIndex(unsigned int uIndex)
    {
        const collection_t& rCol = GetCollection();
        return std::next(rCol.cbegin(), uIndex)->first;
    }

protected:
    //! @brief Helper factory method for the "No filter" case
    static ptrFilter_t CreateNone()
    {
        return nullptr;
    }

    //! @brief Retrives the stored collection of associated filter types versus factory methods
    static const collection_t& GetCollection()
    {
        static const collection_t c_collection
        {
            {EInvalid,                                            CreateNone},
            {CGroupFilterConfiguration::GetOwnType(),             &CGroupFilterConfiguration::Create},
            {CGroupFilterInstance::GetOwnType(),                  &CGroupFilterInstance::Create},
            {CGroupFilterConfigurationAndInstance::GetOwnType(),  &CGroupFilterConfigurationAndInstance::Create}
        };

        return c_collection;
    }

    //! @brief Checks if an associated pair matches the filter type
    static bool Compare(const pair_t& pair, ETreeItemType eType)
    {
        return pair.first == eType;
    }

    //! @brief Finds the associative collection iterator for the entry of the provided filter type
    static collection_t::const_iterator Find(const collection_t& rCol, ETreeItemType eType)
    {
        return std::find_if(rCol.cbegin(), rCol.cend(), std::bind(&Compare, std::placeholders::_1, eType));
    }
};

/* CGroupEditorUI */
CGroupEditorUI::CGroupEditorUI(wxWindow* pParent, CGroup& rEdit, std::shared_ptr<const CProject> pProject)
    : IGroupEditor(pParent)
    , m_rEdit(rEdit)
    , m_pProject(pProject)
{
    ReloadFilterType();
}

void CGroupEditorUI::OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent)
{
    OnItemOperation(rItem);
}

void CGroupEditorUI::OnAnyItemErased(const IProjTreeItem& rItem)
{
    OnItemOperation(rItem);
}

void CGroupEditorUI::OnAnyItemRenamed(const IProjTreeItem& rChanged)
{
    OnItemOperation(rChanged);
}

void CGroupEditorUI::OnItemOperation(const IProjTreeItem& rItem)
{
    const ETreeItemType eType = rItem.GetType();

    if (eType & CGroupFilterConfiguration::GetOwnType())
        PopulateConfigurationsList(m_rEdit.GetFilter());
    
    if (eType & CGroupFilterInstance::GetOwnType())
        PopulateInstancesList(m_rEdit.GetFilter());
}

void CGroupEditorUI::onChangeFilter(wxCommandEvent& event)
{
    // Get the type of the current filter
    ETreeItemType eCurrentType = EInvalid;
    {
        std::shared_ptr<const IGroupFilter> pCurrentFilter = m_rEdit.GetFilter();
        if (pCurrentFilter)
            eCurrentType = pCurrentFilter->GetType();
    }

    // Create new filter if type changed
    const ETreeItemType eNewType = CFilterFactory::FromIndex(m_choiceFilter->GetSelection());

    if (eNewType != eCurrentType)
    {
        CFilterFactory::ptrFilter_t pNewFilter = CFilterFactory::Create(eNewType);
        m_rEdit.SetFilter(pNewFilter);
        ReloadFilterDetails(pNewFilter);
    }
}

void CGroupEditorUI::ReloadFilterType()
{
    // Get the type of the current filter
    const std::shared_ptr<const IGroupFilter> pFilter = m_rEdit.GetFilter();
    const ETreeItemType eType = pFilter ? pFilter->GetType() : EInvalid;

    // Update the combo box
    m_choiceFilter->SetSelection(CFilterFactory::GetIndex(eType));

    // Reload special details of each filter type
    ReloadFilterDetails(pFilter);
}

void CGroupEditorUI::ReloadFilterDetails(std::shared_ptr<const IGroupFilter> pFilter)
{
    const ETreeItemType eType = pFilter ? pFilter->GetType() : EInvalid;

    // Load the list of configurations
    {
        bool bShowConfList = false;
        if (eType & CGroupFilterConfiguration::GetOwnType())
        {
            bShowConfList = true;
            PopulateConfigurationsList(pFilter);
        }

        sbSizerGroupFilterConfigurations->Show(bShowConfList);
    }

    // Load the list of instances
    {
        bool bShowInstList = false;
        if (eType & CGroupFilterInstance::GetOwnType())
        {
            bShowInstList = true;
            PopulateInstancesList(pFilter);
        }

        sbSizerGroupFilterInstances->Show(bShowInstList);
    }

    Layout();
}

void CGroupEditorUI::PopulateInstancesList(std::shared_ptr<const IGroupFilter> pFilter)
{
    m_checkListInsts->Clear();

    assert(m_pProject);
    const IProjTreeItem::vec_cptr_t vInstances = m_pProject->GetInstances()->SubItems();
    for (const IProjTreeItem::cptr_t& pInst : vInstances)
    {
        AppendFilterListItem(pFilter, pInst, m_checkListInsts);
    }
}

static void FlattenTree(IProjTreeItem::vec_cptr_t& vItems, const IProjTreeItem::cptr_t& pRoot)
{
    vItems.push_back(pRoot);

    for (const IProjTreeItem::cptr_t& pChild : pRoot->SubItems())
        FlattenTree(vItems, pChild);
}

void CGroupEditorUI::PopulateConfigurationsList(std::shared_ptr<const IGroupFilter> pFilter)
{
    m_checkListConfigs->Clear();

    assert(m_pProject);
    IProjTreeItem::vec_cptr_t vConfigs;
    FlattenTree(vConfigs, m_pProject->GetConfigurations());

    for (const IProjTreeItem::cptr_t& pConfig : vConfigs)
    {
        AppendFilterListItem(pFilter, pConfig, m_checkListConfigs);
    }
}

void CGroupEditorUI::AppendFilterListItem(std::shared_ptr<const IGroupFilter> pFilter, IProjTreeItem::cptr_t pItem, wxCheckListBox* pListBox)
{
    assert(pItem);
    assert(pListBox);
    const int nIndex = pListBox->Append(pItem->GetName(), (void*)pItem.get());

    const bool bCheck = pFilter ? pFilter->Filter(pItem) : false;
    pListBox->Check(nIndex, bCheck);
}

void CGroupEditorUI::onConfigListToggle(wxCommandEvent& event)
{
    OnListToggle(event, m_checkListConfigs);
}

void CGroupEditorUI::onInstanceListToggle(wxCommandEvent& event)
{
    OnListToggle(event, m_checkListInsts);
}

void CGroupEditorUI::OnListToggle(wxCommandEvent& event, wxCheckListBox* pListBox)
{
    assert(pListBox);
    const int nCheckIndex = event.GetInt();
    const bool bChecked = pListBox->IsChecked(nCheckIndex);
    
    IProjTreeItem::cptr_t pItem = 
        static_cast<const IProjTreeItem*>(pListBox->GetClientData(nCheckIndex))->shared_from_this();

    assert(pItem);

    std::shared_ptr<IGroupFilter> pFilter = m_rEdit.GetFilter();
    assert(pFilter);

    if (bChecked)
        pFilter->push_back(pItem);
    else
        pFilter->erase(pItem);
}

/* CGroupEditor */
CGroupEditor::CGroupEditor(CMainWindow& rMainWindow, IProjTreeItem& rItem)
    : CNameDescEditor(rMainWindow, rItem)
    , m_pUiGroup(new CGroupEditorUI(m_pPanel, dynamic_cast<CGroup&>(rItem), rMainWindow.GetProject() ))
{
    m_pSizer->Add(m_pUiGroup, 0, wxEXPAND, 5);
    m_pPanel->Layout();
}

ITreeItemEditor* CGroupEditor::Create(CMainWindow& rMainWindow, IProjTreeItem& rItem)
{
    return new CGroupEditor(rMainWindow, rItem);
}

void CGroupEditor::OnItemCreated(const IProjTreeItem& rItem, const IProjTreeItem& rParent)
{
    CNameDescEditor::OnItemCreated(rItem, rParent);
    m_pUiGroup->OnItemCreated(rItem, rParent);
}

void CGroupEditor::OnAnyItemErased(const IProjTreeItem& rItem)
{
    CNameDescEditor::OnAnyItemErased(rItem);
    m_pUiGroup->OnAnyItemErased(rItem);
}

void CGroupEditor::OnAnyItemRenamed(const IProjTreeItem& rChanged)
{
    CNameDescEditor::OnAnyItemRenamed(rChanged);
    m_pUiGroup->OnAnyItemRenamed(rChanged);
}