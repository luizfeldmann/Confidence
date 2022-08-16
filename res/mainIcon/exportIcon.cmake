
# Find ImageMagick tool
find_program(IMGMAGICK magick)
if (NOT IMGMAGICK)
    message(FATAL_ERROR "ImageMagick not found! Cannot export application icon")
endif()

# Prepare icon source and destination
SET(AppIcon_SRC "${CMAKE_CURRENT_LIST_DIR}/icon.svg")
SET(AppIcon_DST "${CMAKE_CURRENT_LIST_DIR}/icon.ico")

# Rules to build the icon
add_custom_command(
  OUTPUT ${AppIcon_DST}
  COMMAND "${IMGMAGICK}" -density 256x256 -background transparent "${AppIcon_SRC}" -define icon:auto-resize -colors 256 "${AppIcon_DST}"
  DEPENDS ${AppIcon_SRC}
  VERBATIM
)