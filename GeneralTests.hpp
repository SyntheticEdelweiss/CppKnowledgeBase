#pragma once

namespace testNamespace_InClassUsingDeclaration
{
class Base
{
public:
    void publicFunc() {}
protected:
    void protectedFunc() {}
private:
    void privateFunc() {}
};

class Derived : public Base
{
public:
    using Base::protectedFunc; // OK: protected -> public
    // using Base::privateFunc;  // ERROR: privateFunc is inaccessible in Base
private:
    using Base::publicFunc; // OK: public -> private
};

inline void test()
{
    Derived d;
    // d.publicFunc(); // ERROR: publicFunc is private in Derived
    d.protectedFunc(); // OK: protectedFunc is now public
}
}
