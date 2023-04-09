#include "Vector3.hpp"

#include <assert.h>

const Vector3 Vector3::Zero = Vector3(0.0f, 0.0f, 0.0f);

const Vector3 Vector3::One = Vector3(1.0f, 1.0f, 1.0f);

const Vector3 Vector3::AxisX = Vector3(1.0f, 0.0f, 0.0f);

const Vector3 Vector3::AxisY = Vector3(0.0f, 1.0f, 0.0f);

const Vector3 Vector3::AxisZ = Vector3(0.0f, 0.0f, 1.0f);

float Vector3::Dot(const Vector3& lhs, const Vector3& rhs) {
    return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
}

float Vector3::Length(const Vector3& vec) {
    return Math::Sqrt(Dot(vec, vec));
}

float Vector3::Distance(const Vector3& lhs, const Vector3& rhs) {
    return Length(rhs - lhs);
}

Vector3 Vector3::Normalize(const Vector3& vec) {
    const float length = Length(vec);
    return Vector3(vec.X / length, vec.Y / length, vec.Z / length);
}

bool Vector3::AlmostEquals(const Vector3& lhs, const Vector3& rhs) {
    return Math::AlmostEquals(lhs.X, rhs.X) &&
        Math::AlmostEquals(lhs.Y, rhs.Y) &&
        Math::AlmostEquals(lhs.Z, rhs.Z);
}

Vector3::Vector3(float value)
    : X(value), Y(value), Z(value) {
}

Vector3::Vector3(float x, float y, float z)
    : X(x), Y(y), Z(z) {
}

Vector3::Vector3(float x, const Vector2& yz)
    : X(x), Y(yz.X), Z(yz.Y) {
}

Vector3::Vector3(const Vector2& xy, float z)
    : X(xy.X), Y(xy.Y), Z(z) {
}

Vector3 Vector3::operator+(const Vector3& rhs) const {
    return Vector3(X + rhs.X, Y + rhs.Y, Z + rhs.Z);
}

Vector3 Vector3::operator-(const Vector3& rhs) const {
    return Vector3(X - rhs.X, Y - rhs.Y, Z - rhs.Z);
}

Vector3 Vector3::operator*(const Vector3& rhs) const {
    return Vector3(X * rhs.X, Y * rhs.Y, Z * rhs.Z);
}

Vector3 Vector3::operator/(const Vector3& rhs) const {
    return Vector3(X / rhs.X, Y / rhs.Y, Z / rhs.Z);
}

Vector3& Vector3::operator+=(const Vector3& rhs) {
    X += rhs.X; Y += rhs.Y; Z += rhs.Z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& rhs) {
    X -= rhs.X; Y -= rhs.Y; Z -= rhs.Z;
    return *this;
}

Vector3& Vector3::operator*=(const Vector3& rhs) {
    X *= rhs.X; Y *= rhs.Y; Z *= rhs.Z;
    return *this;
}

Vector3& Vector3::operator/=(const Vector3& rhs) {
    X /= rhs.X; Y /= rhs.Y; Z /= rhs.Z;
    return *this;
}

bool Vector3::operator==(const Vector3& rhs) const {
    return X == rhs.X && Y == rhs.Y && Z == rhs.Z;
}

bool Vector3::operator!=(const Vector3& rhs) const {
    return X == rhs.X && Y == rhs.Y && Z == rhs.Z;
}

float& Vector3::operator[](int index) {
    assert(index < 3);
    return (&X)[index];
}

float Vector3::operator[](int index) const {
    assert(index < 3);
    return (&X)[index];
}
