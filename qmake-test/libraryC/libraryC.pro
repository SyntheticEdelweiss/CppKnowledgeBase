message(<---------- libraryC ---------->)

TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++17

include($$clean_path($$PWD/../global.pri))
include($$clean_path($$PWD/libraryC.pri))

SOURCES += \
    LibraryC.cpp

HEADERS += \
    LibraryC.h
