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

#ifdef __EMSCRIPTEN__


    struct MyStruct
    {
        int x = 5;
        float y = 10.0;
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
        int age = 0;
    };

    // Array fields are treated as if they were std::array<type,size>
    struct ArrayInStruct 
    {
        int field[2];
    };

    float lerp(float a, float b, float t) 
    {
        return (1 - t) * a + t * b;
    }




    // int
    EMSCRIPTEN_KEEPALIVE
    void TestInt(int a)
    {
        std::cout << "C++ TestInt get: " << a << std::endl;
    }

    EMSCRIPTEN_KEEPALIVE
    void TestInt2(int a, int b)
    {
        std::cout << "C++ TestInt2 get: " << a << " " << b << std::endl;
    }

    EMSCRIPTEN_KEEPALIVE
    void TestInt3(int a, int b, int c)
    {
        std::cout << "C++ TestInt3 get: " << a << " " << b << " " << c << std::endl;
    }

    EMSCRIPTEN_KEEPALIVE
    void TestInt4(unsigned int a, unsigned int b, unsigned int c)
    {
        std::cout << "C++ TestInt4 get: " << a << " " << b << " " << c << std::endl;
    }

    EMSCRIPTEN_KEEPALIVE
    int TestReturnInt(int a, int b)
    {
        auto c = a + b;

        std::cout << "C++ TestReturnInt return: " << c << std::endl;
        return c;
    }


    // float
    EMSCRIPTEN_KEEPALIVE
    void TestFloat(float a)
    {
        std::cout << "C++ TestFloat get: " << a << std::endl;
    }

    EMSCRIPTEN_KEEPALIVE
    void TestFloat2(float a, float b)
    {
        std::cout << "C++ TestFloat2 get: " << a << " " << b << std::endl;
    }

    EMSCRIPTEN_KEEPALIVE
    void TestFloat3(float a, float b, float c)
    {
        std::cout << "C++ TestFloat3 get: " << a << " " << b << " " << c << std::endl;
    }

    EMSCRIPTEN_KEEPALIVE
    float TestReturnFloat(float a, float b)
    {
        auto c = a + b;

        std::cout << "C++ TestReturnFloat return: " << c << std::endl;
        return c;
    }


    // int, float
    EMSCRIPTEN_KEEPALIVE
    void TestIntFloat(int a, float b)
    {
        std::cout << "C++ TestIntFloat get: " << a << " " << b << std::endl;
    }

    EMSCRIPTEN_KEEPALIVE
    void TestIntFloat2(int a, float b, float c)
    {
        std::cout << "C++ TestIntFloat2 get: " << a << " " << b << " " << c << std::endl;
    }

    EMSCRIPTEN_KEEPALIVE
    void TestIntFloat3(int a, float b, float c, float d)
    {
        std::cout << "C++ TestIntFloat3 get: " << a << " " << b << " " << c << " " << d << std::endl;
    }


    // string
    EMSCRIPTEN_KEEPALIVE
    void TestString(const char* a)
    {
        std::cout << "C++ TestString get: " << a << std::endl;
    }

    EMSCRIPTEN_KEEPALIVE
    void TestString2(const char* a, const char* b)
    {
        std::cout << "C++ TestString2 get: " << a << " " << b << std::endl;
    }

    EMSCRIPTEN_KEEPALIVE
    void TestString3(const char* a, const char* b, const char* c)
    {
        std::cout << "C++ TestString3 get: " << a << " " << b << " " << c << std::endl;
    }

    EMSCRIPTEN_KEEPALIVE
    const char* TestReturnString(const char* a, const char* b)
    {
        // std::string result = std::string(a) + " " + std::string(b);

        std::cout << "C++ TestReturnString return: " << "Hello String" << std::endl;
        return "Hello String";
    }


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


        function("lerp", &lerp);

        function("TestStruct", &TestStruct);
        function("TestStruct2", &TestStruct2);
        function("FindPersonAtLocation", &FindPersonAtLocation);
    }

#endif

#if defined(__cplusplus)
}
#endif








