#pragma once

#include <functional>
#include <memory>

#include <QtCore/QObject>

#include "Utils.h"

// different arguments and modifiers for signals & slots
void signalSlotArgTest();
// signal & slot interactions depending on QThread & QEventLoop working
void signalSlotThreadTest();

typedef std::function<void(const CtorDtorNotifier&)> StdFunctionConstRefArgWrapper;

class SignalEmitter : public QObject
{
    Q_OBJECT
public:
    ~SignalEmitter();
public slots:
    void emitSignalCopy();
    void emitSignalConstRef();
    void emitSignalMixedArgs();
    void emitSignalSharedPtr();
    void emitSignalPtrConstRef();

    void emitSignalStdFunctionCopyArg();
    void emitSignalStdFunctionRefArg();
    void emitSignalStdFunctionConstRefArgWrapper();
signals:
    void signalCopy(CtorDtorNotifier);
    void signalConstRef(CtorDtorNotifier const&);
    void signalMixedArgs(CtorDtorNotifier const&, CtorDtorNotifier);
    void signalSharedPtr(std::shared_ptr<CtorDtorNotifier> const&);
    void signalPtrConstRefArg(CtorDtorNotifier* const&);

    void signalStdFunctionCopyArg(const std::function<void(CtorDtorNotifier)>&, const CtorDtorNotifier&);
    void signalStdFunctionRefArg(const std::function<void(CtorDtorNotifier&)>&, const CtorDtorNotifier&);
    void signalStdFunctionConstRefArgWrapper(const StdFunctionConstRefArgWrapper&, const CtorDtorNotifier&);
    // void signalStdFunctionConstRefArg(std::function<void(CtorDtorNotifier const&)> const&, CtorDtorNotifier const&); // compiler error due to incorrect MOC output

};

class SlotCaller : public QObject
{
    Q_OBJECT
public:
    ~SlotCaller();
    static void stdFunctionTest(const CtorDtorNotifier&);
public slots:
    void processNothing();

    void processCopy(CtorDtorNotifier);
    void processConstRef(CtorDtorNotifier const&);
    void processMixedArgs(CtorDtorNotifier const&, CtorDtorNotifier);
    void processSharedPtr(std::shared_ptr<CtorDtorNotifier> const&);
    void processPtrConstRefArg(CtorDtorNotifier* const&);

    void processStdFunctionCopyArg(const std::function<void(CtorDtorNotifier)>&, const CtorDtorNotifier&);
    void processStdFunctionRefArg(const std::function<void(CtorDtorNotifier&)>&, const CtorDtorNotifier&);
    void processStdFunctionConstRefArgWrapper(const StdFunctionConstRefArgWrapper&, const CtorDtorNotifier&);
    // void processStdFunctionConstRefArg(std::function<void(CtorDtorNotifier const&)> const&, CtorDtorNotifier const&); // compiler error due to incorrect MOC output    
};
