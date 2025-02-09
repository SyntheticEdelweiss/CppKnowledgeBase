#include "QThreadTest.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtDebug>

QThread* g_pSecondaryThread = nullptr;
QThread* g_pTertiaryThread = nullptr;
QTimer* g_pTimerSecondary = new QTimer;
QTimer* g_pTimerTertiary = new QTimer;

void createTertiaryThread()
{
    g_pTertiaryThread = new QThread();
    // g_pTertiaryThread->moveToThread(QCoreApplication::instance()->thread()); // [2.2]

    QTimer* threadStatusCheckTimer = new QTimer;
    threadStatusCheckTimer->moveToThread(g_pTertiaryThread);
    threadStatusCheckTimer->setInterval(200);
    QObject::connect(threadStatusCheckTimer, &QTimer::timeout, [](){ qWarning() << "tertiary thread check" << QDateTime::currentDateTimeUtc(); });
    QObject::connect(g_pTertiaryThread, &QThread::started, threadStatusCheckTimer, qOverload<>(&QTimer::start));
    QObject::connect(g_pTertiaryThread, &QThread::finished, threadStatusCheckTimer, &QObject::deleteLater);

    // important to make it DirectConnection, as QueuedConnection will be scheduled after Secondary's deletion -> Tertiary will have nobody to process quit() slot
    // QObject::connect(g_pSecondaryThread, &QThread::finished, g_pTertiaryThread, &QThread::quit, Qt::DirectConnection); // [2.3]
    // QObject::connect(g_pSecondaryThread, &QThread::finished, [](){ g_pTertiaryThread->moveToThread(g_pSecondaryThread->thread()); }); // [2.4]

    QObject::connect(g_pTertiaryThread, &QThread::started, []() {
        qWarning() << QString("Started Tertiary thread (0x%1) which lives in 0x%2")
                      .arg(QString::number(reinterpret_cast<std::intptr_t>(g_pTertiaryThread), 16))
                      .arg(QString::number(reinterpret_cast<std::intptr_t>(g_pTertiaryThread->thread()), 16));
    });
    QObject::connect(g_pTimerTertiary, &QTimer::timeout, g_pTertiaryThread, &QThread::quit, Qt::QueuedConnection);
    QObject::connect(g_pTertiaryThread, &QThread::finished, g_pTertiaryThread, &QThread::deleteLater);
    QObject::connect(g_pTertiaryThread, &QThread::finished, []() { qWarning() << "Finished Tertiary thread"; });
    QObject::connect(g_pTertiaryThread, &QThread::destroyed, []() { qWarning() << "Destroyed Tertiary thread"; });
    g_pTertiaryThread->start();
    return;
}

void createSecondaryThread()
{
    g_pSecondaryThread = new QThread();

    QTimer* threadStatusCheckTimer = new QTimer;
    threadStatusCheckTimer->moveToThread(g_pSecondaryThread);
    threadStatusCheckTimer->setInterval(200);
    QObject::connect(threadStatusCheckTimer, &QTimer::timeout, [](){ qWarning() << "secondary thread check" << QDateTime::currentDateTimeUtc(); });
    QObject::connect(g_pSecondaryThread, &QThread::started, threadStatusCheckTimer, qOverload<>(&QTimer::start));
    QObject::connect(g_pSecondaryThread, &QThread::finished, threadStatusCheckTimer, &QObject::deleteLater);

    QObject::connect(g_pSecondaryThread, &QThread::started, []() {
        qWarning() << QString("Started Secondary thread (0x%1) which lives in 0x%2")
                      .arg(QString::number(reinterpret_cast<std::intptr_t>(g_pSecondaryThread), 16))
                      .arg(QString::number(reinterpret_cast<std::intptr_t>(g_pSecondaryThread->thread()), 16));
        QTimer::singleShot(0, QCoreApplication::instance(), []() { createTertiaryThread(); }); // [1]
        // QTimer::singleShot(0, []() { createTertiaryThread(); }); // [2]
    });
    QObject::connect(g_pTimerSecondary, &QTimer::timeout, g_pSecondaryThread, &QThread::quit, Qt::QueuedConnection);
    QObject::connect(g_pSecondaryThread, &QThread::finished, g_pSecondaryThread, &QThread::deleteLater);
    QObject::connect(g_pSecondaryThread, &QThread::finished, []() { qWarning() << "Finished Secondary thread"; });
    QObject::connect(g_pSecondaryThread, &QThread::destroyed, []() { qWarning() << "Destroyed Secondary thread"; });
    g_pSecondaryThread->start();
    return;
}

void runMultithreadingTest()
{
    /* [1] - Both Secondary and Tertiary threads live in Main thread, so slots connected to finished & destroyed signals are processed in Main (evident by respective messages).
     * Changing [1] to [2] results in Tertiary living in Secondary. Since Secondary is deleted before Tertiary, by the time Tertiary should be deleted it lives in no thread, so respective slots have nobody to process them.
     * Enabling [2.1] or [2.2] achieves the same result as [1]
     * Enabling [2.3] prevents dangling thread by making Tertiary quit right before Secondary quit
     * Enabding [2.4] prevents dangling thread by moving Tertiary's to Secondary's thread (Main) before quitting Secondary
     * */
    QTimer::singleShot(0, [](){
        qWarning() << QString("Main thead: 0x%1").arg(QString::number(reinterpret_cast<std::intptr_t>(QCoreApplication::instance()->thread()), 16));
        createSecondaryThread();

        // QTimer::singleShot(100, [](){ QTimer::singleShot(400, g_pTertiaryThread, [](){ g_pTertiaryThread->moveToThread(QCoreApplication::instance()->thread()); }); }); // [2.1]
        // QTimer::singleShot(400, g_pTertiaryThread, [](){ g_pTertiaryThread->moveToThread(QCoreApplication::instance()->thread()); }); // wrong - can't call move from unaffiliated thread

        g_pTimerSecondary->setSingleShot(true);
        g_pTimerSecondary->start(1000); // delete Secondary thread after 1 second
        g_pTimerTertiary->setSingleShot(true);
        g_pTimerTertiary->start(2000); // delete Tertiary thread after 2 seconds
    });
    return;
}
