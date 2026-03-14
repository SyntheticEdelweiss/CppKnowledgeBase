cmake_minimum_required(VERSION 3.16)
project(applicationC LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    main.cpp
)

find_package(QT NAMES Qt5 Qt6 REQUIRED) # find Qt*Config.cmake and set QT_VERSION_MAJOR, etc.
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Core
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    libraryA
    libraryB
    libraryC
    Qt${QT_VERSION_MAJOR}::Core
)

get_filename_component(ABSOLUTE_TARGET "${CMAKE_CURRENT_SOURCE_DIR}/../libraryC" ABSOLUTE)
file(RELATIVE_PATH REL_PATH ${CMAKE_SOURCE_DIR} "${ABSOLUTE_TARGET}")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../libraryC ${CMAKE_BINARY_DIR}/${REL_PATH})

# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../libraryD ${CMAKE_BINARY_DIR}/libraryD)
# add_LibraryD()

include(../libraryA/LibraryA.cmake)
