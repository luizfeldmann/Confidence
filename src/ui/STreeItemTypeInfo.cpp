#include "ui/STreeItemTypeInfo.h"
#include "ui/Editors/CProjectEditor.h"
#include "ui/Editors/CGroupEditor.h"
#include "ui/Editors/CVariableEditor.h"
#include "ui/Editors/CGeneratedTextFileEditor.h"
#include "ui/Editors/CProcessEditor.h"
#include "core/CInstance.h"
#include "core/CConfiguration.h"
#include <map>

const std::vector<ETreeItemType>& STreeItemTypeInfo::GetTypesList()
{
    static const std::vector<ETreeItemType> theList {
        EProject,
        EGroup,
        EVariable,
        ETextFile,
        EInstance,
        EInstGroup,
        EConfig,
        EConfGroup,
        ERunProcess,
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
        { EInstance,
            STreeItemTypeInfo {
                "Instance",
                REGICON(L"RES_ID_ICON_TREEITEM_INSTANCE"),
                &CInstance::Create,
                &CNameDescEditor::Create // TODO: implement dedicated editor for instances
            }
        },
        { EInstGroup,
            STreeItemTypeInfo {
                "Instances collection",
                REGICON(L"RES_ID_ICON_TREEITEM_INSTANCES"),
                nullptr,    // Special item type, cannot be created 
                nullptr     // Cannot be edited
            }
        },
        { EConfig,
            STreeItemTypeInfo {
                "Configuration",
                REGICON(L"RES_ID_ICON_TREEITEM_CONFIG"),
                &CConfiguration::Create,
                &CNameDescEditor::Create // TODO: implement dedicated editor for configurations
            }
        },
        { EConfGroup,
            STreeItemTypeInfo {
                "Root configuration",
                REGICON(L"RES_ID_ICON_TREEITEM_CONFIGS"),
                nullptr,    // Special item type, cannot be created
                &CNameDescEditor::Create // TODO: implement dedicated editor for configurations
            }
        },
        { ERunProcess,
            STreeItemTypeInfo {
                "Run Process",
                REGICON(L"RES_ID_ICON_TREEITEM_EXEC"),
                &CRunProcess::Create,
                &CRunProcessEditor::Create
            }
        },
    };

    const auto it = mapIcons.find(eType);
    return (mapIcons.cend() != it) ? it->second : mapIcons.at(EInvalid);
}