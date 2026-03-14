target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/libraryB.cpp
    ${CMAKE_CURRENT_LIST_DIR}/libraryB.h
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/..>
    $<INSTALL_INTERFACE:include>
)

include(${CMAKE_CURRENT_LIST_DIR}/../libraryC/LibraryC.cmake)
