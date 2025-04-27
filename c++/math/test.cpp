
#include <iostream>

#include "./Matrix4.h"
#include "./Matrix3.h"
#include "./Vector3.h"
#include "./Vector2.h"

Vector2 toCellCoordinate(Vector2 point, Matrix3 matrix)
{
    auto result = matrix * point;

    return result;
}

int main()
{
    float size  = 200.0f;
    float scale = 1 / size;

    Matrix3 matrix;

    matrix.Scale(Vector2(scale));

    Vector3 point1(220, 122, 389);

    // Vector3 point1ToScale = matrix.Mul(point1);
    Vector2 point1ToScale = toCellCoordinate(Vector2(220, 122), matrix);

    // printf("%f,%f,%f\n", point1ToScale.x, point1ToScale.y, point1ToScale.z);
    printf("%f,%f\n", point1ToScale.x, point1ToScale.y);

    Vector2 point1Toworld = toCellCoordinate(point1ToScale, matrix.Invert());

    printf("%f,%f\n", point1Toworld.x, point1Toworld.y);

    return 0;
}

// Vector3 toCellCoordinate(Vector3 point, Matrix3 matrix)
// {
//     auto result = matrix.Mul(point);

//     return result;
// }

