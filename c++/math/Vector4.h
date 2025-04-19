//  copyright daxiangyun, Inc. All Rights Reserved.

#pragma once

#include "TypeDefines.h"

class Vector2;
class Vector3;

class Vector4
{
public:
    f32 x, y, z, w;
    Vector4();
    Vector4(f32 x);
    Vector4(f32 x, f32 y, f32 z, f32 w);
    Vector4(const Vector2& v,f32 z = 0.0f, f32 w = 0.0f);
    Vector4(const Vector3& v, f32 w = 0.0f);
    Vector4(const Vector4& v);
    ~Vector4();

    const Vector4& Normalize();

    f32 Length() const;
    f32 Dot(const Vector4& v) const;

    f32 Distance(const Vector4& v) const;
    Vector4 Middle(const Vector4& v) const;
    Vector4 Lerp(const Vector4& v, f32 a) const;

    const Vector4& operator +=(const Vector4& v);
    const Vector4& operator +=(f32 a);
    const Vector4& operator -=(const Vector4& v);
    const Vector4& operator -=(f32 a);
    const Vector4& operator *=(const Vector4& v);
    const Vector4& operator *=(f32 a);
    const Vector4& operator /=(const Vector4& v);
    const Vector4& operator /=(f32 a);

    bool operator ==(const Vector4& v) const;
    bool operator !=(const Vector4& v) const;

    static const Vector4 Zero;
    static const Vector4 One;
    static const Vector4 X;
    static const Vector4 Y;
    static const Vector4 Z;
    static const Vector4 W;
};

extern Vector4 operator +(const Vector4& v1, const Vector4& v2);
extern Vector4 operator +(const Vector4& v, f32 a);
extern Vector4 operator +(f32 a, const Vector4& v);
extern Vector4 operator -(const Vector4& v1, const Vector4& v2);
extern Vector4 operator -(const Vector4& v, f32 a);
extern Vector4 operator -(f32 a, const Vector4& v);
extern Vector4 operator *(const Vector4& v1, const Vector4& v2);
extern Vector4 operator *(const Vector4& v, f32 a);
extern Vector4 operator *(f32 a, const Vector4& v);
extern Vector4 operator /(const Vector4& v1, const Vector4& v2);
extern Vector4 operator /(const Vector4& v, f32 a);
extern Vector4 operator /(f32 a, const Vector4& v);
