message(<---------- libraryB ---------->)

TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++17

include($$clean_path($$PWD/../global.pri))
include($$clean_path($$PWD/libraryB.pri))

SOURCES += \
    LibraryB.cpp

HEADERS += \
    LibraryB.h
