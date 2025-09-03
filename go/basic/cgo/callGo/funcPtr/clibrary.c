#include <stdio.h>

#include "clibrary.h"

void ON(callback_fcn callback)
{
    int arg = 2;
    printf("before: arg = %d\n", arg);
    int response = callback(2);
    printf("after: responded %d\n", response);
}