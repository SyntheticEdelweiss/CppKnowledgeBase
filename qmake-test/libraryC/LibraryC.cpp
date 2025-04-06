#include "LibraryC.h"

#include <iostream>

LibraryC::LibraryC()
{
}

void LibraryC::bark() const
{
    std::cout << __PRETTY_FUNCTION__ << "  string C" << std::endl;
}
