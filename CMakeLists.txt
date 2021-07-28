cmake_minimum_required(VERSION 3.0.0)

SET(PROJECT_DIR_PATH ${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/configure_cmake.cmake)

option(ENABLE_LIB_BLE_C_API "Build C Wrapper" ON)
option(ENABLE_APP_BLE_TEST "Build Test App" ON)
option(OUTPUT_DIR "Output directory for generated artifacts" ${CMAKE_CURRENT_SOURCE_DIR}/bin/${OUTPUT_SUFFIX})

project(NATIVEBLE)

if (OUTPUT_DIR STREQUAL "OFF")
    # Set OUTPUT_DIR to its default value if it hasn't been set externally.
    set(OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin/${OUTPUT_SUFFIX})
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/configure_targets.cmake)
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/configure_dependencies.cmake)
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/configure_outputs.cmake)
