#pragma once

#include <functional>
#include <iostream>
#include <memory>

#include <QtCore/QEventLoop>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#include "Utils.h"

void runSignalSlotTest();

typedef std::function<void(const CtorDtorNotifier&)> StdFunctionConstRefArgWrapper;

inline void registerCtorDtorNotifierForQt()
{
    qRegisterMetaType<CtorDtorNotifier>("CtorDtorNotifier");
    qRegisterMetaType<std::shared_ptr<CtorDtorNotifier>>("std::shared_ptr<CtorDtorNotifier>");
    qRegisterMetaType<std::function<void(CtorDtorNotifier)>>("std::function<void(CtorDtorNotifier)>");
    qRegisterMetaType<std::function<void(CtorDtorNotifier&)>>("std::function<void(CtorDtorNotifier&)>");
    qRegisterMetaType<StdFunctionConstRefArgWrapper>("StdFunctionConstRefArgWrapper");
    return;
}

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

