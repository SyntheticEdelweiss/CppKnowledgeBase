message(<---------- qmake-test ---------->)
# message(PWD=$$PWD)

TEMPLATE = subdirs

SUBDIRS += \
    applicationA \
    libraryA \
    libraryB \
    libraryC
	
applicationA.subdir = $$PWD/applicationA
libraryA.subdir = $$PWD/libraryA
libraryB.subdir = $$PWD/libraryB
libraryC.subdir = $$PWD/libraryC

libraryA.depends = libraryB libraryC
applicationA.depends = libraryA libraryB libraryC


