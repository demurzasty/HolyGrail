#pragma once

#include "Math.hpp"
#include "Vector2.hpp"

struct Vector2 {
    static const Vector2 Zero;

    static const Vector2 One;

    static float Dot(const Vector2& lhs, const Vector2& rhs);

    static float Length(const Vector2& vec);

    static float Distance(const Vector2& lhs, const Vector2& rhs);

    static Vector2 Normalize(const Vector2& vec);

    static bool AlmostEquals(const Vector2& lhs, const Vector2& rhs);

    Vector2() = default;

    Vector2(float value);

    Vector2(float x, float y);

    Vector2 operator+(const Vector2& rhs) const;

    Vector2 operator-(const Vector2& rhs) const;

    Vector2 operator*(const Vector2& rhs) const;

    Vector2 operator/(const Vector2& rhs) const;

    Vector2& operator+=(const Vector2& rhs);

    Vector2& operator-=(const Vector2& rhs);

    Vector2& operator*=(const Vector2& rhs);

    Vector2& operator/=(const Vector2& rhs);

    float& operator[](int index);

    float operator[](int index) const;

    float X, Y;
};
