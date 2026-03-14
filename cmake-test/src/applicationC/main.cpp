#include <iostream>

#include <QtCore/QDebug>
#include <QtCore/QString>

// #include "libraryA/LibraryA.h"
// #include "libraryB/LibraryB.h"
// #include "libraryC/LibraryC.h"

using namespace std;

int main()
{
    cout << "ApplicationC: Hello World!" << endl;
    // LibraryA().bark();
    // LibraryB().bark();
    // LibraryC().bark();
    qDebug() << QString("Qt Hello World!");
    return 0;
}
