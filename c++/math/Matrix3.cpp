// copyright daxiangyun, Inc. All Rights Reserved.

#include "string.h"

#include "Math.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Matrix3.h"


Matrix3 Matrix3::temp_m3_ = Matrix3();

Matrix3::Matrix3()
{
    Identity();
}

Matrix3::Matrix3(const Matrix3& m)
{
   memcpy(&_m[0], m._m, sizeof(float) * 9);
}

Matrix3::Matrix3(float* m)
{
   memcpy(&_m[0], m, sizeof(float) * 9);
}

void Matrix3::Set(const float* m)
{
    for (int i = 0; i < 9; i++)
    {
        _m[i] = m[i];
    }
}

void Matrix3::Identity()
{
    _11 = 1.0f, _12 = 0.0f, _13 = 0.0f;
    _21 = 0.0f, _22 = 1.0f, _23 = 0.0f;
    _31 = 0.0f, _32 = 0.0f, _33 = 1.0f;
}

bool Matrix3::IsIdentity()
{
    return Math::Equalf(_11, 1.0f)
        && Math::Equalf(_12, 0.0f)
        && Math::Equalf(_13, 0.0f)

        && Math::Equalf(_21, 0.0f)
        && Math::Equalf(_22, 1.0f)
        && Math::Equalf(_23, 0.0f)

        && Math::Equalf(_31, 0.0f)
        && Math::Equalf(_32, 0.0f)
        && Math::Equalf(_33, 1.0f);
}

Matrix3& Matrix3::Transpose()
{
    float temp;
    temp = _m[1]; _m[1] = _m[3]; _m[3] = temp;
    temp = _m[2]; _m[2] = _m[6]; _m[6] = temp;
    temp = _m[5]; _m[5] = _m[7]; _m[7] = temp;
    return *this;
}

Matrix3& Matrix3::Invert()
{
    auto te = _m;

    const float
        n11 = te[0], n21 = te[1], n31 = te[2],
        n12 = te[3], n22 = te[4], n32 = te[5],
        n13 = te[6], n23 = te[7], n33 = te[8],

        t11 = n33 * n22 - n32 * n23,
        t12 = n32 * n13 - n33 * n12,
        t13 = n23 * n12 - n22 * n13,

        det = n11 * t11 + n21 * t12 + n31 * t13;

    if (det == 0)
    {
        _11 = 0.0f, _12 = 0.0f, _13 = 0.0f;
        _21 = 0.0f, _22 = 0.0f, _23 = 0.0f;
        _31 = 0.0f, _32 = 0.0f, _33 = 0.0f;
    }
    else
    {
		const float det_inv = 1 / det;

		te[0] = t11 * det_inv;
		te[1] = (n31 * n23 - n33 * n21) * det_inv;
		te[2] = (n32 * n21 - n31 * n22) * det_inv;

		te[3] = t12 * det_inv;
		te[4] = (n33 * n11 - n31 * n13) * det_inv;
		te[5] = (n31 * n12 - n32 * n11) * det_inv;

		te[6] = t13 * det_inv;
		te[7] = (n21 * n13 - n23 * n11) * det_inv;
		te[8] = (n22 * n11 - n21 * n12) * det_inv;
    }

    return *this;
}

Matrix3& Matrix3::Copy(const Matrix3& m)
{
    memcpy(&_m[0], &m._m, sizeof(float) * 9);
    return *this;
}

Matrix3 Matrix3::Clone()
{
    Matrix3 result;
    memcpy(&result._m[0], &_m, sizeof(float) * 9);
    return result;
}

Matrix3& Matrix3::Mul(const float scaler)
{
    for (int i = 0; i < 9; i++)
    {
        _m[i] *= scaler;
    }
    return *this;
}

Matrix3& Matrix3::Mul(const Matrix3& m)
{
    return Mul(*this, m);
}

Matrix3& Matrix3::PreMul(const Matrix3& m)
{
    return Mul(m, *this);
}

Matrix3& Matrix3::Mul(const Matrix3& a, const Matrix3& b)
{
    auto ae = a._m;
    auto be = b._m;
    auto te = _m;

    const float a11 = ae[0], a12 = ae[3], a13 = ae[6];
    const float a21 = ae[1], a22 = ae[4], a23 = ae[7];
    const float a31 = ae[2], a32 = ae[5], a33 = ae[8];

    const float b11 = be[0], b12 = be[3], b13 = be[6];
    const float b21 = be[1], b22 = be[4], b23 = be[7];
    const float b31 = be[2], b32 = be[5], b33 = be[8];

    te[0] = a11 * b11 + a12 * b21 + a13 * b31;
    te[3] = a11 * b12 + a12 * b22 + a13 * b32;
    te[6] = a11 * b13 + a12 * b23 + a13 * b33;

    te[1] = a21 * b11 + a22 * b21 + a23 * b31;
    te[4] = a21 * b12 + a22 * b22 + a23 * b32;
    te[7] = a21 * b13 + a22 * b23 + a23 * b33;

    te[2] = a31 * b11 + a32 * b21 + a33 * b31;
    te[5] = a31 * b12 + a32 * b22 + a33 * b32;
    te[8] = a31 * b13 + a32 * b23 + a33 * b33;

    return *this;
}


Matrix3& Matrix3::SetFromMatrix4(const Matrix4& m)
{
    _11 = m._00; _12 = m._01; _13 = m._02;
    _21 = m._10; _22 = m._11; _23 = m._12;
    _31 = m._20; _32 = m._21; _33 = m._22;

    return *this;
}

float Matrix3::Determinant() const
{
    auto te = _m;

    const float a = te[0], b = te[1], c = te[2],
        d = te[3], e = te[4], f = te[5],
        g = te[6], h = te[7], i = te[8];

    return a * e * i - a * f * h - b * d * i + b * f * g + c * d * h - c * e * g;
}

Matrix3& Matrix3::Scale(const Vector2& v)
{
    return Scale(v.x, v.y);
}

Matrix3& Matrix3::Scale(float x, float y)
{
    PreMul(temp_m3_.MakeScale(x, y));
    return *this;
}

Matrix3& Matrix3::Rotate(float angle)
{
    PreMul(temp_m3_.MakeRotation(angle));
    return *this;
}

Matrix3& Matrix3::Translate(const Vector2& v)
{
    return Translate(v.x, v.y);
}

Matrix3& Matrix3::Translate(float x, float y)
{
    PreMul(temp_m3_.MakeTranslation(x, y));
    return *this;
}

Matrix3& Matrix3::MakeTranslation(const Vector2& v)
{
    return MakeTranslation(v.x, v.y);
}

Matrix3& Matrix3::MakeTranslation(float x, float y)
{
    Identity();
    _31 = x;
    _32 = y;
    return *this;
}

Matrix3& Matrix3::MakeRotation(float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    Identity();

    _11 = c;
    _21 = -s;

    _12 = s;
    _22 = c;

    return *this;
}



Matrix3& Matrix3::MakeScale(const Vector2& v)
{
    return MakeScale(v.x, v.y);
}

Matrix3& Matrix3::MakeScale(float x, float y)
{

    Identity();

    _11 = x;
    _22 = y;

    return *this;
}

bool operator == (const Matrix3& a, const Matrix3& b)
{
    for (int i = 0; i < 9; i++)
    {
        if (!Math::Equalf(a._m[i], b._m[i]))
        {
            return false;
        }
    }
    return true;
}

bool operator != (const Matrix3& a, const Matrix3& b)
{
   return !(a == b);
}

Matrix3 operator * (const Matrix3& a, const Matrix3& b)
{
    Matrix3 result;
    result.Mul(a, b);
    return result;
}

Vector2 operator*(const Matrix3& a, const Vector2& b)
{
     // 将 Vector2 扩展为齐次坐标 [x, y, 1]
     const float x = a._11 * b.x + a._12 * b.y + a._13 * 1.0f;
     const float y = a._21 * b.x + a._22 * b.y + a._23 * 1.0f;
     const float w = a._31 * b.x + a._32 * b.y + a._33 * 1.0f; // 透视分量

     // 齐次坐标归一化（当 w != 0 且 w != 1 时）
     if (w != 0.0f && w != 1.0f) {
         return Vector2(x / w, y / w);
     }

     return Vector2(x, y);
}

Vector3& operator *= (Vector3& v, const Matrix3& m)
{
    float x = v.x, y = v.y, z = v.z;
    v.x = x * m._11 + y * m._21 + z * m._31;
    v.y = x * m._12 + y * m._22 + z * m._32;
    v.z = x * m._13 + y * m._23 + z * m._33;
    return v;
}