#include "ui/STreeItemTypeInfo.h"
#include "ui/Editors/CProjectEditor.h"
#include "ui/Editors/CGroupEditor.h"
#include "ui/Editors/CVariableEditor.h"
#include "ui/Editors/CGeneratedTextFileEditor.h"
#include "ui/Editors/CProcessEditor.h"
#include "ui/Editors/COperationEditor.h"
#include "ui/Editors/CConstantEditor.h"
#include "ui/Editors/CConditionalEditor.h"
#include "core/items/CInstance.h"
#include "core/items/CConfiguration.h"
#include "core/items/CEnvironmentImport.h"
#include "core/items/file_operation/CFileOperations.h"
#include "core/items/CConstant.h"
#include "core/items/condition/CConditional.h"
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
        EImportEnv,
        EOperations,
        EConstant,
        EConditional
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
                "Invalid Item",
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
                "Output Text File", 
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
                "Instances Collection",
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
                "Configurations",
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
        { EImportEnv,
            STreeItemTypeInfo {
                "Import Environment",
                REGICON(L"RES_ID_ICON_TREEITEM_VARENV"),
                &CEnvironmentImport::Create,
                &CNameDescEditor::Create
            }
        },
        { EOperations,
            STreeItemTypeInfo {
                "File Operations",
                REGICON(L"RES_ID_ICON_TREEITEM_FILE_OPERATIONS"),
                &CFileOperations::Create,
                &COperationEditor::Create
            }
        },
        { EConstant,
            STreeItemTypeInfo {
                "Constant",
                REGICON(L"RES_ID_ICON_TREEITEM_CONSTANT"),
                &CConstant::Create,
                &CConstantEditor::Create
            }
        },
        { EConditional,
            STreeItemTypeInfo {
                "Conditional",
                REGICON(L"RES_ID_ICON_TREEITEM_CONDITIONAL"),
                &CConditional::Create,
                &CConditionalEditor::Create
            },
        }
    };

    const auto it = mapIcons.find(eType);
    return (mapIcons.cend() != it) ? it->second : mapIcons.at(EInvalid);
}