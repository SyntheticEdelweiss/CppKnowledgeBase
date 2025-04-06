# message(<---------- global.pri ---------->)
# only makes sense to include it once -> skip unnecessary processing if it's already included
this_pri_file = $$_FILE_
contains(included_pri_files, $$this_pri_file): return()
included_pri_files += $$this_pri_file

GLOBAL_PRI_ENABLED = true
CONFIG(debug, debug|release) {
    DESTDIR_app = $$PWD/bin
    DESTDIR_lib = $$PWD/lib
    BUILD_TYPE = debug
    DEFINES *= DEBUG_BUILD
}
CONFIG(release, debug|release) {
    DESTDIR_app = $$PWD/bin/release
    DESTDIR_lib = $$PWD/lib/release
    BUILD_TYPE = release
    DEFINES *= RELEASE_BUILD
    QMAKE_CXXFLAGS += -O2
}
TARGET_postfix = .$${BUILD_TYPE}

equals(TEMPLATE, "app") {
    DESTDIR = $$DESTDIR_app
} else: equals(TEMPLATE, "lib"): CONFIG(static, static|staticlib|shared|dll)|CONFIG(staticlib, static|staticlib|shared|dll) {
    DESTDIR = $$DESTDIR_lib
	LIB_TYPE = static
} else: equals(TEMPLATE, "lib"): CONFIG(shared, static|staticlib|shared|dll)|CONFIG(dll, static|staticlib|shared|dll) {
    DESTDIR = $$DESTDIR_app
	LIB_TYPE = shared
} else {
    message("<-------------------------------------------------->")
    message(TEMPLATE=$$TEMPLATE)
    message(CONFIG=$$CONFIG)
    error("TEMPLATE or CONFIG has incorrect value")
}

win32: !contains(CONFIG, build_all): CONFIG -= debug_and_release
