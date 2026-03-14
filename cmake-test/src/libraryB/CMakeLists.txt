cmake_minimum_required(VERSION 3.16)
project(libraryB LANGUAGES CXX)

add_library(${PROJECT_NAME} 
    libraryB.cpp
    libraryB.h
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PUBLIC libraryC)

#export(TARGETS ${PROJECT_NAME} FILE ${PROJECT_NAME}Config.cmake)

set_target_properties(${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    VERSION   1.0.0
    SOVERSION 1
)
