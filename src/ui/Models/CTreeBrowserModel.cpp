#include "ui/Models/CTreeBrowserModel.h"
#include "ui/STreeItemTypeInfo.h"
#include "util/Log.h"

/// =======================================================
/// Global definitions
/// =======================================================

wxDEFINE_EVENT(EVT_TREE_ITEM_RENAME, wxCommandEvent);

//! @brief All items are enabled
bool IsEnabled(const wxDataViewItem&)
{
    return true;
}

/// =======================================================
/// Handlers
/// =======================================================

class CNameModelColumnHandler : public IModelColumnHandler
{
protected:
    wxEvtHandler& m_rEvtHandler;

public:
    CNameModelColumnHandler(wxEvtHandler& rEvtHandler)
        : m_rEvtHandler(rEvtHandler)
    {

    }

    wxString GetType() const override
    {
        return "wxDataViewIconText";
    }

    void GetValue(wxVariant& value, const wxDataViewItem& rItem) const override
    {
        const IProjTreeItem* pItem = CBaseTreeItemModel::GetPointer(rItem);
        assert(pItem);

        #define REGICON(RESNAME) wxIcon(RESNAME, wxBITMAP_TYPE_ICO_RESOURCE, 24, 24)

        const wxString strName = pItem->GetName();
        const wxIcon& rIcon = STreeItemTypeInfo::GetIcon(pItem->GetType());
        wxDataViewIconText iconAndText(strName, rIcon);
        value = wxVariant(iconAndText);
    }

    bool IsEnabled(const wxDataViewItem&) const
    {
        return true;
    }

    bool SetValue(const wxVariant& value, const wxDataViewItem& rItem)
    {
        IProjTreeItem* pItem = CBaseTreeItemModel::GetPointer(rItem);
        assert(pItem);

        wxDataViewIconText iconAndText;
        iconAndText << value;

        bool bSuccess = pItem->SetName(iconAndText.GetText().ToStdString());

        if (bSuccess)
        {
            wxCommandEvent evtNotifyRename(EVT_TREE_ITEM_RENAME);
            evtNotifyRename.SetClientData(pItem);
            m_rEvtHandler.ProcessEvent(evtNotifyRename);
        }

        return bSuccess;
    }
};

class CDescriptionModelColumnHandler : public IModelColumnHandler
{
protected:
    wxEvtHandler& m_rEvtHandler;

public:
    CDescriptionModelColumnHandler(wxEvtHandler& rEvtHandler)
        : m_rEvtHandler(rEvtHandler)
    {

    }

    wxString GetType() const override
    {
        return "string";
    }

    void GetValue(wxVariant& value, const wxDataViewItem& rItem) const override
    {
        const IProjTreeItem* const pItem = CBaseTreeItemModel::GetPointer(rItem);
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
        return false;
    }
};

/// =======================================================
/// Model logic
/// =======================================================

CTreeBrowserModel::CTreeBrowserModel(std::weak_ptr<const IProjTreeItem> pNewRoot)
    : m_pRootItem(pNewRoot)
{
    m_mapColumns[ETreeBrowserColumn::IconAndName] = std::make_unique<CNameModelColumnHandler>(*this);
    m_mapColumns[ETreeBrowserColumn::Description] = std::make_unique<CDescriptionModelColumnHandler>(*this);
}

CTreeBrowserModel::~CTreeBrowserModel()
{

}

IModelColumnHandler* CTreeBrowserModel::GetColumnInfo(unsigned int nModelColumn)
{
    IModelColumnHandler* pInfo = nullptr;

    const auto itFound = m_mapColumns.find((ETreeBrowserColumn)nModelColumn);
    if (m_mapColumns.end() != itFound)
        pInfo = itFound->second.get();

    return pInfo;
}

const IModelColumnHandler* CTreeBrowserModel::GetColumnInfo(unsigned int nModelColumn) const
{
    const IModelColumnHandler* pInfo = nullptr;

    const auto itFound = m_mapColumns.find((ETreeBrowserColumn)nModelColumn);
    if (m_mapColumns.cend() != itFound)
        pInfo = itFound->second.get();

    return pInfo;
}

void CTreeBrowserModel::SetRoot(std::weak_ptr<const IProjTreeItem> pNewRoot)
{
    m_pRootItem = pNewRoot;
    Cleared();
}

const IProjTreeItem& CTreeBrowserModel::GetRootItem() const
{
    std::shared_ptr<const IProjTreeItem> pRoot = m_pRootItem.lock();
    assert(pRoot && "Tree model has no root item");

    return *pRoot.get();
}

/// =======================================================
/// Model interface overrides
/// =======================================================

unsigned int CTreeBrowserModel::GetColumnCount() const
{
    return (unsigned int)ETreeBrowserColumn::Count;
}

bool CTreeBrowserModel::IsContainer(const wxDataViewItem& item) const
{
    bool bIsContainer;

    if (NULL == item.GetID())
        bIsContainer = true; // This is the invisible root node, which contains the project
    else
    {
        const IProjTreeItem* const pItem = GetPointer(item);
        assert(nullptr != pItem);

        bIsContainer = (EInvalid != pItem->GetSupportedChildTypes());
    }

    return bIsContainer;
}

/// =======================================================
/// Operations
/// =======================================================


ITreeItemCollection::ptr_t CTreeBrowserModel::TakeItem(const wxDataViewItem& rItem)
{
    ITreeItemCollection::ptr_t pTakeItem;

    IProjTreeItem* const pChild = GetPointer(rItem);
    const wxDataViewItem cItemParent = GetParent(rItem);
    IProjTreeItem* pParent = GetPointer(cItemParent);

    if (!pChild || !pParent)
        CWARNING("You must select a child item for this operation");
    else
    {
        pTakeItem = pParent->TakeItem(*pChild);

        if (pTakeItem)
        {
            ItemDeleted(cItemParent, rItem);
        }
        else
        {
            const std::string strName = pChild->GetName();
            CWARNING("Item '%s' cannot be removed from the collection", strName.c_str());
        }
    }

    return pTakeItem;
}

bool CTreeBrowserModel::MoveItem(const wxDataViewItem& rItem, bool bUp)
{
    bool bSuccess = false;

    // Ensure there is a selected item for us to operate
    // If the parent is root, then the project itself is selected
    // We cannot move/cut/delete the project itself
    IProjTreeItem* const pChild = GetPointer(rItem);
    const wxDataViewItem cParent = GetParent(rItem);
    IProjTreeItem* const pParent = GetPointer(cParent);

    if (!pChild || !pParent)
        CWARNING("Cannot move the root item");
    else
    {
        // Find the iterator to this item inside it's parent vector
        IProjTreeItem::vec_ref_t& vSubitems = pParent->SubItems();

        IProjTreeItem::vec_ref_t::iterator iterChild = std::find_if(vSubitems.begin(), vSubitems.end(),
            [pChild](const IProjTreeItem::ref_t pSearch)->bool {
                return (&pSearch.get() == pChild);
            });

        assert(vSubitems.cend() != iterChild);

        IProjTreeItem::vec_ref_t::iterator iterSwap;
        if (bUp)
        {
            iterSwap = (vSubitems.begin() == iterChild)
                ? std::prev(vSubitems.end()) // Move up 1st item = wrap around to the end
                : std::prev(iterChild);
        }
        else
        {
            iterSwap = std::next(iterChild);

            // If move down last item - wrap around to the front
            if (vSubitems.cend() == iterSwap)
                iterSwap = vSubitems.begin();
        }

        if (pParent->SwapItems(*iterSwap, *iterChild))
        {
            ItemDeleted(cParent, rItem);
            ItemAdded(cParent, rItem);

            bSuccess = true;
        }
    }

    return bSuccess;
}

bool CTreeBrowserModel::InsertItem(const wxDataViewItem& rParent, IProjTreeItem* pInsertItem)
{
    bool bSuccess = false;

    IProjTreeItem* const pParent = GetPointer(rParent);

    if (!pParent || !pInsertItem)
        CERROR("Item is NULL");
    else
    {
        // Make sure the parent can accept the child
        const ETreeItemType eSupportedTypes = pParent->GetSupportedChildTypes();
        const ETreeItemType eInsertItemType = pInsertItem->GetType();

        if (0 == (eSupportedTypes & eInsertItemType))
        {
            const ETreeItemType eParentItemType = pParent->GetType();

            const wxScopedCharBuffer childTypeName = STreeItemTypeInfo::GetName(eInsertItemType).ToUTF8();
            const wxScopedCharBuffer parentTypeName = STreeItemTypeInfo::GetName(eParentItemType).ToUTF8();

            CWARNING("Cannot paste an item of type '%s' inside a parent of type '%s'",
                childTypeName.data(),
                parentTypeName.data());
        }
        else if (pParent->AddItem(pInsertItem))
        {
            ItemAdded(rParent, GetViewItem(pInsertItem));
            bSuccess = true;
        }
        else
        {
            CERROR("Parent item refused to accept new child");
        }
    }

    return bSuccess;
}