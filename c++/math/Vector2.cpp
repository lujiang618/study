//  copyright daxiangyun, Inc. All Rights Reserved.

#include "Vector2.h"
#include <math.h>
#include <algorithm>
#include "Math.h"

const Vector2 Vector2::Zero = Vector2(0.0f);
const Vector2 Vector2::One = Vector2(1.0f);
const Vector2 Vector2::X = Vector2(1.0f, 0.0f);
const Vector2 Vector2::Y = Vector2(0.0f, 1.0f);

Vector2::Vector2()
    : Vector2(0.0f, 0.0f)
{
}

Vector2::Vector2(f32 x)
    : Vector2(x, x)
{
}

Vector2::Vector2(f32 x, f32 y)
{
    this->x = x;
    this->y = y;
}

Vector2::Vector2(const Vector2& v)
    : Vector2(v.x, v.y)
{
}

Vector2::~Vector2()
{
}

const Vector2& Vector2::Normalize()
{
    const f32 len = this->Length();
    if(Math::IsZero(len))
        return *this;
    x /= len; y /=len;
    return *this;
}

f32 Vector2::Length() const
{
    return sqrtf(x*x + y*y);
}

f32 Vector2::Dot(const Vector2& v) const
{
    return x*v.x + y * v.y;
}

f32 Vector2::Cross(const Vector2& v) const
{
    return x*v.y - v.x*y;
}

f32 Vector2::Distance(const Vector2& v) const
{
    const f32 dx = x - v.x;
    const f32 dy = y - v.y;
    return sqrtf(dx * dx + dy *dy);
}

Vector2 Vector2::Middle(const Vector2& v) const
{
    return Vector2(0.5f*(x+v.x), 0.5f*(y+v.y));
}

Vector2 Vector2::Lerp(const Vector2& v, f32 a) const
{
    const f32 invLerp = 1.0f - a;
    return Vector2(x*a + v.x*invLerp, y*a + v.y*invLerp);
}

Vector2 Vector2::Min(const Vector2& v)
{
    x = std::min(x, v.x);
    y = std::min(y, v.y);

    return *this;
}

Vector2 Vector2::Max(const Vector2& v)
{
    x = std::max(x, v.x);
    y = std::max(y, v.y);

    return *this;
}

const Vector2& Vector2::operator +=(const Vector2& v)
{
    x += v.x; y += v.y;
    return *this;
}

const Vector2& Vector2::operator +=(f32 a)
{
    x += a; y += a;
    return *this;
}

const Vector2& Vector2::operator -=(const Vector2& v)
{
    x -= v.x; y -= v.y;
    return *this;
}

const Vector2& Vector2::operator -=(f32 a)
{
    x -= a; y -= a;
    return *this;
}

const Vector2& Vector2::operator *=(const Vector2& v)
{
    x *= v.x; y *= v.y;
    return *this;
}

const Vector2& Vector2::operator *=(f32 a)
{
    x *= a; y *= a;
    return *this;
}

const Vector2& Vector2::operator /=(const Vector2& v)
{
    x /= v.x; y /= v.y;
    return *this;
}

const Vector2& Vector2::operator /=(f32 a)
{
    x /= a; y /= a;
    return *this;
}

bool Vector2::operator ==(const Vector2& v) const
{
    return Math::Equalf(v.x, this->x) && Math::Equalf(v.y, this->y);
}

bool Vector2::operator !=(const Vector2& v) const
{
    return !( v == *this);
}

Vector2 operator +(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.x+v2.x, v1.y+v2.y);
}

Vector2 operator +(const Vector2& v, f32 a)
{
    return Vector2(v.x+a, v.y+a);
}

Vector2 operator +(f32 a, const Vector2& v)
{
    return v + a;
}

Vector2 operator -(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.x-v2.x, v1.y-v2.y);
}

Vector2 operator -(const Vector2& v, f32 a)
{
    return Vector2(v.x-a, v.y-a);
}

Vector2 operator -(f32 a, const Vector2& v)
{
    return Vector2(a-v.x, a-v.y);
}

Vector2 operator *(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.x*v2.x, v1.y*v2.y);
}

Vector2 operator *(const Vector2& v, f32 a)
{
    return Vector2(v.x*a, v.y*a);
}

Vector2 operator *(f32 a, const Vector2& v)
{
    return v * a;
}

Vector2 operator /(const Vector2& v1, const Vector2& v2)
{
    return Vector2(v1.x/v2.x, v1.y/v2.y);
}

Vector2 operator /(const Vector2& v, f32 a)
{
    return Vector2(v.x/a, v.y/a);
}

Vector2 operator /(f32 a, const Vector2& v)
{
    return Vector2(a/v.x, a/v.y);
}