#include "ui/STreeItemTypeInfo.h"
#include "ui/CProjectEditor.h"
#include "ui/CGroupEditor.h"
#include "ui/CVariableEditor.h"
#include "ui/CGeneratedTextFileEditor.h"
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
    const STreeItemTypeInfo& rInfo = GetInfo(eType);
    return rInfo.m_strTypeName;
}

const wxIcon& STreeItemTypeInfo::GetIcon(ETreeItemType eType)
{
    const STreeItemTypeInfo& rInfo = GetInfo(eType);
    return rInfo.m_icon;
}

const STreeItemTypeInfo& STreeItemTypeInfo::GetInfo(ETreeItemType eType)
{
    #define REGICON(RESNAME) wxIcon(RESNAME, wxBITMAP_TYPE_ICO_RESOURCE, 24, 24)

    static const std::map<ETreeItemType, STreeItemTypeInfo> mapIcons
    {
        { EInvalid, 
            STreeItemTypeInfo {
                "Invalid item",
                REGICON(L"RES_ID_ICON_TREEITEM_ERROR"),
                nullptr,    // Cannot instantiate a new "invalid" item
                nullptr,    // Invalid items do not have editors
            }
        },
        { EProject,
            STreeItemTypeInfo {
                "Project", 
                REGICON(L"RES_ID_ICON_TREEITEM_PROJECT"),
                nullptr,    // Cannot instantiate a new project inside the existing project
                &CProjectEditor::Create
            }
        },
        { EGroup, 
            STreeItemTypeInfo {
                "Group", 
                REGICON(L"RES_ID_ICON_TREEITEM_GROUP"),
                &CGroup::Create,
                &CGroupEditor::Create
            }
        },
        { EVariable, 
            STreeItemTypeInfo {
                "Variable",
                REGICON(L"RES_ID_ICON_TREEITEM_CODE"),
                &CVariable::Create,
                &CVariableEditor::Create
            }
        },
        { ETextFile, 
            STreeItemTypeInfo {
                "Output text file", 
                REGICON(L"RES_ID_ICON_TREEITEM_CODEFILE"),
                &CGeneratedTextFile::Create,
                & CGeneratedTextFileEditor::Create
            }
        },
    };

    const auto it = mapIcons.find(eType);
    return (mapIcons.cend() != it) ? it->second : mapIcons.at(EInvalid);
}