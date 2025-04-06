# message(<---------- libraryB ---------->)
# only makes sense to include it once -> skip unnecessary processing if it's already included
this_pri_file = $$_FILE_
contains(included_pri_files, $$this_pri_file): return()
included_pri_files += $$this_pri_file

include($$clean_path($$PWD/../global.pri))

QT *= core

INCLUDEPATH *= $$clean_path($$PWD/..)

# global.pri should set destdir. If global.pri is not enabled, set destdir here
equals(GLOBAL_PRI_ENABLED, "true") {
    local_path = $$DESTDIR_lib
} else {
    local_path = $$clean_path($$PWD/../lib)
}
local_target = $$basename(PWD)

# add this lib .a as a dependency to other projects, but not itself
!equals(PWD, $$_PRO_FILE_PWD_) {
    LIBS_paths *= -L$${local_path}
    LIBS_libs  *= -l$${local_target}
	equals(LIB_TYPE, "static") {
		PRE_TARGETDEPS *= $${local_path}/lib$${local_target}.a
	} else {
		win32 : PRE_TARGETDEPS *= $${local_path}/lib$${local_target}.dll
		unix : PRE_TARGETDEPS *= $${local_path}/lib$${local_target}.so
	}
}

equals(PWD, $$_PRO_FILE_PWD_) : return()



TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++17

SOURCES += \
    LibraryB.cpp

HEADERS += \
    LibraryB.h
