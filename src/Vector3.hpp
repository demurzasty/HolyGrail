#pragma once 

#include "Math.hpp"
#include "Vector2.hpp"

struct Vector3 {
    static const Vector3 Zero;

    static const Vector3 One;

    static const Vector3 AxisX;

    static const Vector3 AxisY;

    static const Vector3 AxisZ;

    static float Dot(const Vector3& lhs, const Vector3& rhs);

    static float Length(const Vector3& vec);

    static float Distance(const Vector3& lhs, const Vector3& rhs);

    static Vector3 Normalize(const Vector3& vec);

    static bool AlmostEquals(const Vector3& lhs, const Vector3& rhs);

    Vector3() = default;

    Vector3(float value);

    Vector3(float x, float y, float z);

    Vector3(float x, const Vector2& yz);

    Vector3(const Vector2& xy, float z);

    Vector3 operator+(const Vector3& rhs) const;

    Vector3 operator-(const Vector3& rhs) const;

    Vector3 operator*(const Vector3& rhs) const;

    Vector3 operator/(const Vector3& rhs) const;

    Vector3& operator+=(const Vector3& rhs);

    Vector3& operator-=(const Vector3& rhs);

    Vector3& operator*=(const Vector3& rhs);

    Vector3& operator/=(const Vector3& rhs);

    bool operator==(const Vector3& rhs) const;

    bool operator!=(const Vector3& rhs) const;

    float& operator[](int index);

    float operator[](int index) const;

    float X, Y, Z;
};
