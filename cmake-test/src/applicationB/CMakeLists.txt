cmake_minimum_required(VERSION 3.16)
project(applicationB LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    main.cpp
)

include(${SRC_DIR}/libraryA/LibraryA.cmake)
# include(${SRC_DIR}/libraryB/LibraryB.cmake)
# include(${SRC_DIR}/libraryC/LibraryC.cmake)

find_package(QT NAMES Qt5 Qt6 REQUIRED) # find Qt*Config.cmake and set QT_VERSION_MAJOR, etc.
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Core
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
)
