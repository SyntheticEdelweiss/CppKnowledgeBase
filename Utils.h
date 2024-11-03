#pragma once

#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>

#include <QtCore/QDateTime>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGlobal>

#ifndef under_cast
#define under_cast(EnumValue) static_cast<std::underlying_type_t<decltype(EnumValue)>>(EnumValue)
#endif

#ifndef c_cast
#define c_cast(type, var) (type)(var)
#endif

#ifndef UNIQUE_VARNAME
#define UNIQUE_VARNAME_IMPL(lineno) _a_local_var##lineno
#define UNIQUE_VARNAME_IMPL_PREP(lineno) UNIQUE_VARNAME_IMPL(lineno)
#define UNIQUE_VARNAME UNIQUE_VARNAME_IMPL_PREP(__LINE__)
#endif

class LogDuration
{
public:
    explicit LogDuration(const std::string& msg, const std::function<bool(std::chrono::milliseconds)> func = [](std::chrono::milliseconds){ return true; })
        : message(msg)
        , condition(func)
        , start(std::chrono::system_clock::now())
    {}

    ~LogDuration()
    {
        using namespace std::chrono;
        system_clock::time_point end = system_clock::now();
        auto elapsedTime = duration_cast<milliseconds>(end - start);

        if (condition(elapsedTime))
        {
            // Qt variant
            QString t = QString(": start %1; end %2; ")
                        .arg( QDateTime::fromMSecsSinceEpoch(duration_cast<milliseconds>(start.time_since_epoch()).count()).toString("yyyy.MM.dd-hh:mm:ss.zzz")
                            , QDateTime::fromMSecsSinceEpoch(duration_cast<milliseconds>(end.time_since_epoch()).count()).toString("yyyy.MM.dd-hh:mm:ss.zzz"));
            std::cout << message << t.toStdString() << "Elapsed time " << elapsedTime.count() << " ms" << std::endl;

            // non-Qt variant, inconvenient and uses c-style api
            /*const auto start_ms = duration_cast<milliseconds>(start.time_since_epoch()).count()
                                - duration_cast<milliseconds>(duration_cast<seconds>(start.time_since_epoch())).count();
            const auto end_ms = duration_cast<milliseconds>(end.time_since_epoch()).count()
                              - duration_cast<milliseconds>(duration_cast<seconds>(end.time_since_epoch())).count();
            time_t start_tt = system_clock::to_time_t(start);
            time_t end_tt = system_clock::to_time_t(end);
            std::cout << message
                      << ": start " << std::put_time(std::gmtime(&start_tt), "%Y.%m.%d-%H:%M:%S") << "." << start_ms
                      << "; end "   << std::put_time(std::gmtime(&end_tt)  , "%Y.%m.%d-%H:%M:%S") << "." << end_ms
                      << "; Elapsed time " << elapsedTime.count() << " ms"
                      << std::endl;*/

            // requires c++20 and std::format still has no support for resolution higher than seconds
            /*const auto start_ms = duration_cast<milliseconds>(start.time_since_epoch()).count()
                                - duration_cast<milliseconds>(duration_cast<seconds>(start.time_since_epoch())).count();
            const auto end_ms = duration_cast<milliseconds>(end.time_since_epoch()).count()
                              - duration_cast<milliseconds>(duration_cast<seconds>(end.time_since_epoch())).count();
            std::cout << message
                      << ": start " << std::format("{%Y.%m.%d-%H:%M:%S}", start) << "." << start_ms
                      << "; end "   << std::format("{%Y.%m.%d-%H:%M:%S}", end)  << "." << end_ms
                      << "; Elapsed time " << elapsedTime.count() + " ms"
                      << std::endl;*/
        }
    }

    // example: LOG_DURATION_CONDITION("message", LogDuration::elapsedLessThanLimit(std::chrono::milliseconds(500)));
    inline static std::function<bool(std::chrono::milliseconds)> elapsedLessThanLimit(const std::chrono::milliseconds limit)
    {
        return [limit](const std::chrono::milliseconds elapsedTime){ return elapsedTime < limit; };
    }
    inline static std::function<bool(std::chrono::milliseconds)> elapsedMoreThanLimit(const std::chrono::milliseconds limit)
    {
        return [limit](const std::chrono::milliseconds elapsedTime){ return elapsedTime > limit; };
    }

private:
    const std::string message;
    const std::function<bool(std::chrono::milliseconds)> condition;
    std::chrono::system_clock::time_point start;
//    constexpr static char dt_format[] = "%Y.%m.%d-%H:%M:%S"; // constexpr -> requires c++17
//    constexpr static QStringView dt_format = u"yyyy.MM.dd-hh:mm:ss.zzz";
};

#define LOG_DURATION(message) \
    LogDuration UNIQUE_VARNAME {message};
#define LOG_DURATION_CONDITION(message, condition) \
    LogDuration UNIQUE_VARNAME {message, condition};

class CtorDtorNotifier
{
public:
    CtorDtorNotifier();
    virtual ~CtorDtorNotifier();
    CtorDtorNotifier(const CtorDtorNotifier&);               // Copy constructor
    CtorDtorNotifier& operator=(const CtorDtorNotifier&);    // Copy assignment
    CtorDtorNotifier(CtorDtorNotifier&&);                    // Move constructor
    CtorDtorNotifier& operator=(CtorDtorNotifier&&);         // Move assignment

    inline int func() { return 5; }
    uint* counter_ptr;
    uint id;
};

class MetaObjectConnectionWrapper
{
public:
    ~MetaObjectConnectionWrapper() { QObject::disconnect(m_connection); }
    MetaObjectConnectionWrapper(QMetaObject::Connection& a_connection) : m_connection(a_connection) {}
    MetaObjectConnectionWrapper(const MetaObjectConnectionWrapper&)            = delete; // Copy constructor
    MetaObjectConnectionWrapper(MetaObjectConnectionWrapper&&)                 = delete; // Move constructor
    MetaObjectConnectionWrapper& operator=(const MetaObjectConnectionWrapper&) = delete; // Copy assignment
    MetaObjectConnectionWrapper& operator=(MetaObjectConnectionWrapper&&)      = delete; // Move assignment

private:
    QMetaObject::Connection m_connection;
};

#define WRAP_QT_CONNECTION(a_connection) \
    MetaObjectConnectionWrapper UNIQUE_VARNAME {a_connection};
