# Convert the README.md into a webpage that can be displayed by wxWidgets

set(README_MD "${PROJECT_SOURCE_DIR}/README.md")
set(README_HTML "${CMAKE_BINARY_DIR}/README.html")

add_custom_command(
  OUTPUT ${README_HTML}
  COMMAND "pandoc" -s "${README_MD}" -o "${README_HTML}"
  DEPENDS ${README_MD}
  VERBATIM
)

# Embed the README into the application
cmrc_add_resources(${CMAKE_PROJECT_NAME}-resources
	WHENCE "${CMAKE_BINARY_DIR}"
	"${README_HTML}"
)