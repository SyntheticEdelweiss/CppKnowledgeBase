message(<---------- applicationB ---------->)

TEMPLATE = app
CONFIG += console c++17

include($$clean_path($$PWD/../global.pri))
include($$clean_path($$PWD/../libraryA/libraryA.pri))
# include($$clean_path($$PWD/../libraryB/libraryB.pri))
# include($$clean_path($$PWD/../libraryC/libraryC.pri))

!equals(GLOBAL_PRI_ENABLED, "true"): error("GLOBAL_PRI_ENABLED != true // Hint: global.pri is either missing or incorrectly set up")

LIBS *= $$LIBS_paths
LIBS *= $$reverse(LIBS_libs)

SOURCES += \
        main.cpp

message($$GLOBAL_TEST_VAR)



