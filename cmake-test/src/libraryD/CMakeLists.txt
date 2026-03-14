cmake_minimum_required(VERSION 3.16)
option(LibraryD-AsSources "ON = LibraryD is included as set of sources files; OFF = library" ON)
# return()

set(CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR})

macro(add_LibraryD)
target_sources(${PROJECT_NAME} PRIVATE
    ${CURRENT_LIST_DIR}/LibraryD.cpp
    ${CURRENT_LIST_DIR}/LibraryD.h
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
endmacro()

if(NOT LibraryD-AsSources)
    project(libraryD LANGUAGES CXX)
    add_library(${PROJECT_NAME})

    set_target_properties(${PROJECT_NAME} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        VERSION   1.0.0
        SOVERSION 1
    )

    add_LibraryD()
endif()
