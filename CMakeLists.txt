cmake_minimum_required(VERSION 3.16)
project(CppKnowledgeBase LANGUAGES CXX)

include(GlobalConfig.cmake)

add_executable(${PROJECT_NAME}
    main.cpp
    GeneralTests.cpp
    GeneralTests.hpp
    N-dimensionArray.h
    PerformanceTests.cpp
    PerformanceTests.h
    UsefulStuff.cpp
    UsefulStuff.h
    Utils.cpp
    Utils.h
    Qt/QThreadTest.cpp
    Qt/QThreadTest.h
    Qt/SignalSlotTest.cpp
    Qt/SignalSlotTest.h
    Qt/QtPerformanceTests.cpp
    Qt/QtPerformanceTests.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Qt
)
