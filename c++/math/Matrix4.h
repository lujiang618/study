//  copyright daxiangyun, Inc. All Rights Reserved.

#pragma once

#include "TypeDefines.h"

class Vector3;
class Vector4;

class Matrix4
{
public:
   union
   {
   struct
   {
      f32 _m[16];
   };
   struct
   {
      f32 _00, _01, _02, _03;
      f32 _10, _11, _12, _13;
      f32 _20, _21, _22, _23;
      f32 _30, _31, _32, _33;
   };
   };
   Matrix4();
   Matrix4(f32* m);
   Matrix4(const Vector3& pos, const Vector3& rot, const Vector3& scale);
   Matrix4(const Matrix4& m);
   Matrix4(const float& s);

   void Set(const Vector3& pos, const Vector3& rot, const Vector3& scale);
   void Identity();
   bool IsIdentity() const;

   Matrix4& Copy(const Matrix4& m);

   Matrix4 Transpose() const;
   Matrix4 Invert() const;

   Vector3 Mul(const Vector3& v) const;
   Vector4 Mul(const Vector4& v) const;
   Matrix4 Mul(const Matrix4& m) const;


   Matrix4& Mul(const Matrix4& m);
   Matrix4& Mul(const Matrix4& m1, const Matrix4& m2);
   Matrix4& PreMul(const Matrix4& m);

   Vector3 GetTranslation() const;
   void SetTranslation(const Vector3& pos);

   Vector3 GetRotation() const;
   void SetRotation(const Vector3& rot);

   Vector3 GetScale() const;
   void SetScale(const Vector3& scale);

   Matrix4& MakeRotationX(float angle);
   Matrix4& MakeRotationY(float angle);
   Matrix4& MakeRotationZ(float angle);
   Matrix4& MakeTranslation(const Vector3& v);
   Matrix4& MakeTranslation(float x, float y, float z);

   bool operator ==(const Matrix4& m) const;
   bool operator !=(const Matrix4& m) const;

   static Matrix4 Perspective(float fov, float aspect, float z_near, float z_far);

   static Matrix4 Frustum(float left, float right, float bottom, float top, float z_near, float z_far);

   static Matrix4 Ortho(float left, float right, float bottom, float top, float z_near, float z_far);

   static Matrix4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

   // create rotate  matrix
   static Matrix4 Rotate(const Vector3& v);

   // create scale matrix
   static Matrix4 Scale(const Vector3& v);

   // create translate matrix
   static Matrix4 Translate(const Vector3& v);


};

extern Vector3 operator *(const Vector3& v, const Matrix4& m);
extern Vector3& operator *= (Vector3& v, const Matrix4& m);

extern Vector3 operator *(const Matrix4& m, const Vector3& v);
extern Vector4 operator *(const Vector4& v, const Matrix4& m);
extern Vector4 operator *(const Matrix4& m, const Vector4& v);
extern Matrix4 operator *(const Matrix4& m1, const Matrix4& m2);