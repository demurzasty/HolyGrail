#pragma once 

#include "Math.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"

struct Vector4 {
    static const Vector4 Zero;

    static const Vector4 One;

    static float Dot(const Vector4& lhs, const Vector4& rhs);

    static float Length(const Vector4& vec);

    static float Distance(const Vector4& lhs, const Vector4& rhs);

    static Vector4 Normalize(const Vector4& vec);

    static Vector4 NormalizePlane(const Vector4& vec);

    static bool AlmostEquals(const Vector4& lhs, const Vector4& rhs);

    Vector4() = default;

    Vector4(float value);

    Vector4(float x, float y, float z, float w);

    Vector4(const Vector2& xy, const Vector2& zw);

    Vector4(const Vector3& xyz, float w);

    Vector4 operator+(const Vector4& rhs) const;

    Vector4 operator-(const Vector4& rhs) const;

    Vector4 operator*(const Vector4& rhs) const;

    Vector4 operator/(const Vector4& rhs) const;

    Vector4& operator+=(const Vector4& rhs);

    Vector4& operator-=(const Vector4& rhs);

    Vector4& operator*=(const Vector4& rhs);

    Vector4& operator/=(const Vector4& rhs);

    float& operator[](int index);

    float operator[](int index) const;

    float X, Y, Z, W;
};