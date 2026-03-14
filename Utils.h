#pragma once

#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QtGlobal>

#define under_cast(EnumValue) static_cast<std::underlying_type_t<decltype(EnumValue)>>(EnumValue)

#define c_cast(type, var) (type)(var)

#define THIS_T std::remove_pointer<decltype(this)>::type

#ifndef UNIQUE_VARNAME
#define UNIQUE_VARNAME_IMPL(counter) _unique_varname##counter
#define UNIQUE_VARNAME_IMPL_PREP(counter) UNIQUE_VARNAME_IMPL(counter)
#define UNIQUE_VARNAME UNIQUE_VARNAME_IMPL_PREP(__COUNTER__)
#endif

#define DISABLE_CLASS_COPY(Class) \
    Class(const Class &) = delete;\
    Class &operator=(const Class &) = delete;

#define DISABLE_CLASS_MOVE(Class) \
    Class(Class &&) = delete; \
    Class &operator=(Class &&) = delete;

#define DISABLE_CLASS_COPY_MOVE(Class) \
    DISABLE_CLASS_COPY(Class) \
    DISABLE_CLASS_MOVE(Class)

class LogDuration
{
public:
    explicit LogDuration(const std::string& msg,
                         const std::function<bool(std::chrono::milliseconds)> func = [](std::chrono::milliseconds) { return true; }
    )
        : message(msg)
        , condition(func)
        , start(std::chrono::system_clock::now())
    {}

    ~LogDuration()
    {
        using namespace std;
        using namespace chrono;
        decltype(start) end = system_clock::now();
        auto elapsed_ms = duration_cast<milliseconds>(end - start);

        if (condition(elapsed_ms))
        {
#if (__cplusplus >= 202002L)
            if constexpr (true) {
                cout << message
                     << ": start " << format(dt_format, time_point_cast<milliseconds>(start))
                     << "; end "   << format(dt_format, time_point_cast<milliseconds>(end))
                     << "; Elapsed time " << elapsed_ms.count() << " ms" << endl;
            }
            else {
                string fmt_string = string("{}: start ") + dt_format + "; end " + dt_format + "; Elapsed time {} ms";
                cout << vformat(fmt_string, make_format_args(message,
                                                             time_point_cast<milliseconds>(start),
                                                             time_point_cast<milliseconds>(end),
                                                             elapsed_ms.count())) << endl;
            }
#elif defined(QDATETIME_H)
            const auto start_dt = QDateTime::fromMSecsSinceEpoch(duration_cast<milliseconds>(start.time_since_epoch()).count());
            const auto end_dt = QDateTime::fromMSecsSinceEpoch(duration_cast<milliseconds>(end.time_since_epoch()).count());
            QString t = QString(": start %1; end %2; ").arg(start_dt.toString(dt_format_qt), end_dt.toString(dt_format_qt));
            cout << message << t.toStdString() << "Elapsed time " << elapsed_ms.count() << " ms" << endl;
#else
            // old c-style api
            const auto start_ms = duration_cast<milliseconds>(start.time_since_epoch()).count() % 1000;
            const auto end_ms = duration_cast<milliseconds>(end.time_since_epoch()).count() % 1000;
            time_t start_tt = system_clock::to_time_t(start);
            time_t end_tt = system_clock::to_time_t(end);
            std::tm start_tm;
            std::tm end_tm;
            #ifdef _WIN32
                localtime_s(&start_tm, &start_tt);
                localtime_s(&end_tm,   &end_tt);
            #else
                localtime_r(&start_tt, &start_tm);
                localtime_r(&end_tt,   &end_tm);
            #endif
            cout << message
                 << ": start " << put_time(&start_tm, "%Y.%m.%d-%H:%M:%S") << "." << setfill('0') << setw(3) << start_ms
                 << "; end "   << put_time(&end_tm, "%Y.%m.%d-%H:%M:%S") << "." << setfill('0') << setw(3) << end_ms
                 << "; Elapsed time " << elapsed_ms.count() << " ms" << endl;
#endif
        }
    }

    // example: LOG_DURATION_CONDITION("message", LogDuration::elapsedLessThanLimit(std::chrono::milliseconds(500)));
    inline static std::function<bool(std::chrono::milliseconds)> elapsedLessThanLimit(const std::chrono::milliseconds limit)
    { return [limit](const std::chrono::milliseconds elapsedTime) { return elapsedTime < limit; }; }
    inline static std::function<bool(std::chrono::milliseconds)> elapsedMoreThanLimit(const std::chrono::milliseconds limit)
    { return [limit](const std::chrono::milliseconds elapsedTime) { return elapsedTime > limit; }; }

private:
    const std::string message;
    const std::function<bool(std::chrono::milliseconds)> condition;
    const std::chrono::system_clock::time_point start;
    constexpr static char dt_format[] = "{:%Y.%m.%d-%H:%M:%S}"; // std::format uses fractional seconds
    constexpr static QStringView dt_format_qt = u"yyyy.MM.dd-hh:mm:ss.zzz";
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

    inline int func() const { return 5; }
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



class CtorDtorNotifierQt : public QObject
{
    Q_OBJECT
public:
    explicit CtorDtorNotifierQt(QObject* parent = nullptr);
    virtual ~CtorDtorNotifierQt();
    // QObject classes can't be copyable by design // https://doc.qt.io/qt-5/qobject.html#no-copy-constructor-or-assignment-operator
    CtorDtorNotifierQt(const CtorDtorNotifierQt&)               = delete; // Copy constructor
    CtorDtorNotifierQt& operator=(const CtorDtorNotifierQt&)    = delete; // Copy assignment
    CtorDtorNotifierQt(CtorDtorNotifierQt&&)                    = delete; // Move constructor
    CtorDtorNotifierQt& operator=(CtorDtorNotifierQt&&)         = delete; // Move assignment

    inline int func() const { return 9; }
    uint* counter_ptr;
    uint id;

signals:
    void testSignal();
};

