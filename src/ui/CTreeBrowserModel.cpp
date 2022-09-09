#include "ui/CTreeBrowserModel.h"
#include "ui/STreeItemTypeInfo.h"
#include "core/CProject.h"

/// =======================================================
/// Global definitions
/// =======================================================

wxDEFINE_EVENT(EVT_TREE_ITEM_RENAME, wxCommandEvent);

//! @brief Defines the columns shown in the tree browser
enum class ETreeBrowserColumn
{
    IconAndName = 0,
    Description,

    /* MUST BE LAST */
    Count
};

//! @brief Handling of each column in the tree view
struct STreeBrowserColumnInfo
{
    using fnGetValue_t = void (CTreeBrowserModel::*)(wxVariant&, const IProjTreeItem*) const;
    using fnSetValue_t = bool (CTreeBrowserModel::*)(const wxVariant&, IProjTreeItem*);

    wxString m_strType;
    fnGetValue_t m_fnGetValue;
    fnSetValue_t m_fnSetValue;
};

//! @brief Reads information on how to treat each column
static const STreeBrowserColumnInfo* GetColumnInfo(ETreeBrowserColumn eColumn)
{
    static const std::unordered_map<ETreeBrowserColumn, STreeBrowserColumnInfo> mapInfo {
        {ETreeBrowserColumn::IconAndName,  STreeBrowserColumnInfo{"wxDataViewIconText", &CTreeBrowserModel::GetNameAndIcon, &CTreeBrowserModel::SetName} },
        {ETreeBrowserColumn::Description,  STreeBrowserColumnInfo{"string",             &CTreeBrowserModel::GetDescription, nullptr} },
    };

    const STreeBrowserColumnInfo* pInfo = nullptr;

    const auto itFound = mapInfo.find(eColumn);
    if (mapInfo.cend() != itFound)
        pInfo = &(itFound->second);

    return pInfo;
}

//! @brief Casts the DataViewItem into a ProjectItem
static inline IProjTreeItem* GetItem(const wxDataViewItem& item)
{
    return static_cast<IProjTreeItem*>(item.GetID());
}

//! Maps child (key) to parent (value) - this two-way relation does not exist in the project structure
static std::map<wxDataViewItem, wxDataViewItem> g_mapParents;

/// =======================================================
/// Model logic
/// =======================================================

CTreeBrowserModel::CTreeBrowserModel()
{

}

CTreeBrowserModel::~CTreeBrowserModel()
{

}

void CTreeBrowserModel::GetNameAndIcon(wxVariant& value, const IProjTreeItem* pItem) const
{
    #define REGICON(RESNAME) wxIcon(RESNAME, wxBITMAP_TYPE_ICO_RESOURCE, 24, 24)

    const wxString strName = pItem->GetName();
    const wxIcon& rIcon = STreeItemTypeInfo::GetIcon(pItem->GetType());
    wxDataViewIconText iconAndText(strName, rIcon);
    value = wxVariant(iconAndText);
}

void CTreeBrowserModel::GetDescription(wxVariant& value, const IProjTreeItem* pItem) const
{
    const wxString strDesc = pItem->GetDescription();
    value = strDesc;
}

bool CTreeBrowserModel::SetName(const wxVariant& value, IProjTreeItem* pItem)
{
    wxDataViewIconText iconAndText;
    iconAndText << value;

    pItem->SetName( iconAndText.GetText().ToStdString() );

    wxCommandEvent evtNotifyRename(EVT_TREE_ITEM_RENAME);
    evtNotifyRename.SetClientData(pItem);
    ProcessEvent(evtNotifyRename);

    return true;
}

/// =======================================================
/// Model interface overrides
/// =======================================================

unsigned int CTreeBrowserModel::GetColumnCount() const
{
    return (unsigned int)ETreeBrowserColumn::Count;
}

bool CTreeBrowserModel::HasValue(const wxDataViewItem& item, unsigned col) const
{
    return col < GetColumnCount();
}

wxString CTreeBrowserModel::GetColumnType(unsigned int col) const
{
    const STreeBrowserColumnInfo* pInfo = GetColumnInfo(ETreeBrowserColumn(col));
    assert(nullptr != pInfo);

    return pInfo->m_strType;
}

void CTreeBrowserModel::GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const
{
    const STreeBrowserColumnInfo* pInfo = GetColumnInfo(ETreeBrowserColumn(col));
    assert(nullptr != pInfo);

    const IProjTreeItem* pItem = GetItem(item);
    assert(nullptr != pItem);

    STreeBrowserColumnInfo::fnGetValue_t fnGetValue = pInfo->m_fnGetValue;
    assert(nullptr != fnGetValue);

    (this->*fnGetValue)(variant, pItem);
}

bool CTreeBrowserModel::SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col)
{
    const STreeBrowserColumnInfo* pInfo = GetColumnInfo(ETreeBrowserColumn(col));
    assert(nullptr != pInfo);

    IProjTreeItem* pItem = GetItem(item);
    assert(nullptr != pItem);

    bool bAccept = false;
    STreeBrowserColumnInfo::fnSetValue_t fnSetValue = pInfo->m_fnSetValue;
    if (nullptr != fnSetValue)
        bAccept = (this->*fnSetValue)(variant, pItem);

    return bAccept;
}

bool CTreeBrowserModel::IsEnabled(const wxDataViewItem& item, unsigned int col) const
{
    return true;
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

        // Regular items may or not have a vector of subitems
        bIsContainer = pItem->GetItems().has_value();
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

        ITreeItemCollection::OptCVecPtrItem optSubitems = pParent->GetItems();
        if (optSubitems.has_value())
        {
            const ITreeItemCollection::VecPtrItem& vecSubitems = optSubitems.value();

            for (const ITreeItemCollection::PtrItem& pChild : vecSubitems)
            {
                wxDataViewItem child(pChild.get());
                array.push_back(child);
                uiCount++;

                g_mapParents[child] = parent;
            }
        }
    }

    return uiCount;
}