#include "LibraryA.h"

#include "libraryB/LibraryB.h"
#include "libraryC/LibraryC.h"

#include <iostream>

LibraryA::LibraryA()
{
}

void LibraryA::bark() const
{
    std::cout << __PRETTY_FUNCTION__ << " extra string" << std::endl;
    std::cout << "extra call from libA: ";
    LibraryB().bark();
    std::cout << "extra call from libA: ";
    LibraryC().bark();
}
