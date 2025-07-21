// copyright daxiangyun, Inc. All Rights Reserved.

#include <iostream>
#include <string>

#include <emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;

#include "ParameterInterface.h"

PersonRecord* g_person;

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void Init()
{
    g_person       = new PersonRecord();
    g_person->name = "sme03";
    g_person->age  = 28;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void Print()
{
    std::cout << "name: " << g_person->name << " age: " << g_person->age << std::endl;
}

// int
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void TestInt(int a)
{
    std::cout << "C++ TestInt get: " << a << std::endl;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void TestInt2(int a, int b)
{
    std::cout << "C++ TestInt2 get: " << a << " " << b << std::endl;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void TestInt3(int a, int b, int c)
{
    std::cout << "C++ TestInt3 get: " << a << " " << b << " " << c << std::endl;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void TestInt4(unsigned int a, unsigned int b, unsigned int c)
{
    std::cout << "C++ TestInt4 get: " << a << " " << b << " " << c << std::endl;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int TestReturnInt(int a, int b)
{
    auto c = a + b;

    std::cout << "C++ TestReturnInt return: " << c << std::endl;
    return c;
}

// float
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void TestFloat(float a)
{
    std::cout << "C++ TestFloat get: " << a << std::endl;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void TestFloat2(float a, float b)
{
    std::cout << "C++ TestFloat2 get: " << a << " " << b << std::endl;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void TestFloat3(float a, float b, float c)
{
    std::cout << "C++ TestFloat3 get: " << a << " " << b << " " << c << std::endl;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
float TestReturnFloat(float a, float b)
{
    auto c = a + b;

    std::cout << "C++ TestReturnFloat return: " << c << std::endl;
    return c;
}

// int, float
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void TestIntFloat(int a, float b)
{
    std::cout << "C++ TestIntFloat get: " << a << " " << b << std::endl;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void TestIntFloat2(int a, float b, float c)
{
    std::cout << "C++ TestIntFloat2 get: " << a << " " << b << " " << c << std::endl;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void TestIntFloat3(int a, float b, float c, float d)
{
    std::cout << "C++ TestIntFloat3 get: " << a << " " << b << " " << c << " " << d << std::endl;
}

// string
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void TestString(const char* a)
{
    std::cout << "C++ TestString get: " << a << std::endl;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void TestString2(const char* a, const char* b)
{
    std::cout << "C++ TestString2 get: " << a << " " << b << std::endl;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void TestString3(const char* a, const char* b, const char* c)
{
    std::cout << "C++ TestString3 get: " << a << " " << b << " " << c << std::endl;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
const char* TestReturnString(const char* a, const char* b)
{
    // std::string result = std::string(a) + " " + std::string(b);

    std::cout << "C++ TestReturnString return: " << "Hello String" << std::endl;
    return "Hello String";
}

// const char* TestString4(const std::string b)
// {
//     std::cout << "C++ TestString4 get: " << b << std::endl;

//     return "2222";
// }

void TestArray(const ArrayInStruct& a)
{
    std::cout << "C++ TestArray: " << a.field[0] << " " << a.field[1] << " " << a.field[2] << std::endl;
}

void TestArray2(std::array<int, 3>& a)
{
    std::cout << "C++ TestArray: " << a[0] << " " << a[1] << " " << a[2] << std::endl;
}

void TestVector(std::vector<int>& a)
{
    std::cout << "C++ TestVector: " << a[0] << " " << a[1] << " " << a[2] << std::endl;
}

// void TestArray3(int a[3])
// {
//     std::cout << "C++ TestArray: " << a[0] << " " << a[1] << " " << a[2] << std::endl;
// }

// struct
void TestStruct(const MyStruct& st)
{
    std::cout << "C++ TestStruct get: " << st.x << " " << st.y << " " << st.name << std::endl;
}

void TestStruct2(const MyStruct& st, const MyStruct& st2)
{
    std::cout << "C++ TestStruct2 get: " << st.x << " " << st.y << " " << st.name << " -- " << st2.x << " " << st2.y << " " << st2.name << std::endl;
}

PersonRecord FindPersonAtLocation(Point2f point)
{
    std::cout << "C++ FindPersonAtLocation: " << point.x << " " << point.y << std::endl;

    return PersonRecord{"John", 42};
}

// Binding code
EMSCRIPTEN_BINDINGS(my_module)
{
    value_object<MyStruct>("MyStruct")
        .field("x", &MyStruct::x)
        .field("y", &MyStruct::y)
        .field("name", &MyStruct::name);

    value_array<Point2f>("Point2f")
        .element(&Point2f::x)
        .element(&Point2f::y);

    value_object<PersonRecord>("PersonRecord")
        .field("name", &PersonRecord::name)
        .field("age", &PersonRecord::age);

    value_object<ArrayInStruct>("ArrayInStruct")
        .field("field", &ArrayInStruct::field); // Need to register the array type

    // Register std::array<int, 2> because ArrayInStruct::field is interpreted as such
    value_array<std::array<int, 3>>("array3")
        .element(emscripten::index<0>())
        .element(emscripten::index<1>())
        .element(emscripten::index<2>());

    register_vector<int>("VectorInt");

    function("Init", &Init);
    function("Print", &Print);

    function("lerp", &lerp);
    function("TestArray", &TestArray);
    function("TestArray2", &TestArray2);
    // function("TestArray3", &TestArray3);
    function("TestVector", &TestVector);
    // function("TestString4", &TestString4, return_value_policy::take_ownership());

    function("TestStruct", &TestStruct);
    function("TestStruct2", &TestStruct2);
    function("FindPersonAtLocation", &FindPersonAtLocation);
}
