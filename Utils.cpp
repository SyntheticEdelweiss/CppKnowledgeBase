#include "Utils.h"

CtorDtorNotifier::CtorDtorNotifier()
{
    static uint counter = 0;
    counter_ptr = &counter;
    id = ++counter;
    std::cout << std::string("Constructed CtorDtorNotifier id=")
              << std::to_string(id)
              << std::endl;
    return;
}

CtorDtorNotifier::~CtorDtorNotifier()
{
    std::cout << std::string("Destroyed CtorDtorNotifier id=")
              << std::to_string(id)
              << std::endl;
    return;
}

CtorDtorNotifier::CtorDtorNotifier(const CtorDtorNotifier& other)
{
    counter_ptr = other.counter_ptr;
    id = ++(*counter_ptr);
    std::cout << std::string("Copy-constructed CtorDtorNotifier id=")
              << std::to_string(id)
              << std::string(" from id=")
              << std::to_string(other.id)
              << std::endl;
    return;
}

CtorDtorNotifier::CtorDtorNotifier(CtorDtorNotifier&& other)
{
    counter_ptr = other.counter_ptr;
    id = ++(*counter_ptr);
    std::cout << std::string("Move-constructed CtorDtorNotifier id=")
              << std::to_string(id)
              << std::string(" from id=")
              << std::to_string(other.id)
              << std::endl;
    return;
}

CtorDtorNotifier& CtorDtorNotifier::operator=(const CtorDtorNotifier& other)
{
    counter_ptr = other.counter_ptr;
    id = ++(*counter_ptr);
    std::cout << std::string("Copy-assigned CtorDtorNotifier id=")
              << std::to_string(id)
              << std::string(" from id=")
              << std::to_string(other.id)
              << std::endl;
    return *this;
}

CtorDtorNotifier& CtorDtorNotifier::operator=(CtorDtorNotifier&& other)
{
    counter_ptr = other.counter_ptr;
    id = ++(*counter_ptr);
    std::cout << std::string("Copy-assigned CtorDtorNotifier id=")
              << std::to_string(id)
              << std::string(" from id=")
              << std::to_string(other.id)
              << std::endl;
    return *this;
}
