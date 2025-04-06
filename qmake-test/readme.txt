CONFIG settings in superproject have no effect on subprojects. CONFIG in each .pro file affects only the project represented by this .pro file.
Variables assigned in .pro file are scoped only to that .pro file. There's no way to carry over anything from one .pro to another.
If shadow build is enabled, all subprojects use OUT_PWD of superproject.
If shadow build is disabled, each subproject makes in-source build according to its own .pro file.

Note that I keep saying "rebuild" but really mean "build". The difference is that "rebuild" means genuinely rebuilding from 0, while "build" only rebuilds things that were changed. So in case some lib consists of multiple .o files, "build" would rebuild only the changed ones and the .a itself.
"Build" on superproject affects subprojects whose files were changed. For example, changing LibraryA.cpp and then doing Build on superproject rebuilds libraryA.a, but not applicationA. If main.cpp in subproject applicationA was changed, it would also be rebuilt. Since superpoject specifies dependencies (which guarantees that applicationA will be built after libs it depends on), changing both main.cpp and LibraryA.cpp would effectively result in "optimized rebuild", that is resulting .exe would contain all changes, without the need to rebuild unchanged libraries B and C.
After changing libraryB it's enough to build superproject and rebuild applicationA, for resulting .exe to get all changes. Even though libraryA depends on libraryB and uses function from libraryB, rebuilding libraryA is not required due to but due to the way static libs work. This can be seen via applicationB, which is standalone and properly receives changes after only rebuilding libraryB and applicationB.
Specifiying dependencies in superproject only ensures the relative order (e.g. libs will be built before applicationA, but libraryB and libraryC can be built in unspecified order between each other).
To force applicationA rebuild whenver some file changes, that file should be added to PRE_TARGETDEPS in applicationA.pro. One file that is guaranteed to change whenever anything in the project changes, is the target file(s). So to ensure applicationA rebuild whenever libs change (e.g. LibraryA.cpp change), full path to compiled lib (e.g. liblibraryA.a) must be added to PRE_TARGETDEPS in applicationA.pro

.qmake.conf acts like a global .pri file that is included (not literally) automatically at the beginning of every .pro file in its directory and all subdirectories. Basically, like a local qmake.conf (notice no starting dot; located at QMAKESPEC path).
.qmake.conf seems to be run twice, as evident by message() functions giving output twice. Though the first run seems to be a "dry" one, as setting any variables the first time with e.g. += doesn't result in such variable containing double value. Also, first run seems to be done without even running qmake.conf, as evident by contents of any value set in qmake.conf
Only the closest .qmake.conf takes effect. Consider the following project structure. applicationA.pro would then "include" only /qmake-test/applicationA/.qmake.conf, while libraryA.pro would include /qmake-test/qmake-test.pro
/qmake-test
/qmake-test/qmake-test.pro
/qmake-test/.qmake.conf
/qmake-test/applicationA/applicationA.pro
/qmake-test/applicationA/.qmake.conf
/qmake-test/libraryA/libraryA.pro


******************************************************************************************

:-1: error: No rule to make target 'sub-E--repos-sapfir-IServ-IServ_all----src-libs-Net-qmake_all', needed by 'sub-E--repos-sapfir-IServ-src-IServ-qmake_all'.  Stop.

Error like this can be caused by a bug with qmake .subdir path processing.
Consider a subdirs project like this:

TEMPLATE = subdirs
SUBDIRS += Net IServ
Net.subdir = $$PWD/../src/libs/Net
IServ.subdir = $$PWD/../src/IServ
IServ.depends = Net

It would produce the abovementioned error upon running qmake. But after deleting/commenting "IServ.depends=Net" qmake runs fine. That's because qmake fails to properly process .subdir path for a project if that project is present in some .depends and .subdir contains ".." beyond the start of path.
So this works
    Net.subdir = ../src/libs/Net
and this doesn't 
    Net.subdir = $$PWD/../src/libs/Net
which if fixable by removing .. via absolute_path() or clean_path(), thus this also works
    Net.subdir = $$absolute_path($$PWD/../src/libs/Net)
Alternatively, you can specify subdir paths in SUBDIRS variable and it's processed fine. So this subdirs project would also compile fine:

TEMPLATE = subdirs
SUBDIRS += $$PWD/../src/libs/Net $$PWD/../src/IServ
IServ.depends = Net


******************************************************************************************

:-1: error: [Makefile:119: E:/repos/sapfir 2025.05.30/sapfir/IServ/lib/libNet.a] Error 1

Error like this can be caused by a bug with qmake object_script files processing, emerging when path contains spaces. Look at compile output, it should contain messages like below example.

del "E:\repos\sapfir 2025.05.30\sapfir\IServ\lib\libLoggerSingleton.a" 2>NUL
ar -rc "E:\repos\sapfir 2025.05.30\sapfir\IServ\lib\libLoggerSingleton.a" LoggerSingleton.o moc_LoggerSingleton.o
del "E:\repos\sapfir 2025.05.30\sapfir\IServ\lib\libNet.a" 2>NUL
ar -rc E:\repos\sapfir 2025.05.30\sapfir\IServ\lib\libNet.a @object_script.libNet

Library LoggerSingleton builds successfully, because "ar" call quotes lib path. Library Net build fails, because "ar" call is unquoted while lib path contains space -> command is incorrect and lib is not built. The bug boils down to "ar" calls not quoting path when processing object_script files. The only solution is to change path so it contains no spaces.