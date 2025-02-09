#include "Qt/SignalSlotTest.h"
#include "Qt/QThreadTest.h"
#include "PerformanceTests.h"

#include <QtCore/QCoreApplication>
#include <QtWidgets/QApplication>
#include <QtDebug>
#include <QtCore/QThread>


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

    // performance tests
    // ifElseAssignment_vs_lambdaReturnIfElseAssignment();
    // memset_vs_manualAssign();
    lambdaPerformanceTest();

    // lambdaTypeDeduction();

    return 0;
    return app.exec();
}
