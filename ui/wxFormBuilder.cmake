# Find the generator tool
find_program(WXFORM wxFormBuilder.exe
	"C:\\Program Files\\wxFormBuilder"
	"C:\\Program Files (x86)\\wxFormBuilder"
)

# List of all UI projects to generate
SET(WXFB_PROJECTS
    "IMainWindow"
    "INameDescEditor"
    "IProjectEditor"
    "IGroupEditor"
    "IVariableEditor"
    "ITextFileEditor"
    "IRunProcessEditor"
    "IOperationEditor"
    "IConstantEditor"
    "IConditionalEditor"
    "IConfigurationEditor"
)

unset(WXFB_GENERATED)

# Custom build for generated C++
foreach (wxfbProj IN LISTS WXFB_PROJECTS)
    SET(PrjFile "${CMAKE_CURRENT_LIST_DIR}/${wxfbProj}.fbp")
    SET(SrcFile "${CMAKE_CURRENT_LIST_DIR}/wxExport/${wxfbProj}.cpp")
    SET(IncFile "${CMAKE_CURRENT_LIST_DIR}/wxExport/${wxfbProj}.h")
    
    add_custom_command(
      OUTPUT ${SrcFile}
      OUTPUT ${IncFile}
      COMMAND "${WXFORM}" -g "${PrjFile}"
      DEPENDS ${PrjFile}
      VERBATIM
    )
    
    list(APPEND WXFB_GENERATED ${SrcFile} ${IncFile})
endforeach()