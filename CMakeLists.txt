cmake_minimum_required(VERSION 3.14)
project(ACppTemplate
  VERSION 0.0.1
  LANGUAGES CXX)

if(PROJECT_IS_TOP_LEVEL)
  message(NOTICE "Adding All Targets.")
  set(ROOT_PROJECT ${PROJECT_NAME})
  add_subdirectory(all)
  return()
endif()

if(PROJECT_IS_TOP_LEVEL OR(ROOT_PROJECT STREQUAL PROJECT_NAME))
  set(CMAKE_MESSAGE_LOG_LEVEL "DEBUG")
  message(DEBUG "Enable Log Level -- Debug")
else()
  set(CMAKE_MESSAGE_LOG_LEVEL "NOTICE")
endif()

set(CMAKE_CXX_STANDARD_REQUIRED 17)
message(NOTICE "Project: ${PROJECT_NAME}")

if(PROJECT_IS_TOP_LEVEL)
  message(NOTICE "The project is Top-Level.")
endif()

# ---- Include guards ----
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
    "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# ---- Use cpm to manage external dependencies ----
include(cmake/CPM.cmake)

# ---- Declare the dependencies ----
include(cmake/dependencies.cmake)

# ---- Add source files ----
set(PROJECT_MAIN_PREFIX "${PROJECT_SOURCE_DIR}")
set(PROJECT_MAIN_INCLUDE_DIR "${PROJECT_MAIN_PREFIX}/include")
set(PROJECT_MAIN_SOURCE_DIR "${PROJECT_MAIN_PREFIX}/src")

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.
file(GLOB_RECURSE headers CONFIGURE_DEPENDS
  "${PROJECT_MAIN_INCLUDE_DIR}/*.h"
  "${PROJECT_MAIN_INCLUDE_DIR}/*.hpp")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS
  "${PROJECT_MAIN_SOURCE_DIR}/*.cpp"
  "${PROJECT_MAIN_SOURCE_DIR}/*.cxx"
  "${PROJECT_MAIN_SOURCE_DIR}/*.cc")
message(DEBUG "Include Directory is ${PROJECT_MAIN_INCLUDE_DIR}")
message(DEBUG "Include Files Found: ${headers}")
message(DEBUG "Source Directory is ${PROJECT_MAIN_SOURCE_DIR}")
message(DEBUG "Source Files Found: ${sources}")
list(LENGTH sources sources_length)

# ---- Create library ----

# Note: for header-only libraries change all PUBLIC flags to INTERFACE and create an interface
# target: add_library(${PROJECT_NAME} INTERFACE)
if(${sources_length} EQUAL 0)
  message(NOTICE "The library \"${PROJECT_NAME}\" is header-only, because no source file found. The library is just an interface.")
  add_library(${PROJECT_NAME} INTERFACE)
  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
  target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
    $<BUILD_INTERFACE:${PROJECT_MAIN_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
  )
else()
  message(NOTICE "Add Library ${PROJECT_NAME}")
  add_library(${PROJECT_NAME} STATIC ${headers} ${sources})
  set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

  # being a cross-platform target, we enforce standards conformance on MSVC
  target_compile_options(${PROJECT_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")
  target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_MAIN_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
  )
endif()

# ---- Package Version ----
set(VERSION_HEADER_LOCATION ${PROJECT_NAME}/version.h)
message(NOTICE "Package Project. (install prefix=${CMAKE_INSTALL_PREFIX}).")

# ---- Package Exporter ----
CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.8.0")

packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_MAIN_INCLUDE_DIR}
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  DEPENDENCIES "" # "fmt 8.1.1;cxxopts 2.2.0"

  # --- Parameters following are optional: ---
  # INCLUDE_HEADER_PATTERN "*.h"
  NAMESPACE ${PROJECT_NAME}

  # EXPORT_HEADER "${PROJECT_NAME}/export.h"
  # VERSION_HEADER "${VERSION_HEADER_LOCATION}"

  # COMPATIBILITY SameMajorVersion
  # DISABLE_VERSION_SUFFIX YES
  ARCH_INDEPENDENT YES
)