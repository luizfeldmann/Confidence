
# Find ImageMagick tool
find_program(IMGMAGICK magick)
if (NOT IMGMAGICK)
    message(FATAL_ERROR "ImageMagick not found! Cannot export application icon")
endif()

# List of icons to convert
SET(ICONS_SOURCE
    "APPLICATION"
    "SETUP"
    "NEW"
    "OPEN"
    "SAVE1"
    "SAVE2"
    "RUN"
    "STOP"
    "DOCSEXPORT"
    "ARROWUP"
    "ARROWDOWN"
    "SCISSORS"
    "CLIPBOARD"
    "ADD"
    "TRASH"
    "INFO"
    "EXPORT"
    "PENCIL"
    "EXPR_ASSIGN"
    "EXPR_INHERIT"
    "EXPR_UNDEFINED"
    "EXPR_ERROR"
    "TREEITEM_ERROR"
    "TREEITEM_PROJECT"
    "TREEITEM_GROUP"
    "TREEITEM_CODE"
    "TREEITEM_CODEFILE"
    "TREEITEM_EXEC"
    "TREEITEM_INSTANCE"
    "TREEITEM_INSTANCES"
    "TREEITEM_CONFIG"
    "TREEITEM_CONFIGS"
    "TREEITEM_VARENV"
    "TREEITEM_CONSTANT"
    "TREEITEM_FILE_OPERATIONS"
    "TREEITEM_CONDITIONAL"
    "GATE_OR"
    "GATE_AND"
    "GATE_NOT"
)

# Special icons which must come first in list
set(ICON_SPECIAL "\
0 ICON \"${CMAKE_CURRENT_LIST_DIR}/APPLICATION.ico\"\n\
1 ICON \"${CMAKE_CURRENT_LIST_DIR}/SETUP.ico\"\n\
2 ICON \"${CMAKE_CURRENT_LIST_DIR}/TREEITEM_PROJECT.ico\" \
")

# Add build rule for each icon
set(ICON_RESOURCE_PREFIX "RES_ID_ICON_")
set(ICON_RESOURCES_LIST ${ICON_SPECIAL})
unset(ICONS_LIST_EXPORTED)
foreach (fileBaseName IN LISTS ICONS_SOURCE)
    SET(IconSrcFile "${CMAKE_CURRENT_LIST_DIR}/${fileBaseName}.svg")
    SET(IconDstFile "${CMAKE_CURRENT_LIST_DIR}/${fileBaseName}.ico")
    
    add_custom_command(
      OUTPUT ${IconDstFile}
      COMMAND "${IMGMAGICK}" -density 256x256 -background transparent "${IconSrcFile}" -define icon:auto-resize -colors 256 "${IconDstFile}"
      DEPENDS ${IconSrcFile}
      VERBATIM
    )
    
    list(APPEND ICONS_LIST_EXPORTED "${IconDstFile}")
    set(ICON_RESOURCES_LIST "${ICON_RESOURCES_LIST}\n${ICON_RESOURCE_PREFIX}${fileBaseName} ICON \"${IconDstFile}\"")
endforeach()
