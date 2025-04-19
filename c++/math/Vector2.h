//  copyright daxiangyun, Inc. All Rights Reserved.

#pragma once

#include "TypeDefines.h"

class Vector2
{
public:
    f32 x, y;
    Vector2();
    Vector2(f32 x);
    Vector2(f32 x, f32 y);
    Vector2(const Vector2& v);
    ~Vector2();

    const Vector2& Normalize();

    f32 Length() const;
    f32 Dot(const Vector2& v) const;
    f32 Cross(const Vector2& v) const;

    f32 Distance(const Vector2& v) const;
    Vector2 Middle(const Vector2& v) const;
    Vector2 Lerp(const Vector2& v, f32 a) const;

    Vector2 Min(const Vector2& v);
    Vector2 Max(const Vector2& v);

    const Vector2& operator +=(const Vector2& v);
    const Vector2& operator +=(f32 a);
    const Vector2& operator -=(const Vector2& v);
    const Vector2& operator -=(f32 a);
    const Vector2& operator *=(const Vector2& v);
    const Vector2& operator *=(f32 a);
    const Vector2& operator /=(const Vector2& v);
    const Vector2& operator /=(f32 a);

    bool operator ==(const Vector2& v) const;
    bool operator !=(const Vector2& v) const;

    static const Vector2 Zero;
    static const Vector2 One;
    static const Vector2 X;
    static const Vector2 Y;
};

extern Vector2 operator +(const Vector2& v1, const Vector2& v2);
extern Vector2 operator +(const Vector2& v, f32 a);
extern Vector2 operator +(f32 a, const Vector2& v);
extern Vector2 operator -(const Vector2& v1, const Vector2& v2);
extern Vector2 operator -(const Vector2& v, f32 a);
extern Vector2 operator -(f32 a, const Vector2& v);
extern Vector2 operator *(const Vector2& v1, const Vector2& v2);
extern Vector2 operator *(const Vector2& v, f32 a);
extern Vector2 operator *(f32 a, const Vector2& v);
extern Vector2 operator /(const Vector2& v1, const Vector2& v2);
extern Vector2 operator /(const Vector2& v, f32 a);
extern Vector2 operator /(f32 a, const Vector2& v);

