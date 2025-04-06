#include "LibraryB.h"

#include <iostream>

LibraryB::LibraryB()
{
}

void LibraryB::bark() const
{
    std::cout << __PRETTY_FUNCTION__ << " extra !!! " << std::endl;
}
