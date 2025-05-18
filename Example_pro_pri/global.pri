!assureUniquePri($$_FILE_) : return() # only makes sense to include it once -> skip unnecessary processing if it's already included

GLOBAL_PRI_ENABLED = true

# Для удобства определяем некоторые переменные и выставляем общие флаги.
CONFIG(debug, debug|release) {
    BUILD_TYPE = debug
    DEFINES *= DEBUG_BUILD
}
CONFIG(release, debug|release) {
    BUILD_TYPE = release
    DEFINES *= RELEASE_BUILD
    QMAKE_CXXFLAGS += -O2
}
DESTDIR_app = $$EXAMPLE_BIN
DESTDIR_lib = $$EXAMPLE_LIB

win32: !contains(CONFIG, build_all): CONFIG -= debug_and_release

# Определяем пути в зависимости от типа сборки.
equals(_PRO_FILE_PWD_, $$OUT_PWD) : { # in-source build
    rel_path = $$relative_path($$_PRO_FILE_PWD_, $$EXAMPLE_SRC)
    equals(TEMPLATE, "app") {
        DESTDIR = $$DESTDIR_app
    } else: equals(TEMPLATE, "lib"): CONFIG(static, static|staticlib|shared|dll)|CONFIG(staticlib, static|staticlib|shared|dll) {
        DESTDIR = $$getLibDestdir($$_PRO_FILE_PWD_)
        LIB_TYPE = static
    } else: equals(TEMPLATE, "lib"): CONFIG(shared, static|staticlib|shared|dll)|CONFIG(dll, static|staticlib|shared|dll) {
        DESTDIR = $$getLibDestdir($$_PRO_FILE_PWD_)
        LIB_TYPE = shared
    }
    MOC_DIR = $${EXAMPLE_MOC}/$${rel_path}
    OBJECTS_DIR = $${EXAMPLE_OBJ}/$${rel_path}
} else { # out-of-source build / shadow build
    equals(TEMPLATE, "app") {
        DESTDIR = $$DESTDIR_app
    } else: equals(TEMPLATE, "lib"): CONFIG(static, static|staticlib|shared|dll)|CONFIG(staticlib, static|staticlib|shared|dll) {
        DESTDIR = $$getLibDestdir($$_PRO_FILE_PWD_)
        LIB_TYPE = static
    } else: equals(TEMPLATE, "lib"): CONFIG(shared, static|staticlib|shared|dll)|CONFIG(dll, static|staticlib|shared|dll) {
        DESTDIR = $$getLibDestdir($$_PRO_FILE_PWD_)
        LIB_TYPE = shared
    }
    # shadow build should handle moc and obj paths automatically
}
