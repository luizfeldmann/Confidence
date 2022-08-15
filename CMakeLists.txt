CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
PROJECT(Confidence CXX)

# Dependency: JInterface
include(dependencies/JInterface/ConsumeMe.txt)

# Project sources
SET(${CMAKE_PROJECT_NAME}_SOURCES
    "src/main.cpp"
)

SET(${CMAKE_PROJECT_NAME}_INCLUDES
    ${CMAKE_CURRENT_SOURCES_DIR}/include
    ${CMAKE_CURRENT_SOURCES_DIR}/ui
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
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

