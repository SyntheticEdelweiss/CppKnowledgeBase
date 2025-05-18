# Исключаем повторную обработку .pri файла, логически аналогично использованию include guard (#pragma once)
!assureUniquePri($$_FILE_) : return()

# Подключение всех нужных вещей из других библиотек одним простым инклюдом для каждой такой библиотеки.
include($$clean_path($$PWD/path/to/lib_project2/ExampleTertiaryLib.pri))
include($$clean_path($$PWD/path/to/lib_project3/ExampleSecondaryLib.pri))

# Т.к. при подключении библиотеки к другому проекту в том проекте надо подключать используемые в библиотеке QT модули, то подключение модулей выносится в .pri файл.
QT *= \
    core    \
    gui     \
    widgets \
    sql     \
    network

# INCLUDEPATH прописывается через $$PWD, потому что PWD содержит путь к тому файлу, в котором прописана, а ./ содержить путь к тому .pro файлу, который в данный момент собирает qmake.
# Пример: есть 2 проекта, C:\repository\Project_main и C:\repository\SubProject, SubProject является библиотекой и подключается в Project_main.
# Если в SubProject.pri используется INCLUDEPATH *= $$PWD, то результат будет эквивалентен:
#    Этап обработки SubProject.pro :
#    INCLUDEPATH *= C:\repository\SubProject
#    Этап обработки Project_main.pro :
#    INCLUDEPATH *= C:\repository\SubProject
# Если в SubProject.pri используется INCLUDEPATH *= ./ , то результат будет эквивалентен:
#    Этап обработки SubProject.pro :
#    INCLUDEPATH *= C:\repository\SubProject
#    Этап обработки Project_main.pro :
#    INCLUDEPATH *= C:\repository\Project_main

# В .pri файле библиотеки добавляется путь к исходникам самой библиотеки и любые другие нужные пути, чтобы все нужные пути для других проектов получались через один инклюд.
# Путь на 1 директорию выше добавляется для более понятной иерархии инклюдов (#include "libraryName/headerName.h") в .h и .cpp файлах.
# Т.е. чтобы писать, например, #include "Net/NetHeaders.h" - и сразу видно к какой библиотеке относится подключаемый файл.
INCLUDEPATH *= $$clean_path($$PWD/..)

# Добавляем все нужные сторонние библиотеки, саму эту библиотеку и путь к её бинарнику.
# Сразу добавлять в LIBS не получится из-за механизма работы компоновщика и порядка добавления икнлюдов # https://habr.com/ru/post/150327/
# Пример, используемый в библиотеке Net:
win32 {
    LIBS_libs *= -lwsock32 # fixes undefined reference to `__imp_setsockopt'
    LIBS_libs *= -lws2_32  # fixes undefined reference to `__imp_WSAIoctl'
}

# PRE_TARGETDEPS нужен, чтобы при изменении .cpp файлов в библиотеке пересобиралось всё от неё зависящее. Подробнее в проекте qmake-test
# PRE_TARGETDEPS добавляем для приложений и динамических библиотек, статические нет смысла пересобирать лишний раз ввиду особенностей их сборки.
# Т.к. PRE_TARGETDEPS должен указывать на конкретный файл, то тут требуется знать полный путь. Соответственно, переопределяем здесь DESTDIR и TARGET.
# LIB_TYPE определяется в global.pri, соответственно он должен быть подключен заранее.
local_destdir = $$COOPERATION_LIB
local_target = $$basename(PWD)
equals(PWD, $$_PRO_FILE_PWD_) {
	DESTDIR = $$local_destdir
	TARGET = $$basename(PWD)
}

!equals(PWD, $$_PRO_FILE_PWD_) {
    LIBS_paths *= -L$${local_destdir}
    LIBS_libs  *= -l$${local_target}
	!equals(LIB_TYPE, "static") : PRE_TARGETDEPS *= $${local_destdir}/lib$${local_target}.a
}
