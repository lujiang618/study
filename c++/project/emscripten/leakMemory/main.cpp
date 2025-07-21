
#include <stdio.h>

const char* __asan_default_options()
{
    return "detect_stack_use_after_return=1";
}

int* f()
{
    int buf[10];
    return buf;
}

int main()
{
    *f() = 1;
}