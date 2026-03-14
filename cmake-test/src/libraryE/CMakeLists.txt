cmake_minimum_required(VERSION 3.16)
project(libraryC LANGUAGES CXX)

add_library(${PROJECT_NAME} 
    LibraryC.cpp
    LibraryC.h
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    VERSION   1.0.0
    SOVERSION 1
)

