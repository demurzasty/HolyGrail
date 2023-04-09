#include "Vector4.hpp"

#include <assert.h>

const Vector4 Vector4::Zero = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

const Vector4 Vector4::One = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

float Vector4::Dot(const Vector4& lhs, const Vector4& rhs) {
    return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z + lhs.W * rhs.W;
}

float Vector4::Length(const Vector4& vec) {
    return Math::Sqrt(Dot(vec, vec));
}

float Vector4::Distance(const Vector4& lhs, const Vector4& rhs) {
    return Length(rhs - lhs);
}

Vector4 Vector4::Normalize(const Vector4& vec) {
    return vec / Length(vec);
}

Vector4 Vector4::NormalizePlane(const Vector4& vec) {
    return vec / Vector3::Length(Vector3(vec.X, vec.Y, vec.Z));
}

bool Vector4::AlmostEquals(const Vector4& lhs, const Vector4& rhs) {
    return Math::AlmostEquals(lhs.X, rhs.X) &&
        Math::AlmostEquals(lhs.Y, rhs.Y) &&
        Math::AlmostEquals(lhs.Z, rhs.Z) &&
        Math::AlmostEquals(lhs.W, rhs.W);
}

Vector4::Vector4(float value)
    : X(value), Y(value), Z(value), W(value) {
}

Vector4::Vector4(float x, float y, float z, float w)
    : X(x), Y(y), Z(z), W(w) {
}

Vector4::Vector4(const Vector2& xy, const Vector2& zw)
    : X(xy.X), Y(xy.Y), Z(zw.X), W(zw.Y) {
}

Vector4::Vector4(const Vector3& xyz, float w)
    : X(xyz.X), Y(xyz.Y), Z(xyz.Z), W(w) {
}

Vector4 Vector4::operator+(const Vector4& rhs) const {
    return Vector4(X + rhs.X, Y + rhs.Y, Z + rhs.Z, W + rhs.W);
}

Vector4 Vector4::operator-(const Vector4& rhs) const {
    return Vector4(X - rhs.X, Y - rhs.Y, Z - rhs.Z, W - rhs.W);
}

Vector4 Vector4::operator*(const Vector4& rhs) const {
    return Vector4(X * rhs.X, Y * rhs.Y, Z * rhs.Z, W * rhs.W);
}

Vector4 Vector4::operator/(const Vector4& rhs) const {
    return Vector4(X / rhs.X, Y / rhs.Y, Z / rhs.Z, W / rhs.W);
}

Vector4& Vector4::operator+=(const Vector4& rhs) {
    X += rhs.X; Y += rhs.Y; Z += rhs.Z; W += rhs.W;
    return *this;
}

Vector4& Vector4::operator-=(const Vector4& rhs) {
    X -= rhs.X; Y -= rhs.Y; Z -= rhs.Z; W -= rhs.W;
    return *this;
}

Vector4& Vector4::operator*=(const Vector4& rhs) {
    X *= rhs.X; Y *= rhs.Y; Z *= rhs.Z; W *= rhs.W;
    return *this;
}

Vector4& Vector4::operator/=(const Vector4& rhs) {
    X /= rhs.X; Y /= rhs.Y; Z /= rhs.Z; W /= rhs.W;
    return *this;
}

float& Vector4::operator[](int index) {
    assert(index < 4);
    return (&X)[index];
}

float Vector4::operator[](int index) const {
    assert(index < 4);
    return (&X)[index];
}