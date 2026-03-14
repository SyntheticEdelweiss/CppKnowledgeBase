cmake_minimum_required(VERSION 3.16)
project(applicationA LANGUAGES CXX)

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
