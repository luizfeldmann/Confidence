CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
PROJECT(Confidence LANGUAGES CXX VERSION 1.0.0.0 HOMEPAGE_URL https://github.com/luizfeldmann/Confidence)

# Configure project metadata
SET(PROJ_DESCRIPTION "Confidence is a tool to centralize and manage configuration files")
SET(COMPANY_NAME "Luiz Feldmann")
SET(COPYRIGHT_OWNER ${COMPANY_NAME})

# Dependency: cmrc
include(dependencies/cmrc/CMakeRC.cmake)
cmrc_add_resource_library(${CMAKE_PROJECT_NAME}-resources ALIAS ${CMAKE_PROJECT_NAME}::resources NAMESPACE resources)

# Embed the Readme into the application
include(res/README.cmake)

# Prebuild resources
include(res/icons/exportIcons.cmake)
SET(RESOURCESFILE "resources.rc")
configure_file(res/resources.in.rc ${RESOURCESFILE})

# Prebuild UI
include(ui/wxFormBuilder.cmake)

# Configure version file
configure_file(src/util/version.in.cpp "version.cpp")

# Dependency: JInterface
include(dependencies/JInterface/ConsumeMe.cmake)

# Dependency: CLI11
include_directories(dependencies/CLI11/include)

# Dependency: mINI
include_directories(dependencies/mINI/src)

# Dependency: pugiXML
find_package(pugixml CONFIG REQUIRED)

# Dependency: wxWidgets
find_package(wxWidgets COMPONENTS core base aui stc html)

# Dependency: CMark
find_package(cmark CONFIG REQUIRED)

# Dependency: Graphviz
include("dependencies/FindGraphviz.cmake")

# Project sources
SET(${CMAKE_PROJECT_NAME}_SOURCES
    "src/main.cpp"

    # Core
    "include/core/IExecutable.h"
    "include/core/CEvaluationContext.h"
    "src/core/CEvaluationContext.cpp"
    "include/core/CExecutionContextBase.h"
    "src/core/CExecutionContextBase.cpp"
    "include/core/CParentalMap.h"
    "src/core/CParentalMap.cpp"
    "include/core/CExecutionContext.h"
    "include/core/IContextStorage.h"
    "src/core/CExecutionContext.cpp" 
    "include/core/INamedItem.h"
    "include/core/CStoredNameItem.h"
    "src/core/CStoredNameItem.cpp"
    "include/core/IDescribedItem.h"
    "include/core/CStoredDescriptionItem.h"
    "src/core/CStoredDescriptionItem.cpp"
    "include/core/ITreeItemCollection.h"
    "include/core/CStoredItemCollection.h"
    "src/core/CStoredItemCollection.cpp"
    "include/core/CNoChildren.h"
    "src/core/CNoChildren.cpp"
    "include/core/ETreeItemType.h"
    "include/core/ISerializationNotify.h"
    "include/core/IProjTreeItem.h"
    "src/core/IProjTreeItem.cpp"
    "include/core/items/CProject.h"
    "src/core/items/CProject.cpp"
    "include/core/items/group/IGroupFilter.h"
    "include/core/items/group/CGroupFilterBase.h"
    "src/core/items/group/CGroupFilterBase.cpp"
    "include/core/items/group/CGroupFilterInstance.h"
    "src/core/items/group/CGroupFilterInstance.cpp"
    "include/core/items/group/CGroupFilterConfiguration.h"
    "src/core/items/group/CGroupFilterConfiguration.cpp"
    "include/core/items/group/CGroupFilterConfigurationAndInstance.h"
    "src/core/items/group/CGroupFilterConfigurationAndInstance.cpp"
    "include/core/items/group/CGroup.h"
    "src/core/items/group/CGroup.cpp"
    "include/core/items/variable/CVariableExpressionKey.h"
    "src/core/items/variable/CVariableExpressionKey.cpp"
    "include/core/items/variable/EBuiltInVariables.h"
    "src/core/items/variable/EBuiltInVariables.cpp"
    "include/core/CAssignable.h"
    "src/core/items/CAssignable.cpp"
    "include/core/items/variable/CEnvironmentVariable.h"
    "src/core/items/variable/CEnvironmentVariable.cpp"
    "include/core/items/variable/CVariable.h"
    "src/core/items/variable/CVariable.cpp"
    "include/core/items/CConstant.h"
    "src/core/items/CConstant.cpp"
    "include/core/items/file_generator/ITextProvider.h"
    "include/core/items/file_generator/CStoredTextProvider.h"
    "src/core/items/file_generator/CStoredTextProvider.cpp"
    "include/core/items/file_generator/CFileTextProvider.h"
    "src/core/items/file_generator/CFileTextProvider.cpp"
    "include/core/items/file_generator/CGeneratedTextFile.h"
    "src/core/items/file_generator/CGeneratedTextFile.cpp"
    "include/core/items/CEnvironmentImport.h"
    "src/core/items/CEnvironmentImport.cpp"
    "include/core/items/CInstance.h"
    "src/core/items/CInstance.cpp"
    "include/core/items/CInstanceGroup.h"
    "src/core/items/CInstanceGroup.cpp"
    "include/core/items/CConfiguration.h"
    "src/core/items/CConfiguration.cpp"
    "include/core/items/CConfigurationGroup.h"
    "src/core/items/CConfigurationGroup.cpp"
    "include/core/items/process/SProcessStartInfo.h"
    "include/core/items/process/CRunProcess.h"
    "include/core/items/process/IProcessPolicy.h"
    "src/core/items/process/IProcessPolicy.cpp"
    "include/core/IDependsOnVariables.h"
    "include/core/IExpression.h"
    "src/core/IExpression.cpp"
    "include/core/CStoredExpression.h"
    "src/core/CStoredExpression.cpp"
    "include/core/items/process/EProcessPolicy.h"
    "include/core/items/process/CProcessArgument.h"
    "src/core/items/process/CProcessArgument.cpp"
    "include/core/items/process/CProcessFireForget.h"
    "src/core/items/process/CProcessFireForget.cpp"
    "include/core/items/process/CProcessWaitCompletion.h"
    "src/core/items/process/CProcessWaitCompletion.cpp"
    "src/core/items/process/CRunProcess.cpp"
    "include/core/items/file_operation/CFileOperations.h"
    "src/core/items/file_operation/CFileOperations.cpp"
    "include/core/items/file_operation/IFileOperationHandler.h"
    "include/core/items/file_operation/EFileOperationHandlerType.h"
    "include/core/items/file_operation/IFileDataLocator.h"
    "include/core/items/file_operation/IFileOperationContext.h"
    "include/core/items/file_operation/CFileOperation.h"
    "src/core/items/file_operation/CFileOperation.cpp"
    "include/core/items/file_operation/CRegexHandler.h"
    "src/core/items/file_operation/CRegexHandler.cpp"
    "src/core/items/file_operation/CIniFileHandler.cpp"
    "include/core/items/file_operation/CXmlFileHandler.h"
    "src/core/items/file_operation/CXmlFileHandler.cpp"
    "include/core/items/condition/ICondition.h"
    "include/core/items/condition/CLogicalComparator.h"
    "src/core/items/condition/CLogicalComparator.cpp"
    "include/core/items/condition/CLogicalOperator.h"
    "src/core/items/condition/CLogicalOperator.cpp"
    "include/core/items/condition/CConditional.h"
    "src/core/items/condition/CConditional.cpp"
    "include/core/INotifyItemOperation.h"
    "src/core/INotifyItemOperation.cpp"

    # Documentation exporting
    "include/docs/IDocumentable.h"
    "include/docs/IOutputFileName.h"
    "include/docs/CStoredOutputFileName.h"
    "src/docs/CStoredOutputFileName.cpp"
    "include/docs/EDocExporter.h"
    "include/docs/IDocExporter.h"
    "include/docs/CMarkNodeWrapper.h"
    "src/docs/CMarkNodeWrapper.cpp"
    "include/docs/CMarkCustomBlock.h"
    "src/docs/CMarkCustomBlock.cpp"
    "include/docs/CCommonMarkExporter.h"
    "src/docs/CCommonMarkExporter.cpp"
    "include/docs/CMarkdownExporter.h"
    "src/docs/CMarkdownExporter.cpp"
    "include/docs/CHtmlExporter.h"
    "src/docs/CHtmlExporter.cpp"
    "include/docs/IDocumentationContext.h"
    "include/docs/CDiagram.h"
    "src/docs/CDiagram.cpp"

    # wxWidgets Autogenerated
    ${WXFB_GENERATED}

    # GUI
    "include/ui/CMainApp.h"
    "src/ui/CMainApp.cpp"
    "include/ui/CMainWindow.h"
    "src/ui/CMainWindow.cpp"
    "include/ui/CAcceleratorEntry.h"
    "src/ui/CAcceleratorEntry.cpp"
    "include/ui/STreeItemTypeInfo.h"
    "src/ui/STreeItemTypeInfo.cpp"
    "include/ui/SDocExporterTypeInfo.h" 
    "src/ui/SDocExporterTypeInfo.cpp" 
    "include/ui/CTreeItemEditorManager.h"
    "src/ui/CTreeItemEditorManager.cpp"
    "include/ui/Editors/ITreeItemEditor.h"
    "src/ui/Editors/ITreeItemEditor.cpp"
    "include/ui/Editors/CNameDescEditor.h"
    "src/ui/Editors/CNameDescEditor.cpp"
    "include/ui/Editors/CProjectEditor.h"
    "src/ui/Editors/CProjectEditor.cpp"
    "include/ui/Editors/CGroupEditor.h"
    "src/ui/Editors/CGroupEditor.cpp"
    "include/ui/Editors/CVariableEditor.h"
    "src/ui/Editors/CVariableEditor.cpp"
    "include/ui/Editors/CConstantEditor.h"
    "src/ui/Editors/CConstantEditor.cpp"
    "include/ui/Editors/CConfigurationEditor.h"
    "src/ui/Editors/CConfigurationEditor.cpp"
    "include/ui/CTextStyler.h" 
    "src/ui/CTextStyler.cpp"
    "include/ui/Editors/CGeneratedTextFileEditor.h"
    "src/ui/Editors/CGeneratedTextFileEditor.cpp"
    "include/ui/Editors/CProcessEditor.h"
    "src/ui/Editors/CProcessEditor.cpp"
    "include/ui/Editors/COperationEditor.h"
    "src/ui/Editors/COperationEditor.cpp"
    "include/ui/Editors/CConditionalEditor.h" 
    "src/ui/Editors/CConditionalEditor.cpp"
    "include/ui/Models/CBaseModel.h"
    "src/ui/Models/CBaseModel.cpp"
    "include/ui/Models/CBaseModelMapped.h"
    "src/ui/Models/CBaseModelMapped.cpp"
    "include/ui/Models/CBaseTreeItemModel.h"
    "src/ui/Models/CBaseTreeItemModel.cpp"
    "include/ui/Models/CTreeBrowserModel.h"
    "src/ui/Models/CTreeBrowserModel.cpp"
    "include/ui/Models/CProcessArgsModel.h"
    "src/ui/Models/CProcessArgsModel.cpp"
    "include/ui/Models/CInstanceColumn.h"
    "src/ui/Models/CInstanceColumn.cpp"
    "include/ui/Models/CVariableCell.h"
    "src/ui/Models/CVariableCell.cpp"
    "include/ui/Models/CVariableTableModel.h"
    "src/ui/Models/CVariableTableModel.cpp"
    "include/ui/Models/CFileOperationsModel.h"
    "src/ui/Models/CFileOperationsModel.cpp"
    "src/ui/Models/CConditionalTreeModel.cpp"
    "include/ui/Models/CConditionalTreeModel.h"
    "include/ui/Models/CConfigurationSummaryModel.h"
    "src/ui/Models/CConfigurationSummaryModel.cpp"
    "include/ui/Models/CDocExporterTableModel.h"
    "src/ui/Models/CDocExporterTableModel.cpp"

    # Utility
    "include/util/version.h" "version.cpp"
    "include/util/CScopedConsoleStyle.h"
    "src/util/CScopedConsoleStyle.cpp"
    "include/util/Log.h"
    "src/util/Log.cpp"
    "include/util/CGuid.h"
    "src/util/CGuid.cpp"
    
    "include/util/IIdentifiable.h"
    "src/util/IIdentifiable.cpp"
    "include/util/CTempFile.h"
    "src/util/CTempFile.cpp"
    "include/util/StartProcess.h"
    "src/util/StartProcess.cpp"

    # Resources
    ${README_HTML}
    ${RESOURCESFILE}
    ${ICONS_LIST_EXPORTED}

    # Virtual file system
    "include/vfs/IFileGenerator.h"
    "include/vfs/CPersistentFileGenerator.h"
    "src/vfs/CPersistentFileGenerator.cpp"
    "include/vfs/CTempSymlinkGenerator.h"
    "src/vfs/CTempSymlinkGenerator.cpp"
    "include/vfs/IProjectedFileSystemProvider.h"
    "src/vfs/IProjectedFileSystemProvider.cpp"  )

SET(${CMAKE_PROJECT_NAME}_INCLUDES
    "${CMAKE_CURRENT_LIST_DIR}/include"
    "${CMAKE_CURRENT_LIST_DIR}/ui"
)

# Create and configure target
add_executable(${CMAKE_PROJECT_NAME}
    # Own project
    "${${CMAKE_PROJECT_NAME}_SOURCES}"
    # Dependencies
    "${JINTERFACE_SOURCES_RELATIVE}"
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    # Own project
    "${${CMAKE_PROJECT_NAME}_INCLUDES}"
    # Dependencies
    "${JINTERFACE_INCLUDE_RELATIVE}"
    "${wxWidgets_INCLUDE_DIRS}"
)

target_link_libraries(${CMAKE_PROJECT_NAME} ${wxWidgets_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} cmark::cmark)
target_link_libraries(${CMAKE_PROJECT_NAME} pugixml)
target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}::resources)
target_link_libraries(${CMAKE_PROJECT_NAME} graphviz)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_CRTDBG_MAP_ALLOC)
endif()

set_target_properties(${CMAKE_PROJECT_NAME}
    PROPERTIES
    CXX_STANDARD 17
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib
)

# Create installer
install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION "bin")
x_vcpkg_install_local_dependencies(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION "bin")

SET(CPACK_GENERATOR "NSIS")
SET(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
SET(CPACK_PACKAGE_VENDOR "${COMPANY_NAME}")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJ_DESCRIPTION}")
SET(CPACK_PACKAGE_VERSION "${CMAKE_PROJECT_VERSION}")
SET(CPACK_PACKAGE_VERSION_MAJOR "${CMAKE_PROJECT_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${CMAKE_PROJECT_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${CMAKE_PROJECT_VERSION_PATCH}")
SET(CPACK_PACKAGE_VERSION_TWEAK "${CMAKE_PROJECT_VERSION_TWEAK}")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
SET(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/res/icons\\\\SETUP.ico")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
SET(CPACK_NSIS_MODIFY_PATH ON) # Add the tool to system PATH
SET(CPACK_PACKAGE_EXECUTABLES "${CMAKE_PROJECT_NAME};${CMAKE_PROJECT_NAME}")
SET(CPACK_CREATE_DESKTOP_LINKS "${CPACK_PACKAGE_EXECUTABLES}")

# Associate project file extensions on install
set(PROJ_FILE_EXT "cfx")
set(PROJ_FILE_CLASS "${CMAKE_PROJECT_NAME}.Project")
set(PROJ_FILE_DESCRIPTION "${CMAKE_PROJECT_NAME} Project File")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}//res//EXTRA_INSTALL_COMMANDS.in" "EXTRA_INSTALL_COMMANDS")
file(READ "${CMAKE_BINARY_DIR}//EXTRA_INSTALL_COMMANDS" CPACK_NSIS_EXTRA_INSTALL_COMMANDS)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}//res//EXTRA_UNINSTALL_COMMANDS.in" "EXTRA_UNINSTALL_COMMANDS")
file(READ "${CMAKE_BINARY_DIR}//EXTRA_UNINSTALL_COMMANDS" CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS)

# Call CPack
include(CPack)

# Generate installer only if Release
set(IFCPACK "$<CONFIG:Release>")

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND "$<${IFCPACK}:${CMAKE_CPACK_COMMAND}>"
            "$<${IFCPACK}:-C $<CONFIGURATION>>" 
            "$<${IFCPACK}:--config ${CPACK_OUTPUT_CONFIG_FILE}>"
    COMMENT "Running CPack. Please wait..."
    DEPENDS ${PROJECT_NAME}
)