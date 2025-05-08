#include "SignalSlotTest.h"

#include <QtCore/QEventLoop>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtCore/QCoreApplication>
#include <QtDebug>


void signalSlotArgTest()
{
    // register required types
    qRegisterMetaType<CtorDtorNotifier>("CtorDtorNotifier");
    qRegisterMetaType<std::shared_ptr<CtorDtorNotifier>>("std::shared_ptr<CtorDtorNotifier>");
    qRegisterMetaType<std::function<void(CtorDtorNotifier)>>("std::function<void(CtorDtorNotifier)>");
    qRegisterMetaType<std::function<void(CtorDtorNotifier&)>>("std::function<void(CtorDtorNotifier&)>");
    qRegisterMetaType<StdFunctionConstRefArgWrapper>("StdFunctionConstRefArgWrapper");

    const size_t receiversCount = 1; // affects number of signal receivers
    SignalEmitter* src = new SignalEmitter;
    std::array<SlotCaller*, receiversCount> rcvArray;
    for (size_t i = 0; i < receiversCount; ++i)
        rcvArray[i] = new SlotCaller;
    std::array<QThread*, receiversCount> threadsArray;
    if (false) // multithreading
    {
        for (size_t i = 0; i < receiversCount; ++i)
        {
            threadsArray[i] = new QThread;
            rcvArray[i]->moveToThread(threadsArray[i]);
            threadsArray[i]->start();
        }
    }

    int testNumber = 3;
    switch(testNumber)
    {
    case -2: // const ref -> copy
    {
        for (size_t i = 0; i < receiversCount; ++i)
            QObject::connect(src, &SignalEmitter::signalConstRef, rcvArray[i], &SlotCaller::processCopy, Qt::QueuedConnection);
        QTimer::singleShot(0, src, &SignalEmitter::emitSignalConstRef);
        break;
    }
    case -1: // const ref -> nothing
    {
        for (size_t i = 0; i < receiversCount; ++i)
            QObject::connect(src, &SignalEmitter::signalConstRef, rcvArray[i], &SlotCaller::processNothing, Qt::QueuedConnection);
        QTimer::singleShot(0, src, &SignalEmitter::emitSignalConstRef);
        break;
    }
    case 1: // copy -> nothing
    {
        for (size_t i = 0; i < receiversCount; ++i)
            QObject::connect(src, &SignalEmitter::signalCopy, rcvArray[i], &SlotCaller::processNothing, Qt::QueuedConnection);
        QTimer::singleShot(0, src, &SignalEmitter::emitSignalCopy);
        break;
    }
    case 2: // copy -> copy
    {
        for (size_t i = 0; i < receiversCount; ++i)
            QObject::connect(src, &SignalEmitter::signalCopy, rcvArray[i], &SlotCaller::processCopy, Qt::QueuedConnection);
        QTimer::singleShot(0, src, &SignalEmitter::emitSignalCopy);
        break;
    }
    case 3: // const ref -> const ref
    {
        for (size_t i = 0; i < receiversCount; ++i)
            QObject::connect(src, &SignalEmitter::signalConstRef, rcvArray[i], &SlotCaller::processConstRef, Qt::QueuedConnection);
        QTimer::singleShot(0, src, &SignalEmitter::emitSignalConstRef);
        break;
    }
    case 4: // mixed args -> mixed args
    {
        for (size_t i = 0; i < receiversCount; ++i)
            QObject::connect(src, &SignalEmitter::signalMixedArgs, rcvArray[i], &SlotCaller::processMixedArgs, Qt::QueuedConnection);
        QTimer::singleShot(0, src, &SignalEmitter::emitSignalMixedArgs);
        break;
    }
    case 5: // shared_ptr -> shared_ptr
    {
        for (size_t i = 0; i < receiversCount; ++i)
            QObject::connect(src, &SignalEmitter::signalSharedPtr, rcvArray[i], &SlotCaller::processSharedPtr, Qt::QueuedConnection);
        QTimer::singleShot(0, src, &SignalEmitter::emitSignalSharedPtr);
        break;
    }
    case 6: // ptr const ref -> ptr const ref
    {
        for (size_t i = 0; i < receiversCount; ++i)
            QObject::connect(src, &SignalEmitter::signalPtrConstRefArg, rcvArray[i], &SlotCaller::processPtrConstRefArg, Qt::QueuedConnection);
        QTimer::singleShot(0, src, &SignalEmitter::emitSignalPtrConstRef);
        break;
    }
    case 7: // std::function copy arg -> process
    {
        for (size_t i = 0; i < receiversCount; ++i)
            QObject::connect(src, &SignalEmitter::signalStdFunctionCopyArg, rcvArray[i], &SlotCaller::processStdFunctionCopyArg, Qt::QueuedConnection);
        QTimer::singleShot(0, src, &SignalEmitter::emitSignalStdFunctionCopyArg);
        break;
    }
    case 8: // std::function ref arg -> process
    {
        for (size_t i = 0; i < receiversCount; ++i)
            QObject::connect(src, &SignalEmitter::signalStdFunctionRefArg, rcvArray[i], &SlotCaller::processStdFunctionRefArg, Qt::QueuedConnection);
        QTimer::singleShot(0, src, &SignalEmitter::emitSignalStdFunctionRefArg);
        break;
    }
    case 9: // std::function const ref arg wrapper -> process
    {
        for (size_t i = 0; i < receiversCount; ++i)
            QObject::connect(src, &SignalEmitter::signalStdFunctionConstRefArgWrapper, rcvArray[i], &SlotCaller::processStdFunctionConstRefArgWrapper, Qt::QueuedConnection);
        QTimer::singleShot(0, src, &SignalEmitter::emitSignalStdFunctionConstRefArgWrapper);
        break;
    }
    default: { std::cout << "No test selected!\n"; break; }
    }
    QEventLoop localLoop;
    localLoop.exec();
    return;
}

// signal & slot interactions depending on QThread & QEventLoop working
void signalSlotThreadTest()
{
    const bool testEmitFromMainThread = true;
    const bool o_1 = true; // [1] - start thread before emitting signal
    const bool o_2 = !o_1; // [2] - start thread after emitting signal and sleeping
    static_assert(o_1 != o_2);

    CtorDtorNotifierQt* cdn = new CtorDtorNotifierQt;
    QThread* sideThread = new QThread;
    QObject* contextHelper = new QObject;
    contextHelper->moveToThread(sideThread);

    if (testEmitFromMainThread)
    {
        // [1]
        if constexpr(o_1) {
            qWarning() << QDateTime::currentDateTimeUtc() << QThread::currentThread() << "starting sideThread [1]";
            sideThread->start();
        }

        // Connected slot will be called in Main, which is affiliated with object
        // Processing DirectConnection doesn't even require any running QEventLoop -> slot will be called immediately after emitting connected signal
        QObject::connect(cdn, &CtorDtorNotifierQt::testSignal, [](){
            qWarning() << QDateTime::currentDateTimeUtc() << QThread::currentThread() << "DirectConnection lambda called";
        });
        // Connected slot will be called in Side, which is affiliated with contextHelper
        // Queued slot won't be called until Side is started, since its QEventLoop is supposed to process events, including signals from other threads
        QObject::connect(cdn, &CtorDtorNotifierQt::testSignal, contextHelper, [](){
            qWarning() << QDateTime::currentDateTimeUtc() << QThread::currentThread() << "QueuedConnection lambda called";
        }, Qt::QueuedConnection);

        // Direct slot will be called in both [1] and [2]
        // Queued slot will be called only in [1], since in [2] Side is not started yet
        qWarning() << QDateTime::currentDateTimeUtc() << QThread::currentThread() << "emitting signal";
        emit cdn->testSignal();
        qWarning() << QDateTime::currentDateTimeUtc() << QThread::currentThread() << "start sleeping 500 msec";
        QThread::currentThread()->msleep(500);

        // [2]
        if constexpr(o_2) {
            qWarning() << QDateTime::currentDateTimeUtc() << QThread::currentThread() << "starting sideThread [2]";
            sideThread->start();
            // Queued slot will be called despite Main sleeping, because it's connected to execute in Side
            qWarning() << QDateTime::currentDateTimeUtc() << QThread::currentThread() << "start sleeping 500 msec";
            QThread::currentThread()->msleep(500);
        }
    }
    else
    {
        QObject::connect(cdn, &CtorDtorNotifierQt::testSignal, [](){
            qWarning() << QDateTime::currentDateTimeUtc() << QThread::currentThread() << "DirectConnection lambda called";
        });
        QObject::connect(cdn, &CtorDtorNotifierQt::testSignal, QCoreApplication::instance(), [](){
            qWarning() << QDateTime::currentDateTimeUtc() << QThread::currentThread() << "QueuedConnection lambda called";
        }, Qt::QueuedConnection);

        // NOTE: capturing &cdn causes segfault - cdn goes out of scope before it's accessed in slot, so it becomes a dangling reference to a pointer, even though the value that pointer used to point to is valid
        QObject::connect(sideThread, &QThread::started, [cdn](){
            // DirectConnection is processed immediately
            // QueuedConnection is connected to QCoreApplication::instance() thus it's processed in Main and doesn't wait until after Side has finished sleep
            emit cdn->testSignal(); // connected slots are called as expected - <this> thread's event loop doesn't need to execute processEvents()
            QThread::currentThread()->msleep(1000);
            qWarning() << QDateTime::currentDateTimeUtc() << QThread::currentThread() << "Finished sleep";
        });
        sideThread->start();
    }
}



SignalEmitter::~SignalEmitter()
{
    std::cout << std::string("Called SignalEmitter::~SignalEmitter()\n");
}

void SignalEmitter::emitSignalCopy()
{
    std::cout << std::string("Called SignalEmitter::emitSignalCopy()\n");
    CtorDtorNotifier t;
    emit signalCopy(t);
    return;
}

void SignalEmitter::emitSignalConstRef()
{
    std::cout << std::string("Called SignalEmitter::emitSignalConstRef()\n");
    CtorDtorNotifier t;
    emit signalConstRef(t);
    return;
}

void SignalEmitter::emitSignalSharedPtr()
{
    std::cout << std::string("Called SignalEmitter::emitSignalSharedPtr()\n");
    std::shared_ptr<CtorDtorNotifier> ptr = std::make_shared<CtorDtorNotifier>();
    emit signalSharedPtr(ptr);
    return;
}

void SignalEmitter::emitSignalMixedArgs()
{
    std::cout << std::string("Called SignalEmitter::emitSignalMixedArgs()\n");
    CtorDtorNotifier t1;
    CtorDtorNotifier t2;
    emit signalMixedArgs(t1, t2);
    return;
}

void SignalEmitter::emitSignalPtrConstRef()
{
    std::cout << std::string("Called SignalEmitter::emitSignalPtrConstRef()\n");
    CtorDtorNotifier* t = new CtorDtorNotifier;
    std::cout << std::string("arg address=")
              << (void*)&t
              << '\n';
    emit signalPtrConstRefArg(t);
    return;
}

void SignalEmitter::emitSignalStdFunctionCopyArg()
{
    std::cout << std::string("Called SignalEmitter::emitSignalStdFunctionCopyArg()\n");
    CtorDtorNotifier t;
    std::function<void(CtorDtorNotifier const&)> f_func = SlotCaller::stdFunctionTest;
    emit signalStdFunctionCopyArg(f_func, t);
    return;
}

void SignalEmitter::emitSignalStdFunctionRefArg()
{
    std::cout << std::string("Called SignalEmitter::emitSignalStdFunctionRefArg()\n");
    CtorDtorNotifier t;
    std::function<void(CtorDtorNotifier const&)> f_func = SlotCaller::stdFunctionTest;
    emit signalStdFunctionRefArg(f_func, t);
    return;
}

void SignalEmitter::emitSignalStdFunctionConstRefArgWrapper()
{
    std::cout << std::string("Called SignalEmitter::emitSignalStdFunctionConstRefArgWrapper()\n");
    CtorDtorNotifier t;
    std::function<void(CtorDtorNotifier const&)> f_func = SlotCaller::stdFunctionTest;
    emit signalStdFunctionConstRefArgWrapper(f_func, t);
    return;
}


SlotCaller::~SlotCaller()
{
    std::cout << std::string("Called SlotCaller::~SlotCaller()\n");
    return;
}

void SlotCaller::processNothing()
{
    std::cout << std::string("Called SlotCaller::processNothing()\n");
    return;
}

void SlotCaller::processCopy(CtorDtorNotifier t)
{
    std::string s = std::string("Called SlotCaller::processCopy() id=")
                  + std::to_string(t.id)
                  + '\n';
    std::cout << s;
    return;
}

void SlotCaller::processConstRef(CtorDtorNotifier const& t)
{
    std::string s = std::string("Called SlotCaller::processConstRef() id=")
                  + std::to_string(t.id)
                  + '\n';
    std::cout << s;
    return;
}

void SlotCaller::processMixedArgs(CtorDtorNotifier const& t1, CtorDtorNotifier t2)
{
    std::string s = std::string("Called SlotCaller::processMixedArgs() id1=")
                  + std::to_string(t1.id)
                  + std::string(" id2=")
                  + std::to_string(t2.id)
                  + '\n';
    std::cout << s;
    return;
}

void SlotCaller::processSharedPtr(std::shared_ptr<CtorDtorNotifier> const& ptr)
{
    std::string s = std::string("Called SlotCaller::processSharedPtr() id=")
                  + std::to_string(ptr->id)
                  + '\n';
    std::cout << s;
    return;
}

void SlotCaller::processPtrConstRefArg(CtorDtorNotifier* const& t)
{
    std::cout << std::string("Called SlotCaller::processPtrConstRefArg() id=")
              << std::to_string(t->id)
              << std::string("; arg address=")
              << (void*)&t
              << '\n';
    return;
}

// NOTE: f_func here has different argument than the function assigned in SignalEmitter::emitSignalStdFunctionCopyArg
void SlotCaller::processStdFunctionCopyArg(std::function<void(CtorDtorNotifier)> const& f_func, CtorDtorNotifier const& t)
{
    std::string s = std::string("Called SlotCaller::processStdFunctionCopyArg() id=")
                  + std::to_string(t.id)
                  + '\n';
    std::cout << s;
    f_func(t);
    return;
}

// NOTE: f_func here has different argument than the function assigned in SignalEmitter::emitSignalStdFunctionRefArg
void SlotCaller::processStdFunctionRefArg(std::function<void(CtorDtorNotifier&)> const& f_func, CtorDtorNotifier const& t)
{
    std::string s = std::string("Called SlotCaller::processStdFunctionRefArg() id=")
                  + std::to_string(t.id)
                  + '\n';
    std::cout << s;
    f_func(const_cast<CtorDtorNotifier&>(t)); // const_cast required, otherwise "discards qualifiers" error happens
    return;
}

void SlotCaller::processStdFunctionConstRefArgWrapper(std::function<void(CtorDtorNotifier const&)> const& f_func, CtorDtorNotifier const& t)
{
    std::string s = std::string("Called SlotCaller::processStdFunctionConstRefArgWrapper() id=")
                  + std::to_string(t.id)
                  + '\n';
    std::cout << s;
    f_func(t);
    return;
}

/* // Function commented out due to incorrect MOC output. For more info check compiler error with uncommented function.
void SlotCaller::processStdFunctionConstRefArg(std::function<void(CtorDtorNotifier const&)> const& f_func, CtorDtorNotifier const& t)
{
    std::string s = std::string("Called SlotCaller::processStdFunctionConstRefArg() id=")
                  + std::to_string(t.id)
                  + '\n';
    std::cout << s;
    f_func(t);
    return;
}*/

void SlotCaller::stdFunctionTest(CtorDtorNotifier const& t)
{
    std::string s = std::string("Called SlotCaller::stdFunctionTest() id=")
                  + std::to_string(t.id)
                  + '\n';
    std::cout << s;
    return;
}

void sigSlotInheritanceTest()
{
    // can't instantiate ObjectBase when ObjectBase::barkPure is actually pure virtual
    //std::unique_ptr<ObjectBase> base_ptr = std::make_unique<ObjectBase>();
    //emit base_ptr->sig();
    std::unique_ptr<ObjectDerived> derived_ptr = std::make_unique<ObjectDerived>();
    emit derived_ptr->sig();
}

ObjectBase::ObjectBase(QObject* parent)
    : QObject(parent)
{
    connect(this, &ObjectBase::sig, this, &ObjectBase::bark);
    connect(this, &ObjectBase::sig, this, &ObjectBase::barkPure);
}

ObjectDerived::ObjectDerived(QObject *parent)
    : ObjectBase(parent)
{
    // can't connect to protected slot like Base::slotFunc, but can like Derived::slotFunc even if Derived has no override
    // connect(this, &ObjectBase::sig, this, &ObjectBase::barkProtected);
    connect(this, &ObjectBase::sig, this, &ObjectDerived::barkProtected);
}
