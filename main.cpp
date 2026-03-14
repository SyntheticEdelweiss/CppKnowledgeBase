#include <QtCore/QCoreApplication>
#include <QtCore/QThread>
#include <QtCore/QtDebug>
#include <QtWidgets/QApplication>

#include "Qt/QThreadTest.h"
#include "Qt/SignalSlotTest.h"

#include "PerformanceTests.h"
#include "Utils.h"


int main(int argc, char *argv[])
{
    (void)__cplusplus;
    // QApplication::setStyle("Fusion");
    // QApplication app(argc, argv);
    QCoreApplication app(argc, argv);
    qWarning() << "main thread:" << QThread::currentThread();

    // signalSlotArgTest();
    // signalSlotThreadTest();
    // runMultithreadingTest();
    // sigSlotInheritanceTest();

    // performance tests
    // ifElseAssignment_vs_lambdaReturnIfElseAssignment();
    // memset_vs_manualAssign();
    // lambdaPerformanceTest();

    // lambdaTypeDeduction();

    return 0;
    return app.exec();
}
