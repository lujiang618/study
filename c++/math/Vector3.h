#pragma once

#include "TypeDefines.h"

class Matrix3;
class Matrix4;
class Vector2;

class Vector4;

class Vector3
{
public:
    f32 x, y, z;
    Vector3();
    Vector3(f32 x);
    Vector3(f32 x, f32 y, f32 z);
    Vector3(const Vector2& v, f32 z = 0.0f);
    Vector3(const Vector3& v);
    Vector3(const Vector4& v);
    ~Vector3();

    void Normalize();
    Vector3 Normalize() const;

    Vector3& Set(float x, float y, float z);
    Vector3& Copy(const Vector3& v);
    Vector3 Clone() const;


    Vector3& Min(const Vector3& v);
    Vector3& Max(const Vector3& v);
    Vector3& Clamp(const Vector3& min, const Vector3& max);

    Vector3& ApplyMatrix3(const Matrix3&);
    Vector3& ApplyMatrix4(const Matrix4&);

    f32 Length() const;
    f32 Dot(const Vector3& v) const;
    Vector3 Cross(const Vector3& v) const;

    f32 Distance(const Vector3& v) const;
    Vector3 Middle(const Vector3& v) const;
    Vector3 Lerp(const Vector3& v, f32 a) const;

    const Vector3& operator +=(const Vector3& v);
    const Vector3& operator +=(f32 a);
    const Vector3& operator -=(const Vector3& v);
    const Vector3& operator -=(f32 a);
    const Vector3& operator *=(const Vector3& v);

    const Vector3& operator *=(f32 a);
    const Vector3& operator /=(const Vector3& v);
    const Vector3& operator /=(f32 a);

    bool operator ==(const Vector3& v) const;
    bool operator !=(const Vector3& v) const;

    static const Vector3 Zero;
    static const Vector3 One;
    static const Vector3 X;
    static const Vector3 Y;
    static const Vector3 Z;
};

extern Vector3 operator +(const Vector3& v1, const Vector3& v2);
extern Vector3 operator +(const Vector3& v, f32 a);
extern Vector3 operator +(f32 a, const Vector3& v);
extern Vector3 operator -(const Vector3& v1, const Vector3& v2);
extern Vector3 operator -(const Vector3& v, f32 a);
extern Vector3 operator -(f32 a, const Vector3& v);
extern Vector3 operator *(const Vector3& v1, const Vector3& v2);
extern Vector3 operator *(const Vector3& v, f32 a);
extern Vector3 operator *(f32 a, const Vector3& v);
extern Vector3 operator /(const Vector3& v1, const Vector3& v2);
extern Vector3 operator /(const Vector3& v, f32 a);
extern Vector3 operator /(f32 a, const Vector3& v);
