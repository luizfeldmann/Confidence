#include "ui/STreeItemTypeInfo.h"
#include "core/CGroup.h"
#include "core/CVariable.h"
#include <map>

const std::vector<ETreeItemType>& STreeItemTypeInfo::GetTypesList()
{
    static const std::vector<ETreeItemType> theList {
        EProject,
        EGroup,
        EVariable,
        ETextFile
    };

    return theList;
}

const wxString& STreeItemTypeInfo::STreeItemTypeInfo::GetName(ETreeItemType eType)
{
    const STreeItemTypeInfo* pInfo = GetInfo(eType);
    
    return (nullptr != pInfo) ? pInfo->m_strTypeName : GetName(EInvalid);
}

const STreeItemTypeInfo* STreeItemTypeInfo::GetInfo(ETreeItemType eType)
{
    #define REGICON(RESNAME) wxIcon(RESNAME, wxBITMAP_TYPE_ICO_RESOURCE, 24, 24)

    static const std::map<ETreeItemType, STreeItemTypeInfo> mapIcons
    {
        { EInvalid, 
            STreeItemTypeInfo {
                "Invalid item",
                REGICON(L"RES_ID_ICON_TREEITEM_ERROR"),
                nullptr,
            }
        },
        { EProject,
            STreeItemTypeInfo {
                "Project", 
                REGICON(L"RES_ID_ICON_TREEITEM_PROJECT"),
                nullptr,
            }
        },
        { EGroup, 
            STreeItemTypeInfo {
                "Group", 
                REGICON(L"RES_ID_ICON_TREEITEM_GROUP"),
                []() { return new CGroup(); }
            }
        },
        { EVariable, 
            STreeItemTypeInfo {
                "Variable",
                REGICON(L"RES_ID_ICON_TREEITEM_CODE"),
                []() { return new CVariable(); }
            }
        },
        { ETextFile, 
            STreeItemTypeInfo {
                "Output text file", 
                REGICON(L"RES_ID_ICON_TREEITEM_CODEFILE"),
                nullptr
            }
        },
    };

    const STreeItemTypeInfo* pInfo = nullptr;

    const auto it = mapIcons.find(eType);
    if (mapIcons.cend() != it)
        pInfo = &(it->second);

    return pInfo;
}