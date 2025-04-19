//  copyright daxiangyun, Inc. All Rights Reserved.

#include <assert.h>

#include "Matrix4.h"
#include "Math.h"
#include "string.h"

#include "Vector3.h"
#include "Vector4.h"

Matrix4::Matrix4()
{
    Identity();
}

Matrix4::Matrix4(f32* m)
{
   memcpy(&_m[0], m, sizeof(f32) *16);
}

Matrix4::Matrix4(const Vector3& pos, const Vector3& rot, const Vector3& scale)
{
    this->Set(pos, rot, scale);
}

Matrix4::Matrix4(const Matrix4& m)
{
   *this = m;
}

Matrix4::Matrix4(const float &s)
{
    _00 = s, _01 = 0.0, _02 = 0.0, _03 = 0.0;
	_10 = 0.0, _11 = s, _12 = 0.0, _13 = 0.0;
	_20 = 0.0, _21 = 0.0, _22 = s, _23 = 0.0;
	_30 = 0.0, _31 = 0.0, _32 = 0.0, _33 = s;
}

Matrix4& Matrix4::Copy(const Matrix4& m)
{
    memcpy(&_m[0], m._m, sizeof(float) * 16);
    return *this;
}

void Matrix4::Set(const Vector3& pos, const Vector3& rot, const Vector3& scale)
{
    /**
        X:
        1		0		0
        0		cosX	-sinX
        0		sinX    cosX

        Y:
        cosY	0		sinY
        0		1		0
        -SinY	0		cosY

        Z:
        cosZ	-sinZ 	0
        sinZ	cosZ	0
        0		0		1

        Z*X
        cosZ	-cosX*sinZ			sinX*sinZ
        sinZ 	cosX*cosZ 			-sinXcosZ
        0		sinX  				cosX

        Z*X*Y

        cosY*cosZ - sinX*sinY*sinZ 			-cosX*sinZ				sinY * cosZ + sinX*cosY*sinZ
        cosY*sinZ + sinX*sinY*cosZ  		cosX *cosZ 				sinY * sinZ - sinX*cosY*cosZ
        -cosX*SinY							sinX 					cosX*cosY


        scale:

        Sx 				0				0
        0				Sy 				0
        0				0				Sz


        (cosY*cosZ - sinX*sinY*sinZ) * Sx    (-cosX*sinZ) * Sx    (sinY * cosZ + sinX*cosY*sinZ) * Sx

        (cosY*sinZ + sinX*sinY*cosZ) * Sy  	 (cosX * cosZ) * Sy	  (sinY * sinZ - sinX*cosY*cosZ) * Sy

        (-cosX*SinY) * Sz					 (SinX) * Sz 					(cosX*cosY) * Sz

        以上为行主序算法，
        数学库中按列主序算法
    */


    float degx = DEG2RAD *rot.x;
    float degy = DEG2RAD *rot.y;
    float degz = DEG2RAD *rot.z;

    float sx = sinf(degx);
    float cx = cosf(degx);
    float sy = sinf(degy);
    float cy = cosf(degy);
    float sz = sinf(degz);
    float cz = cosf(degz);

    _00 = (cy * cz - sx * sy * sz)* scale.x;
    _01 = (cy * sz + sx * sy * cz) * scale.y;
    _02 = (-cx * sy) * scale.z;
    _03 = 0.0;
    _10 = (-cx * sz) * scale.x;
    _11 = (cx * cz) * scale.y;
    _12 = (sx) * scale.z;
    _13 = 0.0;
    _20 = (sy * cz + sx * cy * sz) * scale.x;
    _21 = (sy * sz - sx * cy * cz) * scale.y;
    _22 = (cx * cy) * scale.z;
    _23 = 0.0;
    _30 = pos.x;
    _31 = pos.y;
    _32 = pos.z;
    _33 = 1.0;
}

void Matrix4::Identity()
{
    _00 = 1.0, _01 = 0.0, _02 = 0.0, _03 = 0.0;
	_10 = 0.0, _11 = 1.0, _12 = 0.0, _13 = 0.0;
	_20 = 0.0, _21 = 0.0, _22 = 1.0, _23 = 0.0;
	_30 = 0.0, _31 = 0.0, _32 = 0.0, _33 = 1.0;
}

bool Matrix4::IsIdentity() const
{
    return Math::Equalf(_00, 1.0f)
        && Math::Equalf(_01, 0.0f)
        && Math::Equalf(_02, 0.0f)
        && Math::Equalf(_03, 0.0f)
        && Math::Equalf(_10, 0.0f)
        && Math::Equalf(_11, 1.0f)
        && Math::Equalf(_12, 0.0f)
        && Math::Equalf(_13, 0.0f)
        && Math::Equalf(_20, 0.0f)
        && Math::Equalf(_21, 0.0f)
        && Math::Equalf(_22, 1.0f)
        && Math::Equalf(_23, 0.0f)
        && Math::Equalf(_30, 0.0f)
        && Math::Equalf(_31, 0.0f)
        && Math::Equalf(_32, 0.0f)
        && Math::Equalf(_33, 1.0f);
}

Matrix4 Matrix4::Transpose() const
{
    Matrix4 m;
    m._00 = _00;    m._01 = _10;    m._02 = _20;    m._03 = _30;
    m._10 = _01;    m._11 = _11;    m._12 = _21;    m._13 = _31;
    m._20 = _02;    m._21 = _12;    m._22 = _22;    m._23 = _32;
    m._30 = _03;    m._31 = _13;    m._32 = _23;    m._33 = _33;
    return m;
}

Matrix4 Matrix4::Invert() const
{
    f32 a00 = _11 * (_22 * _33 - _23 * _32) + _12 * (_23 * _31 - _21 * _33) + _13 * (_21 * _32 - _22 * _31);
    f32 a01 = _10 * (_22 * _33 - _23 * _32) + _12 * (_23 * _30 - _20 * _33) + _13 * (_20 * _32 - _22 * _30);
    f32 a02 = _10 * (_21 * _33 - _23 * _31) + _11 * (_23 * _30 - _20 * _33) + _13 * (_20 * _31 - _21 * _30);
    f32 a03 = _10 * (_21 * _32 - _22 * _31) + _11 * (_22 * _30 - _20 * _32) + _12 * (_20 * _31 - _21 * _30);

    f32 a10 = _01 * (_22 * _33 - _23 * _32) + _02 * (_23 * _31 - _21 * _33) + _03 * (_21 * _32 - _22 * _31);
    f32 a11 = _00 * (_22 * _33 - _23 * _32) + _02 * (_23 * _30 - _20 * _33) + _03 * (_20 * _32 - _22 * _30);
    f32 a12 = _00 * (_21 * _33 - _23 * _31) + _01 * (_23 * _30 - _20 * _33) + _03 * (_20 * _31 - _21 * _30);
    f32 a13 = _00 * (_21 * _32 - _22 * _31) + _01 * (_22 * _30 - _20 * _32) + _02 * (_20 * _31 - _21 * _30);

    f32 a20 = _01 * (_12 * _33 - _13 * _32) + _02 * (_13 * _31 - _11 * _33) + _03 * (_11 * _32 - _12 * _31);
    f32 a21 = _00 * (_12 * _33 - _13 * _32) + _02 * (_13 * _30 - _10 * _33) + _03 * (_10 * _32 - _12 * _30);
    f32 a22 = _00 * (_11 * _33 - _13 * _31) + _01 * (_13 * _30 - _10 * _33) + _03 * (_10 * _31 - _11 * _30);
    f32 a23 = _00 * (_11 * _32 - _12 * _31) + _01 * (_12 * _30 - _10 * _32) + _02 * (_10 * _31 - _11 * _30);

    f32 a30 = _01 * (_12 * _23 - _13 * _22) + _02 * (_13 * _21 - _11 * _23) + _03 * (_11 * _22 - _12 * _21);
    f32 a31 = _00 * (_12 * _23 - _13 * _22) + _02 * (_13 * _20 - _10 * _23) + _03 * (_10 * _22 - _12 * _20);
    f32 a32 = _00 * (_11 * _23 - _13 * _21) + _01 * (_13 * _20 - _10 * _23) + _03 * (_10 * _21 - _11 * _20);
    f32 a33 = _00 * (_11 * _22 - _12 * _21) + _01 * (_12 * _20 - _10 * _22) + _02 * (_10 * _21 - _11 * _20);

    f32 det = 1.0f / (_00 * a00 - _01 * a01 + _02 * a02 - _03 * a03);
    Matrix4 m;
    m._00 = a00 * det;
    m._01 = -a10 * det;
    m._02 = a20 * det;
    m._03 = -a30 * det;
    m._10 = -a01 * det;
    m._11 = a11 * det;
    m._12 = -a21 * det;
    m._13 = a31 * det;
    m._20 = a02 * det;
    m._21 = -a12 * det;
    m._22 = a22 * det;
    m._23 = -a32 * det;
    m._30 = -a03 * det;
    m._31 = a13 * det;
    m._32 = -a23 * det;
    m._33 = a33 * det;
    return m;
}

Vector3 Matrix4::Mul(const Vector3& v) const
{
    return (*this) * v;
}

Vector4 Matrix4::Mul(const Vector4& v) const
{
    return (*this) * v;
}

Matrix4 Matrix4::Mul(const Matrix4& m) const
{
    return (*this) * m;
}

Matrix4& Matrix4::Mul(const Matrix4& m)
{
    return Mul(*this, m);
}

Matrix4& Matrix4::PreMul(const Matrix4& m)
{
    return Mul(m, *this);
}

Matrix4& Matrix4::Mul(const Matrix4& m1, const Matrix4& m2)
{

    auto ae = m1._m;
    auto be = m2._m;
    auto te = _m;

    const float a11 = ae[0], a12 = ae[4], a13 = ae[8], a14 = ae[12];
    const float a21 = ae[1], a22 = ae[5], a23 = ae[9], a24 = ae[13];
    const float a31 = ae[2], a32 = ae[6], a33 = ae[10], a34 = ae[14];
    const float a41 = ae[3], a42 = ae[7], a43 = ae[11], a44 = ae[15];

    const float b11 = be[0], b12 = be[4], b13 = be[8], b14 = be[12];
    const float b21 = be[1], b22 = be[5], b23 = be[9], b24 = be[13];
    const float b31 = be[2], b32 = be[6], b33 = be[10], b34 = be[14];
    const float b41 = be[3], b42 = be[7], b43 = be[11], b44 = be[15];

    te[0] = a11 * b11 + a12 * b21 + a13 * b31 + a14 * b41;
    te[4] = a11 * b12 + a12 * b22 + a13 * b32 + a14 * b42;
    te[8] = a11 * b13 + a12 * b23 + a13 * b33 + a14 * b43;
    te[12] = a11 * b14 + a12 * b24 + a13 * b34 + a14 * b44;

    te[1] = a21 * b11 + a22 * b21 + a23 * b31 + a24 * b41;
    te[5] = a21 * b12 + a22 * b22 + a23 * b32 + a24 * b42;
    te[9] = a21 * b13 + a22 * b23 + a23 * b33 + a24 * b43;
    te[13] = a21 * b14 + a22 * b24 + a23 * b34 + a24 * b44;

    te[2] = a31 * b11 + a32 * b21 + a33 * b31 + a34 * b41;
    te[6] = a31 * b12 + a32 * b22 + a33 * b32 + a34 * b42;
    te[10] = a31 * b13 + a32 * b23 + a33 * b33 + a34 * b43;
    te[14] = a31 * b14 + a32 * b24 + a33 * b34 + a34 * b44;

    te[3] = a41 * b11 + a42 * b21 + a43 * b31 + a44 * b41;
    te[7] = a41 * b12 + a42 * b22 + a43 * b32 + a44 * b42;
    te[11] = a41 * b13 + a42 * b23 + a43 * b33 + a44 * b43;
    te[15] = a41 * b14 + a42 * b24 + a43 * b34 + a44 * b44;

    return *this;
}

Matrix4& Matrix4::MakeRotationX(float angle)
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

Matrix4& Matrix4::MakeRotationY(float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    Identity();
    _00 = c;
    _20 = s;

    _02 = -s;
    _22 = c;
    return *this;
}

Matrix4& Matrix4::MakeRotationZ(float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    Identity();
    _00 = c;
    _10 = -s;

    _01 = s;
    _11 = c;
    return *this;
}

Matrix4& Matrix4::MakeTranslation(const Vector3& v)
{
    Identity();

    _30 = v.x;
    _31 = v.y;
    _32 = v.z;

    return *this;
}

Matrix4& Matrix4::MakeTranslation(float x, float y, float z)
{
    Identity();
    _30 = x;
    _31 = y;
    _32 = z;
    return *this;
}

Vector3 Matrix4::GetTranslation() const
{
    return Vector3(_30, _31, _32);
}

void Matrix4::SetTranslation(const Vector3 &pos)
{
    _30 = pos.x;
    _31 = pos.y;
    _32 = pos.z;
}

Vector3 Matrix4::GetRotation() const
{
    Vector3 scale = GetScale();
    const Vector3 invScale(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);

    float x, y, z;
    x = asinf(Math::Clampf(_12*invScale.x, -1.0f, 1.0f));
    // float cosx =  atanf(sqrtf(_02*_02 + _22*_22));// cos(x);
    x *= RAD2DEG;

    y = atan2f(-_02, _22);
    y *= RAD2DEG;

    z = atan2f(-_10, _11);
    z *= RAD2DEG;

    // if (Math::IsZero(cosx))
    // {
    //     // x = 0.0f;
    //     // const float rotx = _11 * invScale.y;
    //     // const float roty = -_10 * invScale.y;
    //     // z = atan2f(roty, rotx) * RAD2DEG;
    // }
    // else
    // {
    //     const float invCosx = 1.0f / cosx;
    //     float rotx = _02 * invCosx * invScale.z;
    //     float roty = _22 * invCosx * invScale.z;
    //     x = atan2f(-rotx, roty) * RAD2DEG;
    //     rotx = _10 * invCosx * invScale.x;
    //     roty = _11 * invCosx * invScale.y;
    //     z = atan2f(-rotx, roty) * RAD2DEG;
    // }

    if (x < 0.0f) x += 360.0f;
    if (y < 0.0f) y += 360.0f;
    if (z < 0.0f) z += 360.0f;

    return Vector3(x, y, z);
}

void Matrix4::SetRotation(const Vector3 &rot)
{
    Vector3 pos = GetTranslation();
    Vector3 scale = GetScale();

    Set(pos, rot, scale);
}

Vector3 Matrix4::GetScale() const
{
    if (Math::IsZero(_01) && Math::IsZero(_02) && Math::IsZero(_10) && Math::IsZero(_12)&& Math::IsZero(_20) && Math::IsZero(_21))
    {
        return Vector3(_00, _11, _22);
    }
    else
    {
        return Vector3(sqrtf(_00*_00 + _10*_10 + _20*_20),
            sqrtf(_01 * _01 + _11 * _11 + _21 * _21),
            sqrtf(_02 * _02 + _12 * _12 + _22 * _22));
    }
}

void Matrix4::SetScale(const Vector3& scale)
{
    Vector3 pos = GetTranslation();
    Vector3 rot = GetRotation();

    Set(pos, rot, scale);
}

bool Matrix4::operator ==(const Matrix4& m) const
{
    for(int i=0; i<16; i++)
    {
        if(Math::Equalf(_m[i], m._m[i]) == false)
        {
            return false;
        }
    }
    return true;
}

bool Matrix4::operator !=(const Matrix4& m) const
{
    return !(m == *this);
}

Vector3 operator *(const Vector3& v, const Matrix4& m)
{
    Vector3 result;
    result.x = v.x * m._00 + v.y * m._10 + v.z * m._20;
    result.y = v.x * m._01 + v.y * m._11 + v.z * m._21;
    result.z = v.x * m._02 + v.y * m._12 + v.z * m._22;
    return result;
}

Vector3& operator *=(Vector3& v, const Matrix4& m)
{
    float x = v.x, y = v.y, z = v.z;
    auto e = m._m;
    float w = 1.f / (e[3] * x + e[7] * y + e[11] * z + e[15]);

	v.x = (e[0] * x + e[4] * y + e[8] * z + e[12]) * w;
    v.y = (e[1] * x + e[5] * y + e[9] * z + e[13]) * w;
    v.z = (e[2] * x + e[6] * y + e[10] * z + e[14]) * w;

    return v;
}

Vector3 operator *(const Matrix4& m, const Vector3& v)
{
    Vector3 result;
    result.x = v.x * m._00 + v.y * m._10 + v.z * m._20;
    result.y = v.x * m._01 + v.y * m._11 + v.z * m._21;
    result.z = v.x * m._02 + v.y * m._12 + v.z * m._22;
    return result;
}

Vector4 operator *(const Vector4& v, const Matrix4& m)
{
    Vector4 result;
    result.x = v.x * m._00 + v.y * m._10 + v.z * m._20 + v.w * m._30;
    result.y = v.x * m._01 + v.y * m._11 + v.z * m._21 + v.w * m._31;
    result.z = v.x * m._02 + v.y * m._12 + v.z * m._22 + v.w * m._32;
    result.w = v.x * m._03 + v.y * m._13 + v.z * m._23 + v.w * m._33;
    return result;
}

Vector4 operator *(const Matrix4& m, const Vector4& v)
{
    Vector4 result;
    result.x = v.x * m._00 + v.y * m._10 + v.z * m._20 + v.w * m._30;
    result.y = v.x * m._01 + v.y * m._11 + v.z * m._21 + v.w * m._31;
    result.z = v.x * m._02 + v.y * m._12 + v.z * m._22 + v.w * m._32;
    result.w = v.x * m._03 + v.y * m._13 + v.z * m._23 + v.w * m._33;
    return result;
}

Matrix4 operator *(const Matrix4& m1, const Matrix4& m2)
{
    Matrix4 result;
    result._00 = m1._00 * m2._00 + m1._01 * m2._10 + m1._02 * m2._20 + m1._03 * m2._30;
    result._01 = m1._00 * m2._01 + m1._01 * m2._11 + m1._02 * m2._21 + m1._03 * m2._31;
    result._02 = m1._00 * m2._02 + m1._01 * m2._12 + m1._02 * m2._22 + m1._03 * m2._32;
    result._03 = m1._00 * m2._03 + m1._01 * m2._13 + m1._02 * m2._23 + m1._03 * m2._33;

    result._10 = m1._10 * m2._00 + m1._11 * m2._10 + m1._12 * m2._20 + m1._13 * m2._30;
    result._11 = m1._10 * m2._01 + m1._11 * m2._11 + m1._12 * m2._21 + m1._13 * m2._31;
    result._12 = m1._10 * m2._02 + m1._11 * m2._12 + m1._12 * m2._22 + m1._13 * m2._32;
    result._13 = m1._10 * m2._03 + m1._11 * m2._13 + m1._12 * m2._23 + m1._13 * m2._33;

    result._20 = m1._20 * m2._00 + m1._21 * m2._10 + m1._22 * m2._20 + m1._23 * m2._30;
    result._21 = m1._20 * m2._01 + m1._21 * m2._11 + m1._22 * m2._21 + m1._23 * m2._31;
    result._22 = m1._20 * m2._02 + m1._21 * m2._12 + m1._22 * m2._22 + m1._23 * m2._32;
    result._23 = m1._20 * m2._03 + m1._21 * m2._13 + m1._22 * m2._23 + m1._23 * m2._33;

    result._30 = m1._30 * m2._00 + m1._31 * m2._10 + m1._32 * m2._20 + m1._33 * m2._30;
    result._31 = m1._30 * m2._01 + m1._31 * m2._11 + m1._32 * m2._21 + m1._33 * m2._31;
    result._32 = m1._30 * m2._02 + m1._31 * m2._12 + m1._32 * m2._22 + m1._33 * m2._32;
    result._33 = m1._30 * m2._03 + m1._31 * m2._13 + m1._32 * m2._23 + m1._33 * m2._33;
    return result;
}

Matrix4 Matrix4::Perspective(float fov, float aspect, float z_near, float z_far)
{
    assert(abs(aspect) > 0.0F);

    float const tan_half_fov = tan(fov * 0.5f);

    Matrix4 result(0.0f);

    result._00 = 1.0f / (aspect * tan_half_fov);
    result._11 = 1.0f / (tan_half_fov);
    result._22 = z_far / (z_near - z_far);
    result._23 = -1.0f;
    result._32 = -(z_far * z_near) / (z_far - z_near);

    return result;
}

Matrix4 Matrix4::Frustum(float left, float right, float bottom, float top, float z_near, float z_far)
{
    Matrix4 result(0.0f);

    result._00 = (2.0f * z_near) / (right - left);
    result._11 = (2.0f * z_near) / (top - bottom);
    result._20 = (right + left) / (right - left);
    result._21 = (top + bottom) / (top - bottom);
    result._23 = -1.0f;
    result._22 = z_far / (z_near - z_far);
    result._32 = -(z_far * z_near) / (z_far - z_near);

    return result;
}

Matrix4 Matrix4::Ortho(float left, float right, float bottom, float top, float z_near, float z_far)
{
    Matrix4 result(1.0f);

    result._00 = 2.0f / (right - left);
    result._11 = 2.0f / (top - bottom);
    result._30 = - (right + left) / (right - left);
    result._31 = - (top + bottom) / (top - bottom);
    result._22 = - 1.0f / (z_far - z_near);
    result._32 = - z_near / (z_far - z_near);

    return result;
}

Matrix4 Matrix4::LookAt(const Vector3 &eye, const Vector3 &center, const Vector3 &up)
{
    Vector3 f = center - eye; // forward
    f.Normalize();

    Vector3 s = f.Cross(up); // right
    s.Normalize();

    Vector3 u = s.Cross(f); // up

    Matrix4 result(1.0f);

    result._00 = s.x;
    result._10 = s.y;
    result._20 = s.z;
    result._01 = u.x;
    result._11 = u.y;
    result._21 = u.z;
    result._02 =-f.x;
    result._12 =-f.y;
    result._22 =-f.z;
    result._30 =-s.Dot(eye);
    result._31 =-u.Dot(eye);
    result._32 = f.Dot(eye);

    return result;
}

Matrix4 Matrix4::Rotate(const Vector3 &v)
{
    Matrix4 result;

    result.Set(Vector3(0.0f), v, Vector3(1.0f));

    return result;
}

Matrix4 Matrix4::Scale(const Vector3 &v)
{
    Matrix4 result;

    result.Set(Vector3(0.0f), Vector3(0.0f), v);

    return result;
}

Matrix4 Matrix4::Translate(const Vector3 &v)
{
    Matrix4 result;

    result.Set(v, Vector3(0.0f), Vector3(1.0f));

    return result;
}
