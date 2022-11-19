# The VCPKG port of Graphviz currently does not support find_package

SET(GV_IMPORT_PREFIX "${Z_VCPKG_ROOT_DIR}/packages/graphviz_${VCPKG_TARGET_TRIPLET}")

SET(GV_LIB_DBG "${GV_IMPORT_PREFIX}/debug/lib")
SET(GV_BIN_DBG "${GV_IMPORT_PREFIX}/debug/bin")
SET(GV_LIB_REL "${GV_IMPORT_PREFIX}/lib")
SET(GV_BIN_REL "${GV_IMPORT_PREFIX}/bin")

# Import CGraph
add_library(graphviz::cgraph SHARED IMPORTED)

set_property(TARGET graphviz::cgraph APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG RELEASE)

set_target_properties(graphviz::cgraph PROPERTIES
	IMPORTED_IMPLIB_DEBUG "${GV_LIB_DBG}/cgraph.lib"
	IMPORTED_LOCATION_DEBUG "${GV_BIN_DBG}/cgraph.dll"
	IMPORTED_IMPLIB_RELEASE "${GV_LIB_REL}/cgraph.lib"
	IMPORTED_LOCATION_RELEASE "${GV_BIN_REL}/cgraph.dll"
)

# Import GVC
add_library(graphviz::gvc SHARED IMPORTED)

set_property(TARGET graphviz::gvc APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG RELEASE)

set_target_properties(graphviz::gvc PROPERTIES
	IMPORTED_IMPLIB_DEBUG "${GV_LIB_DBG}/gvc.lib"
	IMPORTED_LOCATION_DEBUG "${GV_BIN_DBG}/gvc.dll"
	IMPORTED_IMPLIB_RELEASE "${GV_LIB_REL}/gvc.lib"
	IMPORTED_LOCATION_RELEASE "${GV_BIN_REL}/gvc.dll"
)

# Import plugins: Core
add_library(graphviz::plugin::core SHARED IMPORTED)

set_property(TARGET graphviz::plugin::core APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG RELEASE)

set_target_properties(graphviz::plugin::core PROPERTIES
	IMPORTED_IMPLIB_DEBUG "${GV_LIB_DBG}/gvplugin_core.lib"
	IMPORTED_LOCATION_DEBUG "${GV_BIN_DBG}/gvplugin_core.dll"
	IMPORTED_IMPLIB_RELEASE "${GV_LIB_REL}/gvplugin_core.lib"
	IMPORTED_LOCATION_RELEASE "${GV_BIN_REL}/gvplugin_core.dll"
)

# Import plugins: Dot
add_library(graphviz::plugin::dot SHARED IMPORTED)

set_property(TARGET graphviz::plugin::dot APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG RELEASE)

set_target_properties(graphviz::plugin::dot PROPERTIES
	IMPORTED_IMPLIB_DEBUG "${GV_LIB_DBG}/gvplugin_dot_layout.lib"
	IMPORTED_LOCATION_DEBUG "${GV_BIN_DBG}/gvplugin_dot_layout.dll"
	IMPORTED_IMPLIB_RELEASE "${GV_LIB_REL}/gvplugin_dot_layout.lib"
	IMPORTED_LOCATION_RELEASE "${GV_BIN_REL}/gvplugin_dot_layout.dll"
)

# Import plugins: GDI+
add_library(graphviz::plugin::gdip SHARED IMPORTED)

set_property(TARGET graphviz::plugin::gdip APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG RELEASE)

set_target_properties(graphviz::plugin::gdip PROPERTIES
	IMPORTED_IMPLIB_DEBUG "${GV_LIB_DBG}/gvplugin_gdiplus.lib"
	IMPORTED_LOCATION_DEBUG "${GV_BIN_DBG}/gvplugin_gdiplus.dll"
	IMPORTED_IMPLIB_RELEASE "${GV_LIB_REL}/gvplugin_gdiplus.lib"
	IMPORTED_LOCATION_RELEASE "${GV_BIN_REL}/gvplugin_gdiplus.dll"
)

# Interface for the whole Graphviz
add_library(graphviz INTERFACE IMPORTED)

set_property(TARGET graphviz PROPERTY INTERFACE_LINK_LIBRARIES 
graphviz::cgraph
graphviz::gvc
graphviz::plugin::core
graphviz::plugin::dot
graphviz::plugin::gdip
)

set_property(TARGET graphviz PROPERTY
  INTERFACE_INCLUDE_DIRECTORIES "${GV_IMPORT_PREFIX}/include"
)