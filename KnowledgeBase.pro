TARGET = KnowledgeBase
TEMPLATE = app
CONFIG += console warn_on
CONFIG += c++17
# CONFIG += build_all
win32: !contains(CONFIG, build_all): CONFIG -= debug_and_release

CONFIG(debug, debug|release) {
    BUILD_TYPE = debug
    DEFINES *= DEBUG_BUILD
}
CONFIG(release, debug|release) {
    BUILD_TYPE = release
    DEFINES *= RELEASE_BUILD
    QMAKE_CXXFLAGS += -O2
}

DESTDIR     = $$PWD/bin
UI_DIR      = $$PWD/ui

# if (PWD != build_dir), such as the case with shadow build
!equals(PWD, $${OUT_PWD}) {
    TARGET      = $${TARGET}.$${BUILD_TYPE}
} else {
    TARGET      = $${TARGET}.$${BUILD_TYPE}
    MOC_DIR     = $$PWD/build/$$BUILD_TYPE
    OBJECTS_DIR = $$PWD/build/$$BUILD_TYPE
    RCC_DIR     = $$PWD/build/$$BUILD_TYPE
}


QT *= \
    core    \
    gui     \
    widgets

HEADERS += \
    Utils.h \
    UsefulStuff.h \
    N-dimensionArray.h \
    PerformanceTests.h \
    Qt/QThreadTest.h \
    Qt/SignalSlotTest.h \
    Qt/QtPerformanceTests.h

SOURCES += \
    main.cpp \
    Utils.cpp \
    UsefulStuff.cpp \
    PerformanceTests.cpp \
    Qt/QThreadTest.cpp \
    Qt/SignalSlotTest.cpp \
    Qt/QtPerformanceTests.cpp

OTHER_FILES += \
    "using namespace in headers - ns conflict example.txt"

config_sanitizer_address {
    TARGET = $${TARGET}_sanitizer_address
    CONFIG += sanitizer sanitize_address
}
