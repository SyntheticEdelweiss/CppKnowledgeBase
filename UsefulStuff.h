#pragma once


#include <type_traits>

#ifdef QDATASTREAM_H
#define DEFINE_ENUM_CLASS_OPERATORS_IMPL_QDATASTREAM(EnumName) \
inline QDataStream& operator<<(QDataStream& stream, const EnumName& data)\
{\
    stream << reinterpret_cast<std::underlying_type<EnumName>::type const&>(data);\
    return stream;\
}\
inline QDataStream& operator>>(QDataStream& stream, EnumName& data)\
{\
    stream >> reinterpret_cast<std::underlying_type<EnumName>::type &>(data);\
    return stream;\
}
#else
#define DEFINE_ENUM_CLASS_OPERATORS_IMPL_QDATASTREAM(EnumName)
#endif

#define DEFINE_ENUM_CLASS_OPERATORS_IMPL_LOGICAL(EnumName) \
inline constexpr EnumName operator&(const EnumName& lhs, const EnumName& rhs)\
{\
    std::underlying_type_t<EnumName> ret_t = static_cast<std::underlying_type_t<EnumName>>(lhs)\
          & static_cast<std::underlying_type_t<EnumName>>(rhs);\
    return static_cast<EnumName>(ret_t);\
}\
template<typename T>\
constexpr inline EnumName operator&(const EnumName& lhs, const T& rhs)\
{\
    if constexpr(std::is_enum<T>::value == true)\
    {\
        static_assert(std::is_same<std::underlying_type_t<EnumName>, std::underlying_type_t<T>>::value == true,\
                      "operator&(EnumName, T) cannot be used with enum T of different underlying type");\
        std::underlying_type_t<EnumName> ret_t = static_cast<std::underlying_type_t<EnumName>>(lhs)\
              & static_cast<std::underlying_type_t<T>>(rhs);\
        return static_cast<EnumName>(ret_t);\
    }\
    else\
    {\
        static_assert(std::is_same<std::underlying_type_t<EnumName>, T>::value == true,\
                      "operator&(EnumName, T) cannot be used with type T which is different from EnumName underlying type");\
        std::underlying_type_t<EnumName> ret_t = static_cast<std::underlying_type_t<EnumName>>(lhs)\
              & rhs;\
        return static_cast<EnumName>(ret_t);\
    }\
}\
template<typename T>\
constexpr inline EnumName operator&(const T& lhs, const EnumName& rhs)\
{\
    if constexpr(std::is_enum<T>::value == true)\
    {\
        static_assert(std::is_same<std::underlying_type_t<EnumName>, std::underlying_type_t<T>>::value == true,\
                      "operator&(T, EnumName) cannot be used with enum T of different underlying type");\
        std::underlying_type_t<EnumName> ret_t = static_cast<std::underlying_type_t<T>>(lhs)\
              & static_cast<std::underlying_type_t<EnumName>>(rhs);\
        return static_cast<EnumName>(ret_t);\
    }\
    else\
    {\
        static_assert(std::is_same<std::underlying_type_t<EnumName>, T>::value == true,\
                      "operator&(T, EnumName) cannot be used with type T which is different from EnumName underlying type");\
        std::underlying_type_t<EnumName> ret_t = lhs\
              & static_cast<std::underlying_type_t<EnumName>>(rhs);\
        return static_cast<EnumName>(ret_t);\
    }\
}\
constexpr inline EnumName operator|(const EnumName& lhs, const EnumName& rhs)\
{\
    std::underlying_type_t<EnumName> ret_t = static_cast<std::underlying_type_t<EnumName>>(lhs)\
          | static_cast<std::underlying_type_t<EnumName>>(rhs);\
    return static_cast<EnumName>(ret_t);\
}\
template<typename T>\
constexpr inline EnumName operator|(const EnumName& lhs, const T& rhs)\
{\
    if constexpr(std::is_enum<T>::value == true)\
    {\
        static_assert(std::is_same<std::underlying_type_t<EnumName>, std::underlying_type_t<T>>::value == true,\
                      "operator|(EnumName, T) cannot be used with enum T of different underlying type");\
        std::underlying_type_t<EnumName> ret_t = static_cast<std::underlying_type_t<EnumName>>(lhs)\
              | static_cast<std::underlying_type_t<T>>(rhs);\
        return static_cast<EnumName>(ret_t);\
    }\
    else\
    {\
        static_assert(std::is_same<std::underlying_type_t<EnumName>, T>::value == true,\
                      "operator|(EnumName, T) cannot be used with type T which is different from EnumName underlying type");\
        std::underlying_type_t<EnumName> ret_t = static_cast<std::underlying_type_t<EnumName>>(lhs)\
              | rhs;\
        return static_cast<EnumName>(ret_t);\
    }\
}\
template<typename T>\
constexpr inline EnumName operator|(const T& lhs, const EnumName& rhs)\
{\
    if constexpr(std::is_enum<T>::value == true)\
    {\
        static_assert(std::is_same<std::underlying_type_t<EnumName>, std::underlying_type_t<T>>::value == true,\
                      "operator|(T, EnumName) cannot be used with enum T of different underlying type");\
        std::underlying_type_t<EnumName> ret_t = static_cast<std::underlying_type_t<T>>(lhs)\
              | static_cast<std::underlying_type_t<EnumName>>(rhs);\
        return static_cast<EnumName>(ret_t);\
    }\
    else\
    {\
        static_assert(std::is_same<std::underlying_type_t<EnumName>, T>::value == true,\
                      "operator|(T, EnumName) cannot be used with type T which is different from EnumName underlying type");\
        std::underlying_type_t<EnumName> ret_t = lhs\
              | static_cast<std::underlying_type_t<EnumName>>(rhs);\
        return static_cast<EnumName>(ret_t);\
    }\
}

// NOTE: if you intend to make "iteratable" enum class, BEGIN_VALUE and END_VALUE have to be assigned for proper wrapping
#define DEFINE_ENUM_CLASS_OPERATORS_IMPL_ARITHMETIC_UNARY(EnumName) \
inline constexpr EnumName& operator++(EnumName& var)\
{\
    if (var == EnumName::END_VALUE)\
    var = EnumName::BEGIN_VALUE;\
    else\
    var = static_cast<EnumName>(static_cast<std::underlying_type_t<EnumName>>(var) + 1);\
    return var;\
}\
constexpr inline EnumName& operator--(EnumName& var)\
{\
    if (var == EnumName::BEGIN_VALUE)\
    var = EnumName::END_VALUE;\
    else\
    var = static_cast<EnumName>(static_cast<std::underlying_type_t<EnumName>>(var) - 1);\
    return var;\
}

#define DEFINE_ENUM_CLASS_OPERATORS(EnumName) \
DEFINE_ENUM_CLASS_OPERATORS_IMPL_QDATASTREAM(EnumName)\
DEFINE_ENUM_CLASS_OPERATORS_IMPL_LOGICAL(EnumName)\
DEFINE_ENUM_CLASS_OPERATORS_IMPL_ARITHMETIC_UNARY(EnumName)


#include <QtCore/QByteArray>
#include <QtCore/QString>
#include <QtGlobal>

inline QString getRawQStringFromQByteArray(QByteArray& arr)
{
    QString str(arr.size(), QChar(0x00));
    char* p_str = reinterpret_cast<char*>(str.data());
    char* p_arr = arr.data();
#if (Q_BYTE_ORDER == Q_BIG_ENDIAN)
    ++p_str;
#endif
    char* end = arr.end();
    while (p_arr != end)
    {
        *p_str = *p_arr;
        ++p_arr;
        ++++p_str;
    }
    return str;
}


#include <type_traits>
#include <QtDebug>

// Simple analog of std::concepts to enforce some requirements on derived classes
template <class T>
class BaseConceptAnalog
{
public:
    // Option 1: Base must be a template class, with type T being Derived class, and Derived classes specify T when declaring inheritance, i.e. "Derived : public Base<Derived>"
    BaseConceptAnalog()
    {
        static_assert(std::is_base_of<BaseConceptAnalog, T>::value == true, "Template argument in DerivedConceptAnalog class declaration of inheritance must be DerivedConceptAnalog class");
        static_assert(std::is_member_function_pointer<decltype(&T::requiredNonVirtual)>::value, "specified function is required in DerivedConceptAnalog class");
    }
    /*// Option 2: Base is non-template class with template constructor, with type C being Derived class, and Derived classes specify C in their constructor, i.e. "Derived() : Base(this) {}"
    // Downsides: extra code for each constructor overload; Base either needs to be abstract, or specify some non-Derived class and default value which might be used incorrectly by Derived class, i.e. "Derived() : Base() {}"
    template <class C>
    BaseConceptAnalog(C*)
    {
        static_assert(std::is_BaseConceptAnalog_of<BaseConceptAnalog, C>::value == true, "Template argument in DerivedConceptAnalog class declaration of inheritance must be DerivedConceptAnalog class");
        static_assert(std::is_member_function_pointer<decltype(&C::requiredNonVirtual)>::value, "specified function is required in DerivedConceptAnalog class");
    }*/
    virtual ~BaseConceptAnalog() = default;
    virtual void requiredVirtual() = 0;
    virtual void optionalVirtual() { qWarning() << "BaseConceptAnalog::optionalVirtual()"; }
    void requiredNonVirtual() { qWarning() << "BaseConceptAnalog::requiredNonVirtual"; }
};
class DerivedConceptAnalog : public BaseConceptAnalog<DerivedConceptAnalog>
{
public:
    //DerivedConceptAnalog() : BaseConceptAnalog(this) {} // Option 2
    virtual void requiredVirtual() final { qWarning() << "DerivedConceptAnalog::requiredVirtual()"; }
    //virtual void optionalVirtual() final { qWarning() << "DerivedConceptAnalog::optionalVirtual()"; }
    void requiredNonVirtual() { qWarning() << "DerivedConceptAnalog::requiredNonVirtual()"; }
};


#define DEFINE_HAS_FUNCTION_NAME_TEMPLATE(V_functionName) \
template <typename T>\
class Has_##V_functionName\
{\
    private:\
        typedef char YesType[1];\
    typedef char NoType[2];\
    template <typename C> static YesType& test(decltype(&C::V_functionName));\
    template <typename C> static NoType& test(...);\
    public:\
        enum { value = sizeof(test<T>(0)) == sizeof(YesType) };\
};\
template<typename T>\
inline constexpr bool Has_##V_functionName##_v = Has_##V_functionName<T>::value;

DEFINE_HAS_FUNCTION_NAME_TEMPLATE(toString)

/*// example Has_funcName
template<typename T,
         std::enable_if_t<Has_toString_v<T>, bool> = true>
QString CallToString(const T& t)
{
    return t.toString();
}*/
