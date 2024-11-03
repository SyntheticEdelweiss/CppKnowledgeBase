
TARGET = KnowledgeBase
TEMPLATE = app
CONFIG += console warn_on

DESTDIR      = $$PWD/bin/
MOC_DIR      = $$PWD/moc/
OBJECTS_DIR  = $$PWD/obj/

QT *= \
    core    \
    gui     \
    widgets

HEADERS += \
    Utils.h \
    SignalSlotTest.h \
    MultithreadingTest.h \
    UsefulStuff.h \
    N-dimensionArray.h \

SOURCES += \
    main.cpp \
    Utils.cpp \
    SignalSlotTest.cpp \
    MultithreadingTest.cpp \
    UsefulStuff.cpp

OTHER_FILES += \
    "using namespace in headers - ns conflict example.cpp"
