
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
    "ARROWUP"
    "ARROWDOWN"
    "ADD"
    "TRASH"
    "TREEITEM_ERROR"
    "TREEITEM_PROJECT"
    "TREEITEM_GROUP"
    "TREEITEM_CODE"
    "TREEITEM_CODEFILE"
    "TREEITEM_EXEC"
)

# Add build rule for each icon
set(ICON_RESOURCE_PREFIX "RES_ID_ICON_")
unset(ICON_RESOURCES_LIST)
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
