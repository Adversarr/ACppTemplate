# this script adds all subprojects to a single build to allow IDEs understand the full project
# structure.

cmake_minimum_required(VERSION 3.14)

project(BuildAll LANGUAGES CXX)

set(ROOT_PROJECT ACppTemplate)
include(../cmake/tools.cmake)

# needed to generate test target
enable_testing()

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/../standalone)
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../standalone ${CMAKE_BINARY_DIR}/standalone)
endif()

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/../test)
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../test ${CMAKE_BINARY_DIR}/test)
endif()

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/../documentation)
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../documentation ${CMAKE_BINARY_DIR}/documentation)
endif()
