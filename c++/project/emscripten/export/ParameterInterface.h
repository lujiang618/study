// copyright daxiangyun, Inc. All Rights Reserved.

#include <iostream>
#include <string>

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#include <emscripten/bind.h>

using namespace emscripten;
#endif

#if defined(__cplusplus)
extern "C"
{
#endif

    struct MyStruct
    {
        int         x    = 5;
        float       y    = 10.0;
        std::string name = "hello";
    };

    struct Point2f
    {
        float x = 1.0f;
        float y = 2.0f;
    };

    struct PersonRecord
    {
        std::string name = "";
        int         age  = 0;
    };

    // Array fields are treated as if they were std::array<type,size>
    struct ArrayInStruct
    {
        std::array<int, 3> field;
    };

    float lerp(float a, float b, float t)
    {
        return (1 - t) * a + t * b;
    }

    void Init();

    void Print();

    // int
    void TestInt(int a);

    void TestInt2(int a, int b);

    void TestInt3(int a, int b, int c);

    void TestInt4(unsigned int a, unsigned int b, unsigned int c);

    int TestReturnInt(int a, int b);

    // float
    void TestFloat(float a);

    void TestFloat2(float a, float b);

    void TestFloat3(float a, float b, float c);

    float TestReturnFloat(float a, float b);

    // int, float
    void TestIntFloat(int a, float b);
    void TestIntFloat2(int a, float b, float c);

    void TestIntFloat3(int a, float b, float c, float d);

    // string
    void TestString(const char* a);

    void TestString2(const char* a, const char* b);

    void TestString3(const char* a, const char* b, const char* c);

    // const char* TestString4(const std::string b);

    const char* TestReturnString(const char* a, const char* b);

    void TestArray(const ArrayInStruct& a);

    void TestArray2(std::array<int, 3>& a);

    void TestVector(std::vector<int>& a);

    // void TestArray3(int a[3]);

    // struct
    void TestStruct(const MyStruct& st);

    void TestStruct2(const MyStruct& st, const MyStruct& st2);

    PersonRecord FindPersonAtLocation(Point2f point);

#if defined(__cplusplus)
}
#endif
