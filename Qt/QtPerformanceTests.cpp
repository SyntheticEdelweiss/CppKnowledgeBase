#include "QtPerformanceTests.h"

#include <QtCore/QList>

#include "Utils.h"

void sharedDataInRangeBasedLoop()
{
    QList<CtorDtorNotifier> sourceList;
    sourceList.push_back(CtorDtorNotifier());
    sourceList.push_back(CtorDtorNotifier());
    QList<uint> dump;
    auto sharedSource1 = sourceList; // here nothing is copied yet, only pointer is assigned to shared data
    if (true)
    {
        std::cout << "---------- test no const" << std::endl;
        // here copy happens because non-const begin() is called while sourceList shares data with sharedSource1, so it causes detach()
        for (auto& e : sourceList) // auto is 'CtorDtorNotifier'
            dump.push_back(e.id);
    }
    auto sharedSource2 = sourceList; // again, nothing is copied here yet
    if (true)
    {
        std::cout << "---------- test qAsConst" << std::endl;
        // not copied here because qAsConst adds const modifier to sourceList -> 'begin() const' is called instead of 'begin()' -> it's assured that shared data won't change -> no detach() call
        for (auto& e : qAsConst(sourceList)) // auto is 'const CtorDtorNotifier'
            dump.push_back(e.id);
    }
    auto sharedSource3 = sourceList; // again, nothing is copied here yet
    if (true)
    {
        std::cout << "---------- test auto const&" << std::endl;
        // pay attention at CtorDtorNotifier id's - detach() in "test no const" makes it so that sharedSource1 contains original elements, and sourceList contains copies
        // copy happens 'begin()' is called even though 'e' is const - const modifier is applied after member call
        for (auto const& e : sourceList) // auto is 'CtorDtorNotifier'
            dump.push_back(e.id);
    }
    std::cout << "---------- finished testing" << std::endl;
}
