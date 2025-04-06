message(<---------- libraryA ---------->)

TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++17

include($$clean_path($$PWD/../global.pri))
include($$clean_path($$PWD/libraryA.pri))

LIBS *= $$LIBS_paths
LIBS *= $$reverse(LIBS_libs)

SOURCES += \
    LibraryA.cpp

HEADERS += \
    LibraryA.h

# message($$GLOBAL_TEST_VAR)
