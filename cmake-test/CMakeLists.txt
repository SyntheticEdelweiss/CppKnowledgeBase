cmake_minimum_required(VERSION 3.16)
project(cmake-test LANGUAGES CXX)

# include(CMakePrintHelpers)
include(GlobalConfig.cmake)

option(AppA-Config "applicationA + libs as subdir dependencies" OFF)
option(AppB-Config "applicationB + libs as sources in .cmake files" OFF)
option(AppC-Config "applicationC + libs as external projects" OFF)

if(AppA-Config)
    add_subdirectory(src/libraryC)
    add_subdirectory(src/libraryB)
    add_subdirectory(src/libraryA)
    add_subdirectory(src/applicationA)
endif()

if(AppB-Config)
    add_subdirectory(src/applicationB)
endif()

if(AppC-Config)
    add_subdirectory(src/applicationC)
endif()

_get_all_cmake_targets(all_targets ${CMAKE_CURRENT_LIST_DIR})
message("All targets: ${all_targets}")
