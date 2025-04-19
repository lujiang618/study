//  copyright daxiangyun, Inc. All Rights Reserved.
#include <algorithm>

#include "Matrix4.h"
#include "Matrix3.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Vector2.h"
#include <math.h>
#include "Math.h"


const Vector3 Vector3::Zero = Vector3(0.0f);
const Vector3 Vector3::One = Vector3(1.0f);
const Vector3 Vector3::X = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Y = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Z = Vector3(0.0f, 0.0f, 1.0f);

Vector3::Vector3()
    : Vector3(0.0f, 0.0f, 0.0f)
{
}

Vector3::Vector3(f32 x)
    : Vector3(x, x, x)
{
}

Vector3::Vector3(f32 x, f32 y, f32 z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3::Vector3(const Vector2& v, f32 z /*= 0.0f*/)
: Vector3(v.x, v.y, z)
{

}

Vector3::Vector3(const Vector3& v)
    : Vector3(v.x, v.y, v.z)
{
}

Vector3::Vector3(const Vector4& v)
    : Vector3(v.x, v.y, v.z)
{
}

Vector3::~Vector3()
{
}

Vector3& Vector3::Set(float x_, float y_, float z_)
{
    x = x_;
    y = y_;
    z = z_;
    return *this;
}

Vector3& Vector3::ApplyMatrix3(const Matrix3& m)
{
    const float x_ = x, y_ = y, z_ = z;
    auto e = m._m;

    x = e[0] * x_ + e[3] * y_ + e[6] * z_;
    y = e[1] * x_ + e[4] * y_ + e[7] * z_;
    z = e[2] * x_ + e[5] * y_ + e[8] * z_;

    return *this;
}

Vector3& Vector3::ApplyMatrix4(const Matrix4& m)
{
    const float x_ = x, y_ = y, z_ = z;
    auto e = m._m;

    const float w = 1 / (e[3] * x_ + e[7] * y_ + e[11] * z_ + e[15]);

    x = (e[0] * x_ + e[4] * y_ + e[8] * z_ + e[12]) * w;
    y = (e[1] * x_ + e[5] * y_ + e[9] * z_ + e[13]) * w;
    z = (e[2] * x_ + e[6] * y_ + e[10] * z_ + e[14]) * w;
    return *this;
}

Vector3& Vector3::Copy(const Vector3& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

Vector3 Vector3::Clone() const
{
    return Vector3(*this);
}


Vector3& Vector3::Min(const Vector3& v)
{
    x = std::min(x, v.x);
    y = std::min(y, v.y);
    z = std::min(z, v.z);

    return *this;
}
Vector3& Vector3::Max(const Vector3& v)
{
    x = std::max(x, v.x);
    y = std::max(y, v.y);
    z = std::max(z, v.z);

    return *this;
}

Vector3& Vector3::Clamp(const Vector3& min, const Vector3& max)
{
    x = Math::Clampf(x, min.x, max.x);
    y = Math::Clampf(y, min.y, max.y);
    z = Math::Clampf(z, min.z, max.z);
    return *this;
}

void Vector3::Normalize()
{
    const f32 len = this->Length();
    if(Math::IsZero(len) == false)
    {
        x /= len; y /=len; z /= len;
    }
}

Vector3 Vector3::Normalize() const
{
    Vector3 nor = *this;
    nor.Normalize();
    return nor;
}

f32 Vector3::Length() const
{
    return sqrtf(x*x + y*y + z*z);
}

f32 Vector3::Dot(const Vector3& v) const
{
    return x*v.x + y * v.y + z*v.z;
}

Vector3 Vector3::Cross(const Vector3& v) const
{
    return Vector3(y*v.z - v.y*z, v.x*z - x*v.z, x*v.y - v.x*y);
}

f32 Vector3::Distance(const Vector3& v) const
{
    const f32 dx = x - v.x;
    const f32 dy = y - v.y;
    const f32 dz = z - v.z;

    return sqrtf(dx * dx + dy *dy + dz*dz);
}

Vector3 Vector3::Middle(const Vector3& v) const
{
    return Vector3(0.5f*(x+v.x), 0.5f*(y+v.y), 0.5f*(z+v.z));
}

Vector3 Vector3::Lerp(const Vector3& v, f32 a) const
{
    const f32 invLerp = 1.0f - a;
    return Vector3(x*a + v.x*invLerp, y*a + v.y*invLerp, z*a + v.z*invLerp);
}

const Vector3& Vector3::operator +=(const Vector3& v)
{
    x += v.x; y += v.y; z += v.z;
    return *this;
}

const Vector3& Vector3::operator +=(f32 a)
{
    x += a; y += a; z+= a;
    return *this;
}

const Vector3& Vector3::operator -=(const Vector3& v)
{
    x -= v.x; y -= v.y; z -= v.z;
    return *this;
}

const Vector3& Vector3::operator -=(f32 a)
{
    x -= a; y -= a; z -= a;
    return *this;
}

const Vector3& Vector3::operator *=(const Vector3& v)
{
    x *= v.x; y *= v.y; z *= v.z;
    return *this;
}

const Vector3& Vector3::operator *=(f32 a)
{
    x *= a; y *= a; z *= a;
    return *this;
}


const Vector3& Vector3::operator /=(const Vector3& v)
{
    x /= v.x; y /= v.y; z /= v.z;
    return *this;
}

const Vector3& Vector3::operator /=(f32 a)
{
    x /= a; y /= a; z /= a;
    return *this;
}

bool Vector3::operator ==(const Vector3& v) const
{
    return Math::Equalf(v.x, this->x) && Math::Equalf(v.y, this->y) && Math::Equalf(v.z, this->z);
}

bool Vector3::operator !=(const Vector3& v) const
{
    return !(v == *this);
}

Vector3 operator +(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

Vector3 operator +(const Vector3& v, f32 a)
{
    return Vector3(v.x+a, v.y+a, v.z+a);
}

Vector3 operator +(f32 a, const Vector3& v)
{
    return v + a;
}

Vector3 operator -(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}

Vector3 operator -(const Vector3& v, f32 a)
{
    return Vector3(v.x-a, v.y-a, v.z-a);
}

Vector3 operator -(f32 a, const Vector3& v)
{
    return Vector3(a-v.x, a-v.y, a-v.z);
}

Vector3 operator *(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
}

Vector3 operator *(const Vector3& v, f32 a)
{
    return Vector3(v.x*a, v.y*a, v.z*a);
}

Vector3 operator *(f32 a, const Vector3& v)
{
    return v * a;
}

Vector3 operator /(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.x/v2.x, v1.y/v2.y, v1.z/v2.z);
}

Vector3 operator /(const Vector3& v, f32 a)
{
    return Vector3(v.x/a, v.y/a, v.z/a);
}

Vector3 operator /(f32 a, const Vector3& v)
{
    return Vector3(a/v.x, a/v.y, a/v.z);
}