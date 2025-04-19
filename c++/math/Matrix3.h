// copyright daxiangyun, Inc. All Rights Reserved.

#pragma once

class Vector2;
class Vector3;
class Matrix4;

class Matrix3
{

public:
    static Matrix3 temp_m3_;

public:
    union
    {
        struct
        {
            float  _m[9];
        };

        struct
        {
            float _11, _12, _13;
            float _21, _22, _23;
            float _31, _32, _33;
        };

    };

    Matrix3();
    Matrix3(float* m);

    Matrix3(const Matrix3& m);

    void Set(const float* m);
    void Identity();
    bool IsIdentity();

    Matrix3& Transpose();
    Matrix3& Invert();

    Matrix3& Copy(const Matrix3& m);
    Matrix3 Clone();

    Matrix3& Mul(const float s);
    Matrix3& Mul(const Vector3 v);
    Matrix3& Mul(const Matrix3& m);
    Matrix3& Mul(const Matrix3& a, const Matrix3& b);
    Matrix3& PreMul(const Matrix3& m);

    Matrix3& SetFromMatrix4(const Matrix4& m);

    float Determinant() const;

    Matrix3& Scale(float x, float y);
    Matrix3& Scale(const Vector2& v);
    Matrix3& Rotate(float angle);
    Matrix3& Translate(float x, float y);
    Matrix3& Translate(const Vector2& v);
    Matrix3& MakeScale(float x, float y);
    Matrix3& MakeScale(const Vector2& v);
    Matrix3& MakeRotation(float angle);
    Matrix3& MakeTranslation(float x, float y);
    Matrix3& MakeTranslation(const Vector2& v);

};

extern bool operator == (const Matrix3& a, const Matrix3& b);
extern bool operator != (const Matrix3& a, const Matrix3& b);
extern Matrix3 operator * (const Matrix3& a, const Matrix3& b);
extern Vector2 operator*(const Matrix3& a, const Vector2& b);
extern Vector3& operator *= (Vector3& v, const Matrix3& m);