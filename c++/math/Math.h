//  copyright daxiangyun, Inc. All Rights Reserved.

#pragma once

#include "TypeDefines.h"
#include <math.h>

namespace Math
{

#define EPSINON 0.000001f

#define PI 3.14159265358979f
#define TWOPI 6.28318530717958f
#define PI_2 1.57079632679489f
#define PI64 3.14159265358979
#define TWOPI64 6.28318530717958
#define PI64_2 1.57079632679489

#define DEG2RAD 0.017453293f
#define DEG2RAD64 0.017453293
#define RAD2DEG 57.295779513f
#define RAD2DEG64 57.295779513


inline bool Equalf(f32 x, f32 y, f32 epsinon = EPSINON)
{
    f32 z = x - y;
    return z > -epsinon && z < epsinon;
}

inline bool IsZero(f32 x)
{
    return Equalf(x, 0.0f);
}

inline f32 Clampf(f32 value, f32 min, f32 max)
{
    return value > max ? max : (value < min ? min : value);
}

inline bool DegressEqual(f32 a, f32 b, f32 epsinon = EPSINON)
{
    f32 c = a>b ?  (a - b) : (b-a);
    while (c >= 360.0f)
    {
        c -= 360.0f;
    }
    return c > -epsinon && c < epsinon;
}

// Convert degrees to radians
inline static float Radians(float degrees)
{
    return degrees * DEG2RAD;
}

// Convert radians to degrees
inline static float Degrees(float radians)
{
    return radians * RAD2DEG;
}

}// namespace Math