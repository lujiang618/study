//  copyright daxiangyun, Inc. All Rights Reserved.

#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include <math.h>


#include "Math.h"

const Vector4 Vector4::Zero = Vector4(0.0f);
const Vector4 Vector4::One = Vector4(1.0f);
const Vector4 Vector4::X = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::Y = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
const Vector4 Vector4::Z = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
const Vector4 Vector4::W = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

Vector4::Vector4()
    : Vector4(0.0f, 0.0f, 0.0f, 0.0f)
{
}

Vector4::Vector4(f32 x)
    : Vector4(x, x, x, x)
{
}

Vector4::Vector4(f32 x, f32 y, f32 z, f32 w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vector4::Vector4(const Vector2& v, f32 z /*= 0.0f*/, f32 w /*= 0.0f*/)
: Vector4(v.x, v.y, z, w)
{
}

Vector4::Vector4(const Vector3& v, f32 w /*= 0.0f*/)
: Vector4(v.x, v.y, v.z, w)
{
}

Vector4::Vector4(const Vector4& v)
    : Vector4(v.x, v.y, v.z, v.w)
{
}

Vector4::~Vector4()
{
}

const Vector4& Vector4::Normalize()
{
    const f32 len = this->Length();
    if(Math::IsZero(len) == false)
    {
        x /= len; y /=len; z /= len; w /= len;
    }
    return *this;
}

f32 Vector4::Length() const
{
    return sqrtf(x*x + y*y + z*z + w*w);
}

f32 Vector4::Dot(const Vector4& v) const
{
    return x*v.x + y * v.y + z*v.z + w*v.w;
}


f32 Vector4::Distance(const Vector4& v) const
{
    const f32 dx = x - v.x;
    const f32 dy = y - v.y;
    const f32 dz = z - v.z;
    const f32 dw = w - v.w;
    return sqrtf(dx * dx + dy *dy + dz*dz + dw*dw);
}

Vector4 Vector4::Middle(const Vector4& v) const
{
    return Vector4(0.5f*(x+v.x), 0.5f*(y+v.y), 0.5f*(z+v.z), 0.5f*(w+v.w));
}

Vector4 Vector4::Lerp(const Vector4& v, f32 a) const
{
    const f32 invLerp = 1.0f - a;
    return Vector4(x*a + v.x*invLerp, y*a + v.y*invLerp, z*a + v.z*invLerp, w*a + v.w*invLerp);
}

const Vector4& Vector4::operator +=(const Vector4& v)
{
    x += v.x; y += v.y; z += v.z; w += v.w;
    return *this;
}

const Vector4& Vector4::operator +=(f32 a)
{
    x += a; y += a;  z += a; w += a;
    return *this;
}

const Vector4& Vector4::operator -=(const Vector4& v)
{
    x -= v.x; y -= v.y;  z -= v.z; w -= v.w;
    return *this;
}

const Vector4& Vector4::operator -=(f32 a)
{
    x -= a; y -= a;  z -= a; w -= a;
    return *this;
}

const Vector4& Vector4::operator *=(const Vector4& v)
{
    x *= v.x; y *= v.y;  z *= v.z; w *= v.w;
    return *this;
}

const Vector4& Vector4::operator *=(f32 a)
{
    x *= a; y *= a;  z *= a; w *= a;
    return *this;
}

const Vector4& Vector4::operator /=(const Vector4& v)
{
    x /= v.x; y /= v.y;  z /= v.z; w /= v.w;
    return *this;
}

const Vector4& Vector4::operator /=(f32 a)
{
    x /= a; y /= a;  z /= a; w /= a;
    return *this;
}

bool Vector4::operator ==(const Vector4& v) const
{
    return Math::Equalf(v.x, this->x) && Math::Equalf(v.y, this->y) && Math::Equalf(v.z, this->z) && Math::Equalf(v.w, this->w);
}

bool Vector4::operator !=(const Vector4& v) const
{
    return !(v == *this);
}

Vector4 operator +(const Vector4& v1, const Vector4& v2)
{
    return Vector4(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z, v1.w+v2.w);
}

Vector4 operator +(const Vector4& v, f32 a)
{
    return Vector4(v.x+a, v.y+a, v.z+a, v.w+a);
}

Vector4 operator +(f32 a, const Vector4& v)
{
    return v + a;
}

Vector4 operator -(const Vector4& v1, const Vector4& v2)
{
    return Vector4(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z, v1.w-v2.w);
}

Vector4 operator -(const Vector4& v, f32 a)
{
    return Vector4(v.x-a, v.y-a, v.z-a, v.w-a);
}

Vector4 operator -(f32 a, const Vector4& v)
{
    return Vector4(a-v.x, a-v.y, a-v.z, a-v.w);
}

Vector4 operator *(const Vector4& v1, const Vector4& v2)
{
    return Vector4(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z, v1.w*v2.w);
}

Vector4 operator *(const Vector4& v, f32 a)
{
    return Vector4(v.x*a, v.y*a, v.z*a, v.w*a);
}

Vector4 operator *(f32 a, const Vector4& v)
{
    return v * a;
}

Vector4 operator /(const Vector4& v1, const Vector4& v2)
{
    return Vector4(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z, v1.w/v2.w);
}

Vector4 operator /(const Vector4& v, f32 a)
{
    return Vector4(v.x/a, v.y/a, v.z/a, v.w/a);
}

Vector4 operator /(f32 a, const Vector4& v)
{
    return Vector4(a/v.x, a/v.y, a/v.z, a/v.w);
}