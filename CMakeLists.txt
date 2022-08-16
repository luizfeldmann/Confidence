CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
PROJECT(Confidence LANGUAGES CXX VERSION 1.0.0.0)

# Configure project metadata
SET(PROJ_DESCRIPTION "Confidence is a tool to centralize and manage configuration files")
SET(COMPANY_NAME "Luiz Feldmann")
SET(COPYRIGHT_OWNER ${COMPANY_NAME})

include(res/mainIcon/exportIcon.cmake)
SET(RESOURCESFILE "resources.rc")
configure_file(res/resources.in.rc ${RESOURCESFILE})

# Dependency: JInterface
include(dependencies/JInterface/ConsumeMe.txt)

# Dependency: CLI11
include_directories(dependencies/CLI11/include)

# Project sources
SET(${CMAKE_PROJECT_NAME}_SOURCES
    "src/main.cpp"
    # Utility
    "include/util/CScopedConsoleStyle.h"
    "src/util/CScopedConsoleStyle.cpp"
    "include/util/Log.h"
    "src/util/Log.cpp"
    "include/util/CGuid.h"
    "src/util/CGuid.cpp"
    # Resources
    ${RESOURCESFILE}
    ${AppIcon_DST}
    # Virtual file system
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
)

set_target_properties(${CMAKE_PROJECT_NAME}
    PROPERTIES
    CXX_STANDARD 11
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib
)

