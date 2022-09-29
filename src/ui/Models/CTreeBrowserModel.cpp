#include "ui/Models/CTreeBrowserModel.h"
#include "ui/STreeItemTypeInfo.h"
#include "core/CProject.h"

/// =======================================================
/// Global definitions
/// =======================================================

wxDEFINE_EVENT(EVT_TREE_ITEM_RENAME, wxCommandEvent);

//! @brief All items are enabled
bool IsEnabled(const wxDataViewItem&)
{
    return true;
}

//! @brief Casts the DataViewItem into a ProjectItem
static inline IProjTreeItem* GetItem(const wxDataViewItem& item)
{
    return static_cast<IProjTreeItem*>(item.GetID());
}

//! Maps child (key) to parent (value) - this two-way relation does not exist in the project structure
static std::map<wxDataViewItem, wxDataViewItem> g_mapParents;

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
        IProjTreeItem* pItem = GetItem(rItem);
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
        IProjTreeItem* pItem = GetItem(rItem);
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
        IProjTreeItem* pItem = GetItem(rItem);
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

CTreeBrowserModel::CTreeBrowserModel()
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

/// =======================================================
/// Model interface overrides
/// =======================================================

unsigned int CTreeBrowserModel::GetColumnCount() const
{
    return (unsigned int)ETreeBrowserColumn::Count;
}

wxDataViewItem CTreeBrowserModel::GetParent(const wxDataViewItem& item) const
{
    return g_mapParents[item];
}

bool CTreeBrowserModel::IsContainer(const wxDataViewItem& item) const
{
    bool bIsContainer;

    if (NULL == item.GetID())
        bIsContainer = true; // This is the invisible root node, which contains the project
    else
    {
        const IProjTreeItem* pItem = GetItem(item);
        assert(nullptr != pItem);

        bIsContainer = (EInvalid != pItem->GetSupportedChildTypes());
    }

    return bIsContainer;
}

unsigned int CTreeBrowserModel::GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const
{
    unsigned int uiCount = 0;
    array.clear();

    if (NULL == parent.GetID())
    {
        wxDataViewItem item(&CProject::TheProject());
        array.push_back(item);
        uiCount = 1;

        g_mapParents[item] = parent;
    }
    else
    {
        const IProjTreeItem* pParent = GetItem(parent);
        assert(nullptr != pParent);

        ITreeItemCollection::vec_cref_t vSubitems = pParent->SubItems();
        for (const ITreeItemCollection::cref_t& rChild : vSubitems)
        {
            const IProjTreeItem* pChild = &rChild.get();
            wxDataViewItem child((void*)pChild);
            array.push_back(child);
            uiCount++;

            g_mapParents[child] = parent;
        }
    }

    return uiCount;
}