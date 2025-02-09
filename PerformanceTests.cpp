#include "PerformanceTests.h"

#include "Utils.h"

#include <vector>
#include <string>
#include <random>
#include <iostream>

void ifElseAssignment_vs_lambdaReturnIfElseAssignment()
{
    std::cout << "Executing " << __PRETTY_FUNCTION__ << std::endl;

    const std::vector<std::string> names {
        "Lina",
        "Gourry",
        "Zelgadis",
        "Sylphiel",
        "Amelia"
    };
    std::vector<uint64_t> res(names.size(), 0);
    auto seed = static_cast<std::mt19937_64::result_type>(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> dis{0, static_cast<int>(names.size() - 1)};
    const uint64_t N = pow(10, 7);

    // if-else
    {
        std::mt19937_64 gen{seed};
        LOG_DURATION("if-else assignment");
        for (uint64_t i = 0; i < N; ++i)
        {
            const int namesIdx = dis(gen);
            int typeIdx = 0;
            if (names[namesIdx] == names[0])
            {
                typeIdx = 0;
            }
            else if (names[namesIdx] == names[1])
            {
                typeIdx = 1;
            }
            else if (names[namesIdx] == names[2])
            {
                typeIdx = 2;
            }
            else if (names[namesIdx] == names[3])
            {
                typeIdx = 3;
            }
            else if (names[namesIdx] == names[4])
            {
                typeIdx = 4;
            }
            else
            {
                typeIdx = 0;
            }
            res[typeIdx] += 1;
        }
    }
    // lambda
    {
        std::mt19937_64 gen{seed};
        LOG_DURATION("lambda assignment");
        for (uint64_t i = 0; i < N; ++i)
        {
            const int namesIdx = dis(gen);
            int typeIdx = [&names, &namesIdx](){
                if (names[namesIdx] == names[0])
                {
                    return 0;
                }
                else if (names[namesIdx] == names[1])
                {
                    return 1;
                }
                else if (names[namesIdx] == names[2])
                {
                    return 2;
                }
                else if (names[namesIdx] == names[3])
                {
                    return 3;
                }
                else if (names[namesIdx] == names[4])
                {
                    return 4;
                }
                else
                {
                    return 0;
                }
            }();
            res[typeIdx] += 1;
        }
    }
    std::cout << "Finished " << __PRETTY_FUNCTION__ << std::endl;
    return;
}

void lambdaTypeDeduction()
{
    // no problems with type deduction or explicitly specified type as long as it's not recursive
    bool (*compare)(int, int) = [] (int i1, int i2) { return i1 * 2 > i2; };
    auto compare_auto = [] (int i1, int i2) { return i1 * 2 > i2; };
    (void)compare;
    (void)compare_auto;

    /*// does not work even though type is explicitly specified // due to lambda being operator() of anonymous struct
    // No viable conversion from 'lambda' to 'void (*)(int)'
    void (*f_1)(int) = [&f_1](int i) {
        ++i;
        if (i < 10)
            f_1(i);
    };
    f_1(5);

    // use of 'f_2' before deduction of 'auto'
    // Variable 'f_2' declared with deduced type 'auto' cannot appear in its own initializer
    // caused by capturting f_2 before deducing its type
    auto f_2 = [&f_2](int i) {
        ++i;
        if (i < 10)
            f_2(i);
    };
    f_2(5);

    // does not work even though "return;" should explicitly signify that return type is void
    auto f_3 = [](int i) {
        auto f_impl = [](int i, auto& f_impl) {
            ++i;
            if (i < 10)
                f_impl(i, f_impl);
            return;
        };
        f_impl(i, f_impl);
    };
    f_3(5);*/

    // works even though it's virtually the same as f_3. And it works with 'return 0;' even though it gives 'warning: control reaches end of non-void function [-Wreturn-type]'
    auto f_4 = [](int i) {
        auto f_impl = [](int i, auto& f_impl) {
            ++i;
            if (i > 10)
                return;
            f_impl(i, f_impl);
        };
        f_impl(i, f_impl);
    };
    f_4(5);

    // works - explicitly specifying return type in function signature does the job for auto type deduction
    auto f_5 = [](int i) {
        auto f_impl = [](int i, auto& f_impl) -> void {
            ++i;
            if (i < 10)
                f_impl(i, f_impl);
        };
        f_impl(i, f_impl);
    };
    f_5(5);
}

void memset_vs_manualAssign()
{
    std::cout << "Executing " << __PRETTY_FUNCTION__ << std::endl;

    struct PackStruct
    {
        int f0;
        int f1;
        int f2;
        int f3;
        int f4;
        int f5;
        int f6;
        int f7;
        int f8;
        int f9;
    };
    class PackTesterMemset
    {
    public:
        PackStruct m_data;
        PackTesterMemset()
        {
            ::memset(&m_data, 0, sizeof(m_data));
        }
    };
    class PackTesterAssign
    {
    public:
        PackStruct m_data;
        PackTesterAssign()
        {
            m_data.f0 = 0;
            m_data.f1 = 0;
            m_data.f2 = 0;
            m_data.f3 = 0;
            m_data.f4 = 0;
            m_data.f5 = 0;
            m_data.f6 = 0;
            m_data.f7 = 0;
            m_data.f8 = 0;
            m_data.f9 = 0;
        }
    };

    std::mt19937_64 gen{static_cast<std::mt19937_64::result_type>(std::chrono::system_clock::now().time_since_epoch().count())};
    std::uniform_int_distribution<> dis{0, 9};

#if DEBUG_BUILD
    const quint64 N = std::pow(10, 7);
#elif RELEASE_BUILD
    const quint64 N = std::pow(10, 9);
#endif
    std::vector<quint8>* disVec = new std::vector<quint8>;
    disVec->reserve(N);
    for (quint64 i = 0; i < N; ++i)
        disVec->push_back(dis(gen));
    std::cout << "allocated disVec" << std::endl;

    quint64 val = 0;
    {
        LOG_DURATION("memset");
        for (quint64 i = 0; i < N; ++i)
        {
            PackTesterMemset t;
            switch (disVec->at(i))
            {
            case 0: { val += t.m_data.f0; break; }
            case 1: { val += t.m_data.f1; break; }
            case 2: { val += t.m_data.f2; break; }
            case 3: { val += t.m_data.f3; break; }
            case 4: { val += t.m_data.f4; break; }
            case 5: { val += t.m_data.f5; break; }
            case 6: { val += t.m_data.f6; break; }
            case 7: { val += t.m_data.f7; break; }
            case 8: { val += t.m_data.f8; break; }
            case 9: { val += t.m_data.f9; break; }
            }
        }
    }
    {
        LOG_DURATION("manual assign");
        for (quint64 i = 0; i < N; ++i)
        {
            PackTesterAssign t;
            switch (disVec->at(i))
            {
            case 0: { val += t.m_data.f0; break; }
            case 1: { val += t.m_data.f1; break; }
            case 2: { val += t.m_data.f2; break; }
            case 3: { val += t.m_data.f3; break; }
            case 4: { val += t.m_data.f4; break; }
            case 5: { val += t.m_data.f5; break; }
            case 6: { val += t.m_data.f6; break; }
            case 7: { val += t.m_data.f7; break; }
            case 8: { val += t.m_data.f8; break; }
            case 9: { val += t.m_data.f9; break; }
            }
        }
    }
    std::cout << "finished " << __PRETTY_FUNCTION__ << " anchor val=" << val << std::endl;
}

void lambdaPerformanceTest()
{
    quint64 curIdxEnd = 0;
    auto advanceBlockV1 = [&](quint8 bitComp, ushort dataLen){
        (void)bitComp;
        curIdxEnd += dataLen;
    };
    auto advanceBlockV2 = [&](ushort dataLen){
        curIdxEnd += dataLen;
    };

    std::mt19937_64 gen{static_cast<std::mt19937_64::result_type>(std::chrono::system_clock::now().time_since_epoch().count())};
    std::uniform_int_distribution<> dis{0, 5};
    const int N = std::pow(10, 8);
    std::array<int, N>* heapArr = new std::array<int, N>();
    std::array<int, N>& values = *heapArr;
    for (int i = 0; i < N; ++i)
    {
        values[i] = dis(gen);
    }

    curIdxEnd = 0;
    // lambda with unused variable
    // Marginally slower in debug than V2 since it instantiates bitComp variable even though it's unused. In release mode unused var is optimized out, making it the same as V2
    {
        LOG_DURATION("advanceBlockV1");
        for (int i = 0; i < N; ++i)
            advanceBlockV1(0x80, values[i]);
    }
    std::cout << "curIdxEnd = " << curIdxEnd << std::endl;

    curIdxEnd = 0;
    // lambda without unused variable
    {
        LOG_DURATION("advanceBlockV2");
        for (int i = 0; i < N; ++i)
            advanceBlockV2(values[i]);
    }
    std::cout << "curIdxEnd = " << curIdxEnd << std::endl;

    curIdxEnd = 0;
    // lambda with unused variable, but lambda is instantiated in each loop iteration
    // Notably slower than V1 and V2 since lambda instance is created in each loop iteration
    {
        LOG_DURATION("advanceBlockV3");
        for (int i = 0; i < N; ++i)
        {
            auto advanceBlockV3 = [&](quint8 bitComp, ushort dataLen){
                (void)bitComp;
                curIdxEnd += dataLen;
            };
            advanceBlockV3(0x80, values[i]);
        }
    }
    std::cout << "curIdxEnd = " << curIdxEnd << std::endl;

    curIdxEnd = 0;
    // Optimized - lambda call is replaced with its contents, basically like inlining it
    // Marginally faster than V1 and V2, since invoking lambda incurs overhead. Though shouldn't lambda calls get optimized-inlined resulting in code equivalent to this?
    {
        LOG_DURATION("\'clean\' code");
        for (int i = 0; i < N; ++i)
            curIdxEnd += values[i];
    }
    std::cout << "curIdxEnd = " << curIdxEnd << std::endl;
}
