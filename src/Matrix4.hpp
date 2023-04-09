#pragma once 

#include "Vector3.hpp"
#include "Vector4.hpp"

struct Matrix4 {
    static const Matrix4 Identity;

    static Vector3 ExtractPosition(const Matrix4& matrix);

    static Vector3 ExtractEuler(const Matrix4& matrix);

    static Vector3 ExtractDirection(const Matrix4& matrix);

    static Matrix4 CreatePerspective(float fovy, float aspect, float near, float far);

    static Matrix4 CreateOrthographic(float left, float right, float bottom, float top, float near, float far);

    static Matrix4 CreateTranslation(const Vector3& position);

    static Matrix4 CreateRotationX(float radians);

    static Matrix4 CreateRotationY(float radians);

    static Matrix4 CreateRotationZ(float radians);

    static Matrix4 CreateRotation(const Vector3& rotation);

    static Matrix4 CreateRotation(float radians, const Vector3& axis);

    static Matrix4 CreateScaling(const Vector3& scale);

    static Matrix4 Invert(const Matrix4& matrix);

    static Matrix4 Transpose(const Matrix4& matrix);

    static Vector3 TransformNormal(const Matrix4& matrix, const Vector3& vector);

    float& operator[](int index);

    const float& operator[](int index) const;

    bool operator==(const Matrix4& rhs) const;

    bool operator!=(const Matrix4& rhs) const;

    Matrix4 operator*(const Matrix4& rhs) const;

    Vector4 operator*(const Vector4& rhs) const;

    float Values[16];
};
