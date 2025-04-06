#include <iostream>

#include "libraryA/LibraryA.h"
// #include "libraryB/LibraryB.h"
// #include "libraryC/LibraryC.h"

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    LibraryA().bark();
    // LibraryB().bark();
    // LibraryC().bark();
    return 0;
}
