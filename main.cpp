#include "SignalSlotTest.h"
#include "MultithreadingTest.h"

#include <QtCore/QCoreApplication>
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
//    QApplication::setStyle("Fusion");
//    QApplication app(argc, argv);
    QCoreApplication app(argc, argv);

//    runSignalSlotTest();
//    runMultithreadingTest();

    return app.exec();
}
