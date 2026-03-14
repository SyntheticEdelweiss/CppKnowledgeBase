#include "LibraryD.h"

#include <iostream>

LibraryD::LibraryD()
{
}

void LibraryD::bark() const
{
    std::cout << __PRETTY_FUNCTION__ << "  string D" << std::endl;
}
